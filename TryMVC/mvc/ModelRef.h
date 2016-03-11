#pragma once

#include <memory>
#include "ModelSafePtr.h"

using namespace std;

class ViewBase;
class ModelBase;

template<typename T>
class ModelRef {

  friend class ViewBase;

private:
  T m_fallback;
  T *m_fieldPtr;
  weak_ptr<ModelBase> m_wpRefModel;
  ViewBase *m_pMyView;

public:

  template<typename ... Args>
  ModelRef(ViewBase * pView, Args ... args) : m_fallback(args...), m_wpRefModel() {
    m_fieldPtr = &m_fallback;
    m_pMyView = pView;
  }

  ~ModelRef() {
    m_wpRefModel.reset();
  }

  template<typename M>
  void Bind(string modelId, T M::*mPtr) {
    auto spModel = App::GetModel<M>(modelId);
    m_wpRefModel = spModel.get_spModel();
    if (spModel.isValid()) {
      m_fieldPtr = &(spModel->*mPtr);
      if (m_pMyView){
        spModel.get_spModel()->AddBindedView(m_pMyView->m_wpThis);
      }
    } else {
      throw runtime_error("Can not bind to an object which is destroied.");
    }
  }

  void Bind(string modelId) {
    auto spModel = App::GetModel<T>(modelId);
    m_wpRefModel = spModel.get_spModel();
    if (spModel.isValid()) {
      m_fieldPtr = spModel;
      if (m_pMyView){
        spModel.get_spModel()->RemoveBindedView(m_pMyView->m_wpThis);
      }
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