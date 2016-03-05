#pragma once

#include <memory>

using namespace std;

class ViewBase;
typedef weak_ptr<ViewBase> PtrView;

template <typename T = ViewBase>
struct WeakPtrComparer{
  bool operator()(const weak_ptr<T> &x, const weak_ptr<T> &y) const {
    auto px = x.lock();
    auto py = y.lock();
    return px < py;
  }
};

