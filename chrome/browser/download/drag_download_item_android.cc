// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/download/drag_download_item.h"

#include <string>

#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "components/download/public/common/download_item.h"
#include "net/base/mime_util.h"
#include "ui/android/window_android.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/file_info/file_info.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/display/screen.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/button_drag_utils.h"
#include "ui/views/widget/widget.h"
#include "url/gurl.h"


void DragDownloadItem(const download::DownloadItem* download,
                      gfx::Image* icon,
                      gfx::NativeView view) {
  DCHECK(download);
  DCHECK_EQ(download::DownloadItem::COMPLETE, download->GetState());
  return;
}
