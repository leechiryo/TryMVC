#pragma once

#include <map>
#include <string>
#include <memory>
#include "App.h"

namespace mvc {
  using namespace std;

  template<typename T>
  shared_ptr<T> v(string id, const ConstructorProxy<T> &cp){
    return App::CreateView<T>(id, cp);
  }

  template<typename T>
  ModelSafePtr<T> m(string id, const ConstructorProxy<Model<T>> &cp){
    return App::CreateModel<T>(id, cp);
  }

  template<typename T>
  shared_ptr<T> getv(string id) {
    return App::GetView<T>(id);
  }

  template<typename T>
  ModelSafePtr<T> getm(string id) {
    return App::GetModel<T>(id);
  }
}