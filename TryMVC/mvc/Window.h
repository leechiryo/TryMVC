#pragma once

#include "View.h"

#include <d2d1.h>
#include <vector>

namespace mvc {
  class Window : public View<Window>
  {
  private:
    ID2D1HwndRenderTarget* m_pRenderTarget;
    HWND m_hwnd;

  protected:
    virtual void CreateD2DResource() {

      HRESULT hr = S_OK;

      if (!m_pRenderTarget) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = App::s_pDirect2dFactory->CreateHwndRenderTarget(
          D2D1::RenderTargetProperties(),
          D2D1::HwndRenderTargetProperties(m_hwnd, size),
          &m_pRenderTarget);
      }
    }

    virtual void DestoryD2DResource() {
      SafeRelease(m_pRenderTarget);
    }

  public:
    Window() {}
    Window(const WPViewSet & subViews) : View(subViews) { }

    virtual ~Window() {
      SafeRelease(m_pRenderTarget);
      m_ppRenderTarget = nullptr;
    }

    void DrawSelf() {
      MessageBoxA(0, "MainWindow is drawed", "Test", 0);
    }
  };
}
