#pragma once

/*
 * Model提供了一个方法检测Model的值的变化情况，如果
 * 发现了值发生了变化，该方法将返回True。
*/

#include <list>
#include <memory>
#include "WeakPtrComparer.h"
#include "ViewBase.h"
#include "ModelSafePtr.h"

template<typename T>
class ModelRef;

class App;

class ModelBase {

private:
  set <PtrView, WeakPtrComparer<ViewBase>> m_linkedViews;

public:
  virtual bool ModelChanged() = 0;

  void AddBindedView(const PtrView &v) {
    m_linkedViews.insert(v);
  }

  void RemoveBindedView(const PtrView &v) {
    m_linkedViews.erase(v);
  }

  void UpdateBindedViews() {
    for (auto v : m_linkedViews) {
      auto pv = v.lock();
      if (pv) {
        v.lock()->Draw();
      }
      else if (v.expired()) {
        m_linkedViews.erase(v);
      }
    }
  }
};

template <typename T>
class Model : public ModelBase {
  friend class App;
  friend class ModelRef<T>;

private:
  T m_modelCopy;
  T m_model;
  weak_ptr<Model<T>> m_wpThis;

  ModelSafePtr<T> get_accessor() {
    auto spThis = m_wpThis.lock();
    if (spThis) {
      return ModelSafePtr<T>{&m_model, spThis};
    } else {
      throw runtime_error("The model has been deleted.");
    }
  }

  T* get_ptr() {
    return &m_model;
  }

public:
  template<typename... Args>
  Model(Args... args) : m_model(args...) {
    m_modelCopy = m_model;
  }

  bool ModelChanged() {
    bool changed = (m_modelCopy != m_model);
    if (changed) {
      m_modelCopy = m_model;
    }
    return changed;
  }
};
