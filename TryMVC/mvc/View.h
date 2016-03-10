#pragma once

#include <map>
#include <string>
#include <memory>
#include "Model.h"
#include "ViewBase.h"
#include "App.h"
#include "WeakPtrComparer.h"

using namespace std;

//template<typename M>
//class ModelRef;

template<typename DerivedType>
class View : public ViewBase {
  typedef void(*ControllerMethod)(shared_ptr<DerivedType>, int param);
  friend class App;

private:
  map<int, ControllerMethod> m_eventHandlers;

public:

  View() : ViewBase(){
  }

  View(const list<PtrView> & views) : ViewBase(views){
  }

  void AddEventHandler(int msg, ControllerMethod method) {
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()) {
      m_eventHandlers.insert({ msg, method });
    }
  }

  void FireEvent(int msg){
    if (m_eventHandlers.find(msg) != m_eventHandlers.end()){

      auto sharedthis = m_wpThis.lock();
      if (sharedthis){
        m_eventHandlers[msg](sharedthis, msg);
      }

      App::UpdateViews();
    }
  }

  //template<typename M>
  //void Bind(const ModelRef<M> &mref, string modelId) {
  //  mref.Bind(modelId);
  //  auto spModel = mref.m_wpModel.lock();
  //  if (spModel){
  //    sp->AddBindedView(m_wpThis);
  //  }
  //}

  //template<typename M, typename F>
  //void Bind(const ModelRef<M> &mref, string modelId, F M::*pField) {
  //  mref.Bind(modelId, pField);
  //  auto spModel = mref.m_wpModel.lock();
  //  if (spModel){
  //    sp->AddBindedView(m_wpThis);
  //  }
  //}

  //template<typename FieldType>
  //void bind(weak_ptr<Model<FieldType>> &pfield, weak_ptr<Model<FieldType>> &model) {
  //  pfield = model;
  //  model->AddBindedViews(m_wpThis);
  //}
};
