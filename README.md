[![Apache
License](https://img.shields.io/github/license/google/resumable-assert.svg)](LICENSE)

# Resumable Assert

![](https://gist.githubusercontent.com/shoumikhin/56ca2bf71e6b469d71488a50eb9f293b/raw/ce1860cfc638fa42247d814863e9abbd680e0edb/demo.gif)

## Assert replacement to continue execution in debugger

In any large app, it sometimes happens that some asserts are failing in code you
don't currently care about, and blocking the entire team from being able to run
the app until the issue is fixed is not the best workflow. So we usually end up
moving the execution marker past the assert line in IDE or debugger, or even
comment the assert out, recompile and relaunch. With Resumable Assert, you can
simply continue execution when an assertion fails in debugger, or even disable
asserts that you are not interested in, so that those never bother you again.

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

Once `RESUMABLE_ASSERT()` variant is hit in debug mode, you can ignore it and
continue execution, or disable it permanently, or even disable all asserts
permanently with corresponding `lldb` commands when prompted.

For example, the following assert somewhere in `ViewController.viewDidLoad`
method:

```c
RESUMABLE_ASSERT(2 + 2 == 5);
```

Or:

```c
RESUMABLE_ASSERT_WITH_FORMAT(2 + 2 == 5, "Calculation error");
```

Leads to the following debugger console output:

```sh
ViewController.m:-[ViewController viewDidLoad]:42
Assertion failed: 2 + 2 == 5
Calculation error
Continue execution or issue one of the following lldb commands:
e disable = 1  # disable this assert permanently
e unleash = 1  # disable all asserts permanently
```

Where you can just continue execution, or additionally disable the assert:

```sh
(lldb) e disable = 1
(volatile int) $1 = 1
(lldb) c
Process 11193 resuming
```

## Custom logging

`RESUMABLE_ASSERT()` macro in C uses `stdout` to print the failure message by
default. To change this behavior and use something else for logging,
(e.g. [`NSLog()`](https://developer.apple.com/documentation/foundation/1395275-nslog))
redefine the `RESUMABLE_ASSERT_LOG()` macro in C like so:

```objectivec
#import <Foundation/Foundation.h>

#include "ResumableAssert.h"

#undef RESUMABLE_ASSERT_LOG
#define RESUMABLE_ASSERT_LOG(condition, format, ...)                 \
  do {                                                               \
    NSLog(@"%s:%u\nAssertion failed: %s\n" format,                   \
          __PRETTY_FUNCTION__, __LINE__, condition, ##__VA_ARGS__);  \
  } while (0)
```

Similarly, you can define your own assert macro with a custom name:

```objectivec
#define ASSERT_WITH_FORMAT(condition, format, ...) \
  RESUMABLE_ASSERT_WITH_FORMAT(condition, format, ##__VA_ARGS__)
#define ASSERT(condition) ASSERT_WITH_FORMAT(condition, "")
```

## Swift

For Swift and other languages, we provide `ResumableAssertDebugTrap()` function
that implements the core logic of resumable assert. You can then implement
a custom `assert()` function somewhere in a custom `Diagnostics` module which
would use `ResumableAssertDebugTrap()` internally:

```swift
import ResumableAssert // Import the module or use a bridging header and import ResumableAssert.h.

public func assert(
  _ condition: @autoclosure () -> Bool,
  _ message: @autoclosure () -> String = "",
  file: StaticString = #file,
  function: StaticString = #function,
  line: UInt = #line
) {
#ifdef DEBUG
  if !condition() {
    NSLog("Assertion failed:" + " \(message()):" +
          " file \(file.description), function \(function.description), line \(line)")
    ResumableAssertDebugTrap()
  }
#endif
}
```

Then, you can use the new function as:

```swift
Diagnostics.assert(2 + 2 == 5, "Calculation error")
```

## Setup

### Bazel

In your `BUILD` file, add `ResumableAssert` deps to corresponding targets:

```python
objc_library(
  # ...
  deps = [
    "//path/to/ResumableAssert",
  ],
  # ...
)
```

Include or import the header:

```objectivec
#import "path/to/ResumableAssert/ResumableAssert.h"
```

### CocoaPods

To use `ResumableAssert` for Objective-C, add the following to your `Podfile`:

```ruby
pod 'ResumableAssert', '~> 1.0'
```

Then, run `pod install` and import the umbrella header in generated project:

```objectivec
#import <ResumableAssert/ResumableAssert.h>
```

Or, the module:

```objectivec
@import ResumableAssert;
```
