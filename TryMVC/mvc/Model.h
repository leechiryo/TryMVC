#pragma once

/*
 * Model提供了一个方法检测Model的值的变化情况，如果
 * 发现了值发生了变化，该方法将返回True。
*/

#include <set>
#include <memory>
#include "ViewBase.h"

class ModelBase {
  typedef weak_ptr<ViewBase> PtrView;
private:
  set<PtrView> m_linkedViews;

public:
  virtual bool ModelChanged() = 0;
  virtual void * GetModelAddr() = 0;

  void AddBindedViews(const PtrView &v) {
    m_linkedViews.insert(v);
  }

  void RemoveBindedViews(const PtrView &v) {
    m_linkedViews.erase(v);
  }

  void UpdateBindedViews() {
    for (auto v : m_linkedViews) {
      auto pv = v.lock();
      if (pv){
        v.lock()->Draw();
      }
      else if (!v.expired) {
        m_linkedViews.erase(v);
      }
    }
  }
};

template <typename T>
class Model: ModelBase {
private:
  T _modelCopy;
  T _model;

public:
  template<typename... Args>
  ModelChecker(Args... args) : _model(args...), _modelCopy(pModel) {
  }

  bool ModelChanged() {
    bool changed = (_modelCopy != _model);
    if (changed) {
      modelCopy = _model;
    }
    return changed;
  }

  const T *Get() {
    return &_model;
  }
};
