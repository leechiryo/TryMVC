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

template <typename T, typename ... Args>
static weak_ptr<T> App::CreateView(string, Args ... args)
{
  if (s_views.find(id) != s_views.end()){
    throw std::runtime_error("The id exists already." + id);
  }

  // save the weak pointer of T to the object itself.
  shared_ptr<T> ptr = make_shared<T>(args...);
  s_views.insert({ id, ptr });
  
  View<T> *pvb = ptr.get();
  pvb->m_weakthis = ptr;
  
  return pvb->m_weakthis;
}

template <typename T, typename ... Args>
static weak_ptr<T> App::CreateModel(string, Args ... args)
{
  if (s_models.find(id) != s_models.end()){
    throw std::runtime_error("The id exists already." + id);
  }

  auto ptr = make_shared<T>(args...);
  s_models.insert({ id, ptr });

  return weak_ptr<T>(ptr);
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
      it->second.reset();
    }
    s_models.erase(it);
  }
}

