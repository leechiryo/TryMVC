#include "..\stdafx.h"
#include "App.h"

set<ModelCheckerBase*> App::s_modelCheckers;
map<string, ViewBase*> App::s_views;
map<string, void*> App::s_models;

void App::Start()
{
}

void App::UpdateViews()
{
  for (auto mc : s_modelCheckers) {
    if (mc->ModelChanged()) {
      mc->UpdateLinkedViews();
    }
  }
}

void App::AddView(string id, ViewBase * v)
{
  s_views[id] = v;
}

void App::AddModel(string id, void * m)
{
  s_models[id] = m;
}

void App::AddModelChecker(ModelCheckerBase *mc)
{
  s_modelCheckers.insert(mc);
}

void App::RemoveView(string id)
{
  if (s_views.find(id) != s_views.end()) {
    if (s_views[id] != nullptr) {
      delete s_views[id];
      s_views[id] = nullptr;
    }
    s_views.erase(id);
  }
}

void App::RemoveModel(string id)
{
  if (s_models.find(id) != s_models.end()) {
    if (s_models[id] != nullptr) {
      delete s_models[id];
      s_models[id] = nullptr;
    }
    s_models.erase(id);
  }
}

void App::RemoveModelChecker(ModelCheckerBase *mc)
{

}