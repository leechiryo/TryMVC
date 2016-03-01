#pragma once

#include <map>
#include <set>
#include <string>
#include <memory>
#include "ViewBase.h"
#include "Model.h"

using namespace std;
class App {
public:
  typedef shared_ptr<ViewBase> PtrView;
  typedef shared_ptr<ModelBase> PtrModel;

private:
  static map<string, PtrView> s_views;
  static map<string, PtrModel> s_models;

public:

  static ViewBase* mainWnd;

  static void Start();
  static void UpdateViews();

  template <typename T, typename ... Args>
  static weak_ptr<T> CreateView(string id, Args ... args) {
    if (s_views.find(id) != s_views.end()) {
      throw std::runtime_error("The id exists already." + id);
    }

    shared_ptr<T> ptr = make_shared<T>(args...);
    s_views.insert({ id, ptr });

    // save the weak pointer of T to the object itself.
    View<T> *pvb = ptr.get();
    pvb->m_weakthis = ptr;

    return pvb->m_weakthis;
  }

  template <typename T, typename ... Args>
  static weak_ptr<T> CreateModel(string id, Args ... args) {
    if (s_models.find(id) != s_models.end()) {
      throw std::runtime_error("The id exists already." + id);
    }

    auto ptr = make_shared<T>(args...);
    s_models.insert({ id, ptr });

    return weak_ptr<T>(ptr);
  }

  static void RemoveView(string);
  static void RemoveModel(string);

};