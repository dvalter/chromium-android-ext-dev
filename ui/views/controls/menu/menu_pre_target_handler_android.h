// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_PRE_TARGET_HANDLER_ANDROID_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_PRE_TARGET_HANDLER_ANDROID_H_

#include "ui/android/window_android.h"
#include "ui/events/event_handler.h"
#include "ui/views/controls/menu/menu_pre_target_handler.h"
#include "ui/views/views_export.h"

namespace views {

class MenuController;
class Widget;

// MenuPreTargetHandlerAndroid is used to observe activation changes, cancel
// events, and root window destruction, to shutdown the menu.
//
// Additionally handles key events to provide accelerator support to menus.
class VIEWS_EXPORT MenuPreTargetHandlerAndroid
    : public ui::EventHandler,
      public MenuPreTargetHandler {
 public:
  MenuPreTargetHandlerAndroid(MenuController* controller, Widget* owner);
  ~MenuPreTargetHandlerAndroid() override;

  // ui::EventHandler:
  void OnCancelMode(ui::CancelModeEvent* event) override;
  void OnKeyEvent(ui::KeyEvent* event) override;

 private:
  void Cleanup();

  MenuController* controller_;
  ui::WindowAndroid* root_;

  DISALLOW_COPY_AND_ASSIGN(MenuPreTargetHandlerAndroid);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_MENU_MENU_PRE_TARGET_HANDLER_ANDROID_H_
