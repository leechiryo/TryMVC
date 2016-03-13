#pragma once

#include "new_View.h"
#include "new_ModelRef.h"

namespace mvc2 {

  class Button : public View<Button>
  {
  public:
    ModelRef<string> title;
    ModelRef<char> ch;

    Button(string id, string ttl) : View{ id }, title{ this, ttl }, ch{ this } {
    }

    void DrawSelf() {
      MessageBoxA(0, "Button is drawed", "Test", 0);
    }
  };
}