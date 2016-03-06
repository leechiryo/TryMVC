#pragma once

#include <memory>
#include "ModelAccessor.h"
#include "Model.h"
#include "App.h"

using namespace std;

template<typename T>
class ModelRef {
private:
  T m_fallback;
  T *m_fieldPtr;
  weak_ptr<ModelBase> m_wpModel;

public:

  template<typename ... Args>
  ModelRef(Args ... args) : m_fallback(args...) {
    m_fieldPtr = &m_fallback;
  }

  ~ModelRef() {
    m_wpModel.reset();
  }

  template<typename M>
  void Bind(string modelId, double M::*mPtr) {
    auto accModel = App::GetModel<M>(modelId);
    m_wpModel = accModel.get_spModel();
    if (accModel.isValid()) {
      m_fieldPtr = &(accModel.operator->()->*mPtr);
    } else {
      throw runtime_error("Can not bind to an object which is destroied.");
    }
  }

  void Bind(string modelId) {
    auto accModel = App::GetModel<T>(modelId);
    m_wpModel = accModel.get_spModel();
    if (accModel.isValid()) {
      m_fieldPtr = accModel.operator->();
    } else {
      throw runtime_error("Can not bind to an object which is destroied.");
    }
  }

  void UnBind() {
    m_fallback = *m_fieldPtr;
    m_fieldPtr = &m_fallback;
  }

  ModelAccessor<T> Access() {
    auto sp = m_wpModel.lock();
    if (!sp) UnBind();
    return ModelAccessor<T>{m_fieldPtr, sp};
  }

};