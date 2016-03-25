#pragma once

#include "mvc/App.h"
#include "mvc/Button.h"

using namespace mvc;

class MyController {

public:
  static LRESULT DoSomething(shared_ptr<Button> btn, WPARAM msg, LPARAM lp) {
    btn->title = "Something Done!";
    auto size = btn->title->length();
  }

  static LRESULT DoSomething2(shared_ptr<Button> btn, WPARAM msg, LPARAM lp) {
    btn->title = "Something2 Done!";
  }
};
