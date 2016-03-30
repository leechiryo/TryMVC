﻿#pragma once

#include "ViewBase.h"
#include "App.h"

using namespace std;

namespace mvc {
  template<typename DerivedType>
  class View : public ViewBase {
    typedef LRESULT(*ControllerMethod)(shared_ptr<DerivedType>, WPARAM, LPARAM);
    friend class App;

  private:
    map<int, ControllerMethod> m_eventHandlers;

  protected:


    virtual char HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT &result) {

      bool isMouseEvent = 0;
      int pixelX = 0;
      int pixelY = 0;
      if (msg == WM_MOUSEMOVE || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) {
        // When mouse event occured, we get the coordinate first.
        pixelX = GET_X_LPARAM(lParam);
        pixelY = GET_Y_LPARAM(lParam);
        isMouseEvent = true;
      }

      for (auto v : m_subViews) {
        auto spv = v.lock();
        if (!spv) continue;
        if (isMouseEvent) {
          double dipX = PixelsToDipsX(pixelX);
          double dipY = PixelsToDipsY(pixelY);

          if (!spv->HitTest(dipX, dipY)) {
            // Mouse event should be processed only when it is
            // occured in the element's area.
            if (spv->m_mouseIn) {
              spv->m_mouseIn = 0;
              spv->MouseLeft(dipX, dipY);
            }
            continue;
          }
          else {
            if (!spv->m_mouseIn) {
              spv->m_mouseIn = 1;
              spv->MouseEnter(dipX, dipY);
            }
          }
        }

        // child element will try to process the message first.
        char processed = spv->HandleMessage(msg, wParam, lParam, result);

        // if child processed the message, then return the result
        // and the parent's event handler will be skipped.
        if (processed) return 1;
      }

      // if the child elements are not process the current
      // message, then try the parent element itself.
      auto it = m_eventHandlers.find(msg);
      if (it != m_eventHandlers.end()) {
        auto spThis = m_wpThis.lock();
        if (spThis){
          result = (it->second)(dynamic_pointer_cast<DerivedType>(spThis), wParam, lParam);
          return 1;  // Notify to parent element that it processed the message.
        }
      }

      // Notify to parent element that it doesn't processed the message.
      return 0;
    }

  public:

    View() : ViewBase() {
    }

    View(ID2D1HwndRenderTarget** ppRndrTgt, const WPViewSet & views) : ViewBase(ppRndrTgt, views) {
    }

    void AddEventHandler(int msg, ControllerMethod method) {
      if (m_eventHandlers.find(msg) == m_eventHandlers.end()) {
        m_eventHandlers.insert({ msg, method });
      }
    }

    void FireEvent(int msg) {
      if (m_eventHandlers.find(msg) != m_eventHandlers.end()) {
        auto sharedthis = m_wpThis.lock();
        if (sharedthis) {
          m_eventHandlers[msg](dynamic_pointer_cast<DerivedType>(sharedthis), msg, 0);
        }
        App::UpdateViews();
      }
    }


  };
}
