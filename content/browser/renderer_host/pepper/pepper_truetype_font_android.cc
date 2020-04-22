// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/renderer_host/pepper/pepper_truetype_font.h"

#include <stddef.h>
#include <stdint.h>

#include <memory>

#include "base/compiler_specific.h"
#include "base/files/scoped_file.h"
#include "base/macros.h"
#include "base/numerics/safe_conversions.h"
#include "base/sys_byteorder.h"
#include "build/build_config.h"
#include "components/services/font/ppapi_fontconfig_matching.h"
#include "content/public/common/common_sandbox_support_linux.h"
#include "ppapi/buildflags/buildflags.h"
#include "ppapi/c/dev/ppb_truetype_font_dev.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/trusted/ppb_browser_font_trusted.h"

namespace content {

namespace {

class PepperTrueTypeFontAndroid : public PepperTrueTypeFont {
 public:
  PepperTrueTypeFontAndroid();

  // PepperTrueTypeFont implementation.
  int32_t Initialize(ppapi::proxy::SerializedTrueTypeFontDesc* desc) override;
  int32_t GetTableTags(std::vector<uint32_t>* tags) override;
  int32_t GetTable(uint32_t table_tag,
                   int32_t offset,
                   int32_t max_data_length,
                   std::string* data) override;

 private:
  ~PepperTrueTypeFontAndroid() override;

  base::ScopedFD fd_;

  DISALLOW_COPY_AND_ASSIGN(PepperTrueTypeFontAndroid);
};

PepperTrueTypeFontAndroid::PepperTrueTypeFontAndroid() {
}

PepperTrueTypeFontAndroid::~PepperTrueTypeFontAndroid() {
}

int32_t PepperTrueTypeFontAndroid::Initialize(
    ppapi::proxy::SerializedTrueTypeFontDesc* desc) {
  return fd_.is_valid() ? PP_OK : PP_ERROR_FAILED;
}

int32_t PepperTrueTypeFontAndroid::GetTableTags(std::vector<uint32_t>* tags) {
  return PP_ERROR_FAILED;
}

int32_t PepperTrueTypeFontAndroid::GetTable(uint32_t table_tag,
                                          int32_t offset,
                                          int32_t max_data_length,
                                          std::string* data) {
  return PP_ERROR_FAILED;
}

}  // namespace

// static
PepperTrueTypeFont* PepperTrueTypeFont::Create() {
  return new PepperTrueTypeFontAndroid();
}

}  // namespace content
