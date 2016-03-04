#pragma once

#include <memory>
#include "Model.h"

using namespace std;

template<typename T>
class ModelAccessor {
private:
  T *m_fieldPtr;
  shared_ptr<ModelBase> m_spModel;

public:
  ModelAccessor(T *p, const shared_ptr<ModelBase> &pm) {
    m_fieldPtr = p;
    m_spModel = pm;
  }

  ModelAccessor<T>& operator=(const T &val) {
    (*m_fieldPtr) = val;
    return *this;
  }

  ModelAccessor<T>& operator=(const ModelAccessor<T> &val) {
    m_fieldPtr = val.m_fieldPtr;
    m_spModel = val.m_spModel;
    return *this;
  }

  operator bool() const {
    if (m_spModel) return true;
    else return false;
  }

  T* operator->() {
    if (m_spModel) return m_fieldPtr;
    else return nullptr;
  }
};

struct C;

template<typename T>
class ModelRef {
private:
  T m_fallback;
  T *m_modelPtr;
  weak_ptr<ModelBase> m_wpChecker;

public:

  template<typename ... Args>
  ModelRef(Args ... args) : m_fallback(args...) {
    m_modelPtr = &m_fallback;
  }

  ~ModelRef() {
    m_wpChecker.reset();
  }

  template<typename M>
  bool Bind(weak_ptr<Model<M>> mObj, double M::*mPtr) {
    m_wpChecker = mObj;
    auto spMObj = mObj.lock();
    if (spMObj) {
      m_modelPtr = &(spMObj->get_ref().*mPtr);
      return true;
    }
    return false;
  }

  template<typename M>
  bool Bind(weak_ptr<Model<M>> mObj) {
    m_wpChecker = mObj;
    auto spMObj = mObj.lock();
    if (spMObj) {
      m_modelPtr = &(*spMObj);
      return true;
    }
    return false;
  }

  void UnBind() {
    m_fallback = *m_modelPtr;
    m_modelPtr = &m_fallback;
  }

  ModelAccessor<T> GetAccessor() {
    auto sp = m_wpChecker.lock();
    return ModelAccessor<T>{m_modelPtr, sp};
  }

};