#pragma once

#include "ModelChecker.h"
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

  template<typename FieldType>
  void bind(FieldType *(&pField), const FieldType &model) {
    pField = &model;
    ModelChecker<FieldType> m{ model };
  }


};
