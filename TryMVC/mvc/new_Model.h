#pragma once

#include "new_mvc.h"
#include "new_ModelBase.h"

namespace mvc2 {

  template<typename T, typename... Args>
  shared_ptr<T> m(Args... args);

  template <typename T>
  class Model : public ModelBase {

    template<typename M>
    friend class ModelRef;

    template<typename... Args>
    friend shared_ptr<T> m<T>(Args... args);


  private:
    T m_modelCopy;
    T m_model;
    weak_ptr<Model<T>> m_wpThis;

    T* ptr() {
      return &m_model;
    }

  protected:
    template<typename... Args>
    Model(string id, Args... args) : ModelBase(id), m_model(args...) {
      m_modelCopy = m_model;
      m_wpThis = Find(id);
    }

  public:
    bool ModelChanged() {
      bool changed = (m_modelCopy != m_model);
      if (changed) {
        m_modelCopy = m_model;
      }
      return changed;
    }
  };

  template<typename T, typename... Args>
  shared_ptr<T> m(Args... args){
    auto pm = new Model<T>(args...);
    return pm->m_wpThis.lock();
  }
}