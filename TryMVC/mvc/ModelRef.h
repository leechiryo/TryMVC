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
  void Bind(weak_ptr<Model<M>> mObj, double M::*mPtr) {
    m_wpChecker = mObj;
    auto spMObj = mObj.lock();
    if (spMObj) {
      m_modelPtr = &(spMObj->get_ref().*mPtr);
    } else {
      throw runtime_error("Can not bind to an object which is destroied.");
    }
  }

  template<typename M>
  void Bind(weak_ptr<Model<M>> mObj) {
    m_wpChecker = mObj;
    auto spMObj = mObj.lock();
    if (spMObj) {
      m_modelPtr = &(spMObj->get_ref());
    } else {
      throw runtime_error("Can not bind to an object which is destroied.");
    }
  }

  void UnBind() {
    m_fallback = *m_modelPtr;
    m_modelPtr = &m_fallback;
  }

  ModelAccessor<T> GetAccessor() {
    auto sp = m_wpChecker.lock();
    if (!sp) UnBind();
    return ModelAccessor<T>{m_modelPtr, sp};
  }

};