#pragma once

#include <map>
#include <set>
#include <string>
#include <memory>
#include "ViewBase.h"
#include "ModelChecker.h"

using namespace std;
class App {
  typedef ViewBase *PtrView;
  typedef void *PtrModel;
  typedef ModelCheckerBase *PtrModelChecker;

private:
  static map<string, PtrView> s_views;
  static map<string, PtrModel> s_models;
  static set<PtrModelChecker> s_modelCheckers;

public:

  static ViewBase* mainWnd;

  static void Start();
  static void UpdateViews();

  static void AddView(string, ViewBase*);
  static void AddModel(string, void*);
  static void AddModelChecker(ModelCheckerBase*);

  static void RemoveView(string);
  static void RemoveModel(string);
  static void RemoveModelChecker(ModelCheckerBase*);

};