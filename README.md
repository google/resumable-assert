[![Apache
License](https://img.shields.io/github/license/google/resumable-assert.svg)](LICENSE)

# Resumable Assert

## Assert replacement to continue execution in debugger

In any large app, it's entirely possible some assert is failing in code you
don't currently care about, and blocking the entire team from being able to run
the app until the issue is fixed is not the best workflow. So we usually end up
moving the execution marker past the assert line in IDE or debugger, or even
comment the assert out, recompile and relaunch. With `ResumableAssert`, you can
finally ignore or disable asserts that you are not interested in.

_Disclaimer: be careful with that power though, since execution of potentially
broken code may lead to unrecoverable errors in future._

## C/C++/Objective-C

First, include or import the header:

```objectivec
#import "ResumableAssert.h"
```

Then, use the `RESUMABLE_ASSERT()` or `RESUMABLE_ASSERT_WITH_FORMAT()` macros
instead of the standard ones:
[`assert()`](http://en.cppreference.com/w/c/error/assert),
[`NSAssert()`](https://developer.apple.com/documentation/foundation/nsassert),
etc.

Once `RESUMABLE_ASSERT()` variant is hit in debug mode, you can ignore or
disable it, or even disable all asserts permanently with corresponding `lldb`
commands when prompted.

For example, the code:

```c
RESUMABLE_ASSERT(2 + 2 == 5);
// Or:
RESUMABLE_ASSERT_WITH_FORMAT(2 + 2 == 5, "Calculation failure");
```

Leads to the following debugger console output:

```sh
ViewController.m:-[ViewController viewDidLoad]:42
2 + 2 == 5
Calculation failure
Type the following lldb commands to resolve and continue:
e ignore = 1   # ignore this assert this time
e disable = 1  # disable this assert permanently
e unleash = 1  # disable all asserts permanently
```

Where you can choose an option and continue:

```sh
(lldb) e ignore = 1
(volatile int) $1 = 1
(lldb) c
Process 11193 resuming
```

## Custom logging

`RESUMABLE_ASSERT()` macro in C use `stdout` to print the failure message by
default. To change this behavior and use something else for logging,
(e.g. [`NSLog()`](https://developer.apple.com/documentation/foundation/1395275-nslog))
redefine the `RESUMABLE_ASSERT_LOG()` macro in C like so:

```objectivec
#import <Foundation/Foundation.h>

#include "ResumableAssert.h"

#undef RESUMABLE_ASSERT_LOG
#define RESUMABLE_ASSERT_LOG(__condition__, __format__, ...)            \
  do {                                                                  \
    NSLog(@"%s:%u\n%s\n" __format__,                                    \
          __PRETTY_FUNCTION__, __LINE__, __condition__, ##__VA_ARGS__); \
  } while (0)
```

## Swift

For Swift and other languages we provide `resumableAssertDebugTrap()`
function that implements the core loop of resumable assert. You can then
implement a custom assert function which would use it internally:

```swift
import ResumableAssert

public func assert(
  _ condition: @autoclosure () -> Bool,
  _ message: @autoclosure () -> String = "",
  file: StaticString = #file,
  function: StaticString = #function,
  line: UInt = #line
) {
#ifdef DEBUG
  if !condition() {
    print("Assertion failed: " + (message.isEmpty ? "" : "\(message): ") +
          "file \(file.description), function \(function.description), line \(line)")
    resumableAssertDebugTrap()
  }
#endif
}
```
