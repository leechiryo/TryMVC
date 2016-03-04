#pragma once

#include <map>
#include <string>
#include <memory>
#include "App.h"

namespace mvc {
  using namespace std;

  template<typename T, typename ... Args>
  T& v(string id, Args ... args){
    auto wptr = App::CreateView<T>(id, args...);
    auto sptr = wptr.lock();
    return (*sptr);
  }

  template<typename T, typename ... Args>
  T& m(string id, Args ... args){
    auto wptr = App::CreateModel<Model<T>>(id, args...);
    auto sptr = wptr.lock();
    return sptr->get_ref();
  }

  template<typename T>
  T& getv(string id) {
    return App::GetView<T>(id);
  }

  template<typename T>
  T& getm(string id) {
    return App::GetModel<T>(id);
  }
}