#pragma once

#include <memory>

using namespace std;

class ViewBase;
typedef weak_ptr<ViewBase> PtrView;
struct WeakPtrComparer{
  bool operator()(const PtrView &x, const PtrView &y){
    auto px = x.lock();
    auto py = y.lock();
    return px < py;
  }
};
