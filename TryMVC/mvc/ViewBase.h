#pragma once

#include <list>

using namespace std;

class ViewBase {

private:
 
  list<ViewBase*> m_subViews;

public:

  double left, top, width, height;

  virtual void DrawSelf() = 0;

  void Draw() {
    DrawSelf();

    for (auto v : m_subViews) {
      v->Draw();
    }
  }

};