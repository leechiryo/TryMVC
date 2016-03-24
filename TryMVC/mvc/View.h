#pragma once

#include "ViewBase.h"
#include "App.h"

using namespace std;

namespace mvc {
  template<typename DerivedType>
  class View : public ViewBase {
    typedef void(*ControllerMethod)(shared_ptr<DerivedType>, int param);
    friend class App;

  private:
    map<int, ControllerMethod> m_eventHandlers;

  protected:
    // 指向Window对象的D2DRenderTarget字段的指针。每个Window都有一个独立的D2DRenderTarget对象，
    // 其内部的所有subview将共享这一对象，并利用该对象进行绘制。
    ID2D1HwndRenderTarget** m_ppRenderTarget;

  public:

    View() : ViewBase() {
    }

    View(const WPViewSet & views) : ViewBase(views) {
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
          m_eventHandlers[msg](dynamic_pointer_cast<DerivedType>(sharedthis), msg);
        }
        App::UpdateViews();
      }
    }
  };
}
