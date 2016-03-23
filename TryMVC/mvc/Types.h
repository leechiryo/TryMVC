#pragma once

#include <memory>
#include <set>

namespace mvc {

  class ViewBase;
  class ModelBase;

  template<typename T>
  class View;

  template<typename T>
  class Model;

  template<typename T>
  class ModelRef;

  template<typename T>
  class ModelSafePtr;

  template <typename T>
  struct WeakPtrComparer {
    bool operator()(const std::weak_ptr<T> &x, const std::weak_ptr<T> &y) const {
      auto px = x.lock();
      auto py = y.lock();
      return px < py;
    }
  };

  typedef std::shared_ptr<ViewBase> SPView;
  typedef std::shared_ptr<ModelBase> SPModel;
  typedef std::weak_ptr<ViewBase> WPView;
  typedef std::weak_ptr<ModelBase> WPModel;

  typedef std::set<WPView, WeakPtrComparer<ViewBase>> WPViewSet;


  template<class Interface>
  inline void SafeRelease(Interface *pT) {
    if (pT != nullptr) {
      pT->Release();
      pT = nullptr;
    }
  }

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
  EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif
}
