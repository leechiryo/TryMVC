#pragma once

/*
 * 每个Model有一个ModelChecker与之联系在一起，
 * ModelChecker提供了一个方法检测Model的值的变化情况，如果
 * 发现了值发生了变化，该方法将返回True。
*/

#include <set>
#include <memory>
#include "ViewBase.h"

class ModelCheckerBase {
  typedef ViewBase *PtrView;
private:
  set<PtrView> m_linkedViews;

public:
  virtual bool ModelChanged() = 0;
  virtual void * GetModelAddr() = 0;

  void AddLinkedViews(const PtrView &v) {
    m_linkedViews.insert(v);
  }

  void RemoveLinkedViews(const PtrView &v) {
    m_linkedViews.erase(v);
  }

  void UpdateLinkedViews() {
    for (auto v : m_linkedViews) {
      if (!v.expired) {
        v.lock()->Draw();
      }
    }
  }
};

template <typename T>
class ModelChecker : ModelCheckerBase {
private:
  T modelCopy;
  const T* pModel;

public:
  ModelChecker(const T &model) : modelCopy(model) {
    pModel = &model;
  }

  bool ModelChanged() {
    bool changed = (modelCopy != *pModel);
    if (changed) {
      modelCopy = *pModel;
    }
    return changed;
  }

  const void * GetModelAddr() {
    return static_cast<void*>(pModel);
  }
};
