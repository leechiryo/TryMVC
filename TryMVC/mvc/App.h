#pragma once

#include <map>
#include <set>
#include <string>
#include <memory>
#include "ViewBase.h"
#include "Model.h"

using namespace std;
class App {
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
  static weak_ptr<T> CreateView(string, Args ... args);

  template <typename T, typename ... Args>
  static weak_ptr<T> CreateModel(string, Args ... args);

  static void RemoveView(string);
  static void RemoveModel(string);

};