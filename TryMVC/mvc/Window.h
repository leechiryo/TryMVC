#pragma once

#include "View.h"

#include <d2d1.h>
#include <vector>

namespace mvc {
  class Window : public View<Window>
  {
  private:
    ID2D1HwndRenderTarget* m_pRenderTarget;

  public:
    Window() {}
    Window(const WPViewSet & subViews) : View(subViews) { }

    void DrawSelf() {
      MessageBoxA(0, "MainWindow is drawed", "Test", 0);
    }
  };
}
