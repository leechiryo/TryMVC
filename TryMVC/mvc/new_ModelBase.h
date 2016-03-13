#pragma once

#include "new_mvc.h"
#include "new_ViewBase.h"

namespace mvc2 {
  class ModelBase {

  private:
    static map<string, SPModel> s_models;
    WPViewSet m_linkedViews;

  public:
    template<typename T>
    static shared_ptr<Model<T>> Find(string id) {
      auto it = s_models.find(id);
      if (it != s_models.end()) {
        return dynamic_pointer_cast<Model<T>>(it->second);
      }

      // can not find, return empty.
      return shared_ptr<Model<T>>{};
    }

    static void Remove(string id) {
      auto it = s_models.find(id);
      if (it != s_models.end()) {
        if (it->second) {
          auto cnt = it->second.use_count();
          it->second.reset();
          cnt = it->second.use_count();
          cnt++;
        }
        s_models.erase(it);
      }
    }

    static void UpdateViews()
    {
      for (auto m : s_models) {
        if (m.second->ModelChanged()) {
          m.second->UpdateBindedViews();
        }
      }
    }

    ModelBase(string id) {

      if (s_models.find(id) != s_models.end()) {
        throw std::runtime_error("The id exists already." + id);
      }

      s_models.insert({ id, SPModel{this} });
    }

    virtual ~ModelBase() { }


    virtual bool ModelChanged() = 0;

    void AddBindedView(const WPView &v) {
      m_linkedViews.insert(v);
    }

    void RemoveBindedView(const WPView &v) {
      m_linkedViews.erase(v);
    }

    void UpdateBindedViews() {
      for (auto v : m_linkedViews) {
        auto pv = v.lock();
        if (pv) {
          pv->Draw();
        }
        else if (v.expired()) {
          m_linkedViews.erase(v);
        }
      }
    }
  };

  map<string, SPModel> ModelBase::s_models;
}