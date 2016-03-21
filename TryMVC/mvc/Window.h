#pragma once

#include "View.h"

#include <vector>

namespace mvc {
  class Window : public View<Window>
  {
  public:
    Window() {}
    Window(const WPViewSet & subViews) : View(subViews) { }

    void DrawSelf() {
      MessageBoxA(0, "MainWindow is drawed", "Test", 0);
    }
  };
}
