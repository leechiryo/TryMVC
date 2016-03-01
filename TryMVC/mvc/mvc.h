#pragma once

#include <map>
#include <string>
#include <memory>
#include "App.h"

namespace mvc {
  using namespace std;

  template<typename T, typename ... Args>
  weak_ptr<T> v(string id, Args ... args){
    return App::CreateView(id, args);
  }

  template<typename T, typename ... Args>
  weak_ptr<Model<T>> m(string id, Args ... args){
    return App::CreateModel<Model<T>>(id, args...);
  }
}