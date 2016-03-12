#pragma once

#include <memory>

using namespace std;

class ModelBase;

template<typename T>
class ModelSafePtr {
private:
  T *m_fieldPtr;
  shared_ptr<ModelBase> m_spModel;  // a shared pointer which to ensure the model will not be destroied while access to it.

public:
  ModelSafePtr(T *p, const shared_ptr<ModelBase> &pm) {
    m_fieldPtr = p;
    m_spModel = pm;
  }

  bool isValid() const {
    if (m_spModel) return true;
    else return false;
  }

  T* operator->() {
    return m_fieldPtr;
  }

  operator T*() const {
    return m_fieldPtr;
  }

  shared_ptr<ModelBase> get_spModel() {
    return m_spModel;
  }
};