#pragma once

#include "new_ViewBase.h"

namespace mvc2 {

  template<typename DerivedType>
  class View : public ViewBase {
    typedef void(*ControllerMethod)(shared_ptr<DerivedType>, int param);

  private:
    map<int, ControllerMethod> m_eventHandlers;

  public:

    View(string id) : ViewBase(id) { }

    View(string id, const WPViewSet & views) : ViewBase(id, views) { }

    virtual ~View() {}

    void AddEventHandler(int msg, ControllerMethod method) {
      if (m_eventHandlers.find(msg) == m_eventHandlers.end()) {
        m_eventHandlers.insert({ msg, method });
      }
    }

    void FireEvent(int msg) {
      auto it = m_eventHandlers.find(msg);
      if (it != m_eventHandlers.end()) {
        auto spThis = m_wpThis.lock();
        if (spThis) {
          it->second(dynamic_pointer_cast<DerivedType>(spThis), msg);
        }
        ModelBase::UpdateViews();
      }
    }
  };

}