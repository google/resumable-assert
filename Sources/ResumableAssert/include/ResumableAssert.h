/**
 Copyright 2018 Google Inc. All rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at:

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef RESUMABLE_ASSERT_H_
#define RESUMABLE_ASSERT_H_

#include <stdio.h>

// Redefine RESUMABLE_ASSERT_LOG with a proper logging implementation if needed.
#define RESUMABLE_ASSERT_LOG(condition, format, ...)                           \
  do {                                                                         \
    printf("%s:%s:%u\nAssertion failed: %s\n" format,                          \
           __FILE__, __PRETTY_FUNCTION__, __LINE__, condition, ##__VA_ARGS__); \
  } while (0)

extern
#ifdef __cplusplus
"C"
#endif
// Helper function that wraps RESUMABLE_ASSERT_DEBUG_TRAP to use it in other
// languages.
void ResumableAssertDebugTrap(void);

#ifndef DEBUG

#define RESUMABLE_ASSERT_WITH_FORMAT(condition, format, ...) \
  do {} while (0)
#define RESUMABLE_ASSERT(condition) \
  do {} while (0)

#else  // #ifndef DEBUG

extern int gResumableAssertsAreAllDisabled;

extern
#ifdef __cplusplus
"C"
#endif
int ResumableAssertIsDebuggerAttached(void);

// RESUMABLE_DEBUG_BREAK is same as __builtin_trap, but allows resuming
// execution after the break.
#if __i386__ || __x86_64__
#define RESUMABLE_ASSERT_DEBUG_BREAK() asm("int3")
#elif __arm__
#define RESUMABLE_ASSERT_DEBUG_BREAK()                    \
  asm("mov r0, %0    \n" /* PID                        */ \
      "mov r1, 0x11  \n" /* SIGSTOP                    */ \
      "mov r12, 0x25 \n" /* syscall kill               */ \
      "svc 0x80      \n" /* software interrupt         */ \
      "mov r0, r0    \n" /* nop to pause debugger here */ \
      ::"r"(getpid())                                     \
      : "r0", "r1", "r12", "memory")
#elif __arm64__
#define RESUMABLE_ASSERT_DEBUG_BREAK()                    \
  asm("mov x0, %x0   \n" /* PID                        */ \
      "mov x1, 0x11  \n" /* SIGSTOP                    */ \
      "mov x16, 0x25 \n" /* syscall kill               */ \
      "svc 0x80      \n" /* software interrupt         */ \
      "mov x0, x0    \n" /* nop to pause debugger here */ \
      ::"r"(getpid())                                     \
      : "x0", "x1", "x16", "memory")
#else
#error "Unsupported architecture."
#endif  // __i386__ || __x86_64__

#define RESUMABLE_ASSERT_DEBUG_TRAP()                                          \
  do {                                                                         \
    static volatile int gResumableAssertIsDisabled;                            \
    if (gResumableAssertIsDisabled || gResumableAssertsAreAllDisabled) break;  \
    if (!ResumableAssertIsDebuggerAttached()) __builtin_trap();                \
    fprintf(stderr,                                                            \
        "\nContinue execution or issue one of the following lldb commands:\n"  \
        "e disable = 1  # disable this assert permanently\n"                   \
        "e unleash = 1  # disable all asserts permanently\n\n");               \
    volatile int disable = 0, unleash = 0;                                     \
    RESUMABLE_ASSERT_DEBUG_BREAK();                                            \
    gResumableAssertIsDisabled = disable;                                      \
    gResumableAssertsAreAllDisabled = unleash;                                 \
  } while (0)

#define RESUMABLE_ASSERT_WITH_FORMAT(condition, format, ...)   \
  do {                                                         \
    if (__builtin_expect(!(condition), 0)) {                   \
      RESUMABLE_ASSERT_LOG(#condition, format, ##__VA_ARGS__); \
      RESUMABLE_ASSERT_DEBUG_TRAP();                           \
    }                                                          \
  } while (0)

#define RESUMABLE_ASSERT(condition) \
  RESUMABLE_ASSERT_WITH_FORMAT(condition, "")

#endif  // #ifndef DEBUG

#endif  // RESUMABLE_ASSERT_H_
