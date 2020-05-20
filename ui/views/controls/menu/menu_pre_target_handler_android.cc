// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/menu/menu_pre_target_handler_android.h"

#include "ui/views/controls/menu/menu_controller.h"
#include "ui/views/widget/widget.h"
#include "ui/wm/public/activation_client.h"

namespace views {

namespace {

ui::WindowAndroid* GetOwnerRootWindow(views::Widget* owner) {
  return nullptr;
}

}  // namespace

MenuPreTargetHandlerAndroid::MenuPreTargetHandlerAndroid(MenuController* controller,
                                                   Widget* owner)
    : controller_(controller), root_(GetOwnerRootWindow(owner)) {
}

MenuPreTargetHandlerAndroid::~MenuPreTargetHandlerAndroid() {
  Cleanup();
}

void MenuPreTargetHandlerAndroid::OnCancelMode(ui::CancelModeEvent* event) {
  controller_->Cancel(MenuController::ExitType::kAll);
}

void MenuPreTargetHandlerAndroid::OnKeyEvent(ui::KeyEvent* event) {
  controller_->OnWillDispatchKeyEvent(event);
}

void MenuPreTargetHandlerAndroid::Cleanup() {
}

// static
std::unique_ptr<MenuPreTargetHandler> MenuPreTargetHandler::Create(
    MenuController* controller,
    Widget* owner) {
  return nullptr;
}

}  // namespace views
