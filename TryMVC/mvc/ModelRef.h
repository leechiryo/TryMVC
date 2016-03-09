#pragma once

#include <memory>
#include "ModelSafePtr.h"
#include "Model.h"
#include "App.h"

using namespace std;

class ViewBase;

template<typename T>
class ModelRef {

  friend class ViewBase;

private:
  T m_fallback;
  T *m_fieldPtr;
  weak_ptr<ModelBase> m_wpRefModel;
  weak_ptr<ViewBase> m_wpMyView;

public:

  template<typename ... Args>
  ModelRef(Args ... args) : m_fallback(args...), m_wpRefModel() {
    m_fieldPtr = &m_fallback;
  }

  ~ModelRef() {
    m_wpRefModel.reset();
  }

  template<typename M>
  void Bind(string modelId, T M::*mPtr) {
    auto accModel = App::GetModel<M>(modelId);
    m_wpRefModel = accModel.get_spModel();
    if (accModel.isValid()) {
      m_fieldPtr = &(accModel->*mPtr);
    } else {
      throw runtime_error("Can not bind to an object which is destroied.");
    }
  }

  void Bind(string modelId) {
    auto accModel = App::GetModel<T>(modelId);
    m_wpRefModel = accModel.get_spModel();
    if (accModel.isValid()) {
      m_fieldPtr = accModel;
    } else {
      throw runtime_error("Can not bind to an object which is destroied.");
    }
  }

  void UnBind() {
    m_fallback = *m_fieldPtr;
    m_fieldPtr = &m_fallback;
  }

  ModelSafePtr<T> SafePtr() {
    auto sp = m_wpRefModel.lock();
    if (!sp) UnBind();
    return ModelSafePtr<T>{m_fieldPtr, sp};
  }

};