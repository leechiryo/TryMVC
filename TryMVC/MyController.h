#pragma once

#include "mvc/App.h"
#include "mvc/Button.h"

using namespace mvc;

class MyController {

public:
  static void DoSomething(shared_ptr<Button> btn, int msg) {
    btn->title = "Something Done!";
    auto size = btn->title->length();
  }

  static void DoSomething2(shared_ptr<Button> btn, int msg) {
    btn->title = "Something2 Done!";
  }
};
