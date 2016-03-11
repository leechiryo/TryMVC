#pragma once

#include <set>
#include <memory>

#include "WeakPtrComparer.h"
#include "ModelRef.h"

using namespace std;

class ViewBase {

  template<typename T>
  friend class ModelRef;
  friend class App;

private:
 
  set<PtrView, WeakPtrComparer<ViewBase>> m_subViews;
  weak_ptr<ViewBase> m_wpThis;

public:

  ViewBase(){
  }

  ViewBase(const set<PtrView, WeakPtrComparer<ViewBase>> & views) : m_subViews(views){
  }

  double left, top, width, height;

  virtual void DrawSelf() = 0;

  void Draw() {
    DrawSelf();

    for (auto it = m_subViews.begin(); it != m_subViews.end(); ++it) {
      const auto &v = *it;
      auto ptr = v.lock();

      if (ptr){
        ptr->Draw();
      }
      else if (v.expired()){
        m_subViews.erase(it);
      }
    }
  }

  void AddSubView(const PtrView &v){
    m_subViews.insert(v);
  }

  void RemoveSubView(const PtrView &v){
    m_subViews.erase(v);
  }

};