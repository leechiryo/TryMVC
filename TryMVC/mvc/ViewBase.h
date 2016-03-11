#pragma once

#include <set>
#include <memory>

#include "WeakPtrComparer.h"
#include "ModelRef.h"

using namespace std;

class ViewBase {

private:
 
  set<PtrView, WeakPtrComparer<ViewBase>> m_subViews;

protected:
  weak_ptr<ViewBase> m_wpThis;

  template<typename T, typename ... Args>
  ModelRef<T> make_ref(Args ... args) {
    ModelRef<T> ref(args...);
    ref.m_wpMyView = &m_wpThis;
    return ref;
  }

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