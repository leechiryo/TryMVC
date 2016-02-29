#pragma once

#include "Model.h"
#include "ViewBase.h"
#include <map>
#include <string>

using namespace std;

template<typename DerivedType>
class View : public ViewBase {
  typedef void(*ControllerMethod)(DerivedType*, int param);

private:
  map<int, ControllerMethod> m_eventHandlers;

public:
  void AddEventHandler(int msg, ControllerMethod method) {
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()) {
      m_eventHandlers.insert({ msg, method });
    }
  }

  void FireEvent(int msg){
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()){
      m_eventHandlers[msg](static_cast<DerivedType*>(this), msg);
      // todo: call App::UpdateViews
    }
  }

  template<typename FieldType>
  void bind(weak_ptr<Model<FieldType>> &pfield, weak_ptr<Model<FieldType>> &model) {
    pfield = &model;
  }

};
