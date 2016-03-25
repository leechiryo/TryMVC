#pragma once

#include "View.h"
#include "ModelRef.h"

namespace mvc {
  class Button : public View<Button>
  {
  protected:
    virtual void CreateD2DResource() {

    }
    virtual void DestroyD2DResource() {

    }
  public:
    ModelRef<string> title;

    Button(string ttl) : title{ this, ttl } {
    }

    void DrawSelf() {
      MessageBoxA(0, "Button is drawed", "Test", 0);
    }
  };
}
