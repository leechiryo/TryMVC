#pragma once

/*
 * Model提供了一个方法检测Model的值的变化情况，如果
 * 发现了值发生了变化，该方法将返回True。
*/

#include <list>
#include <memory>
#include "WeakPtrComparer.h"
#include "ViewBase.h"

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
      if (pv){
        v.lock()->Draw();
      }
      else if (v.expired()) {
        m_linkedViews.erase(v);
      }
    }
  }
};

template <typename T>
class Model: public ModelBase {
private:
  T _modelCopy;
  T _model;

public:
  template<typename... Args>
  Model(Args... args) : _model(args...) {
    _modelCopy = _model;
  }

  bool ModelChanged() {
    bool changed = (_modelCopy != _model);
    if (changed) {
      _modelCopy = _model;
    }
    return changed;
  }

  T* operator->() {
    return &_model;
  }
};
