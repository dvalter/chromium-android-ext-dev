// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/platform_util.h"

#include "base/bind.h"
#include "base/files/file.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/task/post_task.h"
#include "chrome/browser/platform_util_internal.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"

using content::BrowserThread;

namespace platform_util {

namespace {

bool shell_operations_allowed = true;

}  // namespace

namespace internal {

void DisableShellOperationsForTesting() {
  shell_operations_allowed = false;
}

}  // namespace internal

bool IsBrowserLockedFullscreen(const Browser* browser) {
  return false;
}

}  // namespace platform_util
