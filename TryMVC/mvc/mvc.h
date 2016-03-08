#pragma once

#include <map>
#include <string>
#include <memory>
#include "App.h"

namespace mvc {
  using namespace std;

  template<typename T, typename ... Args>
  shared_ptr<T> v(string id, Args ... args){
    return App::CreateView<T>(id, args...);
  }

  template<typename T, typename ... Args>
  ModelSafePtr<T> m(string id, Args ... args){
    return App::CreateModel<T>(id, args...);
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