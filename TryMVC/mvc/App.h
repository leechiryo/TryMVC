#pragma once

#include <map>
#include <set>
#include <string>
#include <memory>
#include "ViewBase.h"
#include "Model.h"
#include "ConstructorProxy.h"

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

  template <typename T>
  static shared_ptr<T> CreateView(string id, const ConstructorProxy<T> &cp) {
    if (s_views.find(id) != s_views.end()) {
      throw std::runtime_error("The id exists already." + id);
    }

    shared_ptr<T> ptr = cp.GetSP();
    s_views.insert({ id, ptr });

    // save the weak pointer of T to the object itself.
    ptr->m_wpThis = ptr;

    return ptr;
  }

  template <typename T>
  static ModelSafePtr<T> CreateModel(string id, const ConstructorProxy<Model<T>> &cp) {
    if (s_models.find(id) != s_models.end()) {
      throw std::runtime_error("The id exists already." + id);
    }

    shared_ptr<Model<T>> ptr = cp.GetSP();
    s_models.insert({ id, ptr });

    // save the weak pointer of T to the object itself.
    ptr->m_wpThis = ptr;

    return ptr->get_safeptr();
  }

  static void RemoveView(string);
  static void RemoveModel(string);

  template <typename T>
  static shared_ptr<T> GetView(string id) {
    if (s_views.find(id) == s_views.end()) {
      throw std::runtime_error("Can not find the view.");
    }
    auto ptr = dynamic_pointer_cast<T, ViewBase>(s_views[id]);
    return ptr;
  }

  template <typename T>
  static ModelSafePtr<T> GetModel(string id) {
    if (s_models.find(id) == s_models.end()) {
      throw std::runtime_error("Can not find the model.");
    }
    auto ptrm = dynamic_pointer_cast<Model<T>, ModelBase>(s_models[id]);
    return ptrm->get_safeptr();
  }
};