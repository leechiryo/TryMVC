#pragma once

#include "new_mvc.h"
#include "new_ModelBase.h"

namespace mvc2 {

  template <typename T>
  class Model : public ModelBase {

    template<typename M>
    friend class ModelRef;

  private:
    T m_modelCopy;
    T m_model;
    weak_ptr<Model<T>> m_wpThis;

    T* ptr() {
      return &m_model;
    }

  public:
    template<typename... Args>
    Model(string id, Args... args) : ModelBase(id), m_model(args...) {
      m_modelCopy = m_model;
    }

    bool ModelChanged() {
      bool changed = (m_modelCopy != m_model);
      if (changed) {
        m_modelCopy = m_model;
      }
      return changed;
    }
  };
}