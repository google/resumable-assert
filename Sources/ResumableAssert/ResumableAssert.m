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

#include "ResumableAssert.h"

#include <sys/sysctl.h>
#include <unistd.h>

int gResumableAssertsAreAllDisabled;

void ResumableAssertDebugTrap() {
#ifdef RESUMABLE_ASSERT_DEBUG_TRAP
  RESUMABLE_ASSERT_DEBUG_TRAP();  // See the debugger console and continue.
#endif  // RESUMABLE_ASSERT_DEBUG_TRAP
}

int ResumableAssertIsDebuggerAttached() {
  // See http://developer.apple.com/library/mac/#qa/qa1361/_index.html
  int mib[] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid()};
  struct kinfo_proc info;
  size_t size = sizeof(info);
  return sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0) == 0 ?
      (info.kp_proc.p_flag & P_TRACED) != 0 : 0;
}
