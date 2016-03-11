#pragma once

#include "View.h"
#include "ModelRef.h"

class Button : public View<Button>
{
public:
  ModelRef<string> title;

  Button(string ttl) : title{ this, ttl }{
  }

  void DrawSelf(){
  }
};