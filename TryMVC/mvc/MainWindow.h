#pragma once

#include "View.h"

#include <vector>

namespace mvc {
  class MainWindow : public View<MainWindow>
  {
  public:
    MainWindow() {}
    MainWindow(const WPViewSet & subViews) : View(subViews) { }

    void DrawSelf() {
      MessageBoxA(0, "MainWindow is drawed", "Test", 0);
    }
  };
}
