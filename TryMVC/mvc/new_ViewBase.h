#pragma once

#include <map>
#include <string>
#include "new_mvc.h"

using namespace std;

namespace mvc2 {
  class ViewBase {

    template<typename T>
    friend class ModelRef;

  private:
    static map<string, SPView> s_views;
    WPViewSet m_subViews;

  protected:
    WPView m_wpThis;

  public:

    template <typename T = ViewBase>
    static shared_ptr<T> Find(string id) {
      auto it = s_views.find(id);
      if (it != s_views.end()) {
        return dynamic_pointer_cast<T>(it->second);
      }

      // can not find, return empty.
      return shared_ptr<T>{};
    }

    static void Remove(string id) {
      auto it = s_views.find(id);
      if (it != s_views.end()) {
        if (it->second) {
          it->second.reset();
        }
        s_views.erase(it);
      }
    }

    ViewBase(string id) {

      if (s_views.find(id) != s_views.end()) {
        throw std::runtime_error("The id exists already." + id);
      }

      auto spThis = SPView{ this };
      m_wpThis = spThis;
      s_views.insert({ id, spThis });

    }

    ViewBase(string id, const WPViewSet & views) : m_subViews(views) {

      if (s_views.find(id) != s_views.end()) {
        throw std::runtime_error("The id exists already." + id);
      }

      auto spThis = SPView{ this };
      m_wpThis = spThis;
      s_views.insert({ id, spThis });

    }

    virtual ~ViewBase() {}

    double left, top, width, height;

    virtual void DrawSelf() = 0;

    void Draw() {
      DrawSelf();

      for (auto it = m_subViews.begin(); it != m_subViews.end(); ++it) {
        const auto &v = *it;
        auto ptr = v.lock();

        if (ptr) {
          ptr->Draw();
        }
        else if (v.expired()) {
          m_subViews.erase(it);
        }
      }
    }

    void AddSubView(const WPView &v) {
      m_subViews.insert(v);
    }

    void RemoveSubView(const WPView &v) {
      m_subViews.erase(v);
    }

  };

  map<string, SPView> ViewBase::s_views;
}