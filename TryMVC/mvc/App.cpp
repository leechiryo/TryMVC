#include "stdafx.h"
#include "App.h"
#include "View.h"

map<string, App::PtrView> App::s_views;
map<string, App::PtrModel> App::s_models;

void App::Start()
{
}

void App::UpdateViews()
{
  for (auto m : s_models) {
    if (m.second->ModelChanged()) {
      m.second->UpdateBindedViews();
    }
  }
}

void App::RemoveView(string id)
{
  auto it = s_views.find(id);
  if (it != s_views.end()) {
    if (it->second) {
      it->second.reset();
    }
    s_views.erase(it);
  }
}

void App::RemoveModel(string id)
{
  auto it = s_models.find(id);
  if (it != s_models.end()) {
    if (it->second) {
      int cnt = it->second.use_count();
      it->second.reset();
    }
    s_models.erase(it);
  }
}

