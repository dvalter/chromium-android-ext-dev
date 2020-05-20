// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/event_monitor_android.h"

#include <memory>

#include "base/logging.h"
#include "base/scoped_observer.h"
#include "ui/android/window_android.h"
#include "ui/events/event_observer.h"
#include "ui/events/event_target.h"

namespace views {

namespace {

// An EventMonitorAndroid that removes its event observer on window destruction.
class WindowMonitorAndroid : public EventMonitorAndroid {
 public:
  WindowMonitorAndroid(ui::EventObserver* event_observer,
                    ui::WindowAndroid* target_window,
                    const std::set<ui::EventType>& types)
      : EventMonitorAndroid(event_observer, target_window, types),
        target_window_(target_window) {
  }
  ~WindowMonitorAndroid() override = default;

 private:
  ui::WindowAndroid* target_window_;

  DISALLOW_COPY_AND_ASSIGN(WindowMonitorAndroid);
};

}  // namespace

// static
std::unique_ptr<EventMonitor> EventMonitor::CreateApplicationMonitor(
    ui::EventObserver* event_observer,
    gfx::NativeWindow context,
    const std::set<ui::EventType>& types) {
  return std::make_unique<EventMonitorAndroid>(event_observer,
                                            nullptr, types);
}

// static
std::unique_ptr<EventMonitor> EventMonitor::CreateWindowMonitor(
    ui::EventObserver* event_observer,
    gfx::NativeWindow target_window,
    const std::set<ui::EventType>& types) {
  return std::make_unique<WindowMonitorAndroid>(event_observer, nullptr,
                                             types);
}

EventMonitorAndroid::EventMonitorAndroid(ui::EventObserver* event_observer,
                                   ui::EventTarget* event_target,
                                   const std::set<ui::EventType>& types)
    : event_observer_(event_observer), event_target_(event_target) {
  DCHECK(event_observer_);
  DCHECK(event_target_);
}

EventMonitorAndroid::~EventMonitorAndroid() {
  TearDown();
}

gfx::Point EventMonitorAndroid::GetLastMouseLocation() {
  return gfx::Point();
}

void EventMonitorAndroid::TearDown() {
  event_observer_ = nullptr;
}

}  // namespace views
