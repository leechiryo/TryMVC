#pragma once

#include <map>
#include <string>
#include <memory>
#include "Model.h"
#include "ViewBase.h"
#include "App.h"

using namespace std;

template<typename DerivedType>
class View : public ViewBase {
  typedef void(*ControllerMethod)(shared_ptr<DerivedType>, int param);
  friend class App;

private:
  map<int, ControllerMethod> m_eventHandlers;
  weak_ptr<DerivedType> m_weakthis;

public:

  View() : ViewBase(){
  }

  View(const list<ViewBase::PtrView> & views) : ViewBase(views){
  }

  void AddEventHandler(int msg, ControllerMethod method) {
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()) {
      m_eventHandlers.insert({ msg, method });
    }
  }

  void FireEvent(int msg){
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()){

      auto sharedthis = m_weakthis.lock();
      if (sharedthis){
        m_eventHandlers[msg](sharedthis, msg);
      }
      // todo: call App::UpdateViews
    }
  }

  template<typename FieldType>
  void bind(weak_ptr<Model<FieldType>> &pfield, weak_ptr<Model<FieldType>> &model) {
    pfield = model;
    model->AddBindedViews(m_weakthis);
  }

};
