#pragma once

#include <map>
#include <string>
#include <memory>
#include "ViewBase.h"
#include "App.h"

using namespace std;

template<typename DerivedType>
class View : public ViewBase {
  typedef void(*ControllerMethod)(shared_ptr<DerivedType>, int param);
  friend class App;

private:
  map<int, ControllerMethod> m_eventHandlers;

public:

  View() : ViewBase(){
  }

  View(const list<PtrView> & views) : ViewBase(views){
  }

  void AddEventHandler(int msg, ControllerMethod method) {
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()) {
      m_eventHandlers.insert({ msg, method });
    }
  }

  void FireEvent(int msg){
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()){
      auto sharedthis = m_wpThis.lock();
      if (sharedthis){
        m_eventHandlers[msg]( dynamic_pointer_cast<DerivedType>(sharedthis), msg);
      }
      App::UpdateViews();
    }
  }
};
