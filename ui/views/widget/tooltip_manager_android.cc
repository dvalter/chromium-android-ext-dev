// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/tooltip_manager_android.h"

#include "base/logging.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/display/screen.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/widget/widget.h"
#include "ui/wm/public/tooltip_client.h"

namespace views {

////////////////////////////////////////////////////////////////////////////////
// TooltipManagerAndroid public:

TooltipManagerAndroid::TooltipManagerAndroid(Widget* widget) : widget_(widget) {
  wm::SetTooltipText(GetWindow(), &tooltip_text_);
}

TooltipManagerAndroid::~TooltipManagerAndroid() {
  wm::SetTooltipText(GetWindow(), nullptr);
}

// static
const gfx::FontList& TooltipManagerAndroid::GetDefaultFontList() {
  return ui::ResourceBundle::GetSharedInstance().GetFontList(
      ui::ResourceBundle::BaseFont);
}

// static
void TooltipManagerAndroid::UpdateTooltipManagerForCapture(Widget* source) {
}

////////////////////////////////////////////////////////////////////////////////
// TooltipManagerAndroid, TooltipManager implementation:

const gfx::FontList& TooltipManagerAndroid::GetFontList() const {
  return GetDefaultFontList();
}

int TooltipManagerAndroid::GetMaxWidth(const gfx::Point& point) const {
  return 0;
}

void TooltipManagerAndroid::UpdateTooltip() {
}

void TooltipManagerAndroid::TooltipTextChanged(View* view)  {
}

View* TooltipManagerAndroid::GetViewUnderPoint(const gfx::Point& point) {
  return nullptr;
}

void TooltipManagerAndroid::UpdateTooltipForTarget(View* target,
                                                const gfx::Point& point,
                                                aura::Window* root_window) {
  if (target) {
    gfx::Point view_point = point;
    View::ConvertPointFromWidget(target, &view_point);
    tooltip_text_ = target->GetTooltipText(view_point);
  } else {
    tooltip_text_.clear();
  }

  wm::SetTooltipId(GetWindow(), target);

  wm::GetTooltipClient(root_window)->UpdateTooltip(GetWindow());
}

aura::Window* TooltipManagerAndroid::GetWindow() {
  return nullptr;
}

}  // namespace views.
