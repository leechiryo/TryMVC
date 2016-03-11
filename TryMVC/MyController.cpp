#include "stdafx.h"
#include "mvc/App.h"
#include "mvc/Button.h"

class MyController {

public:
  static void DoSomething(shared_ptr<Button> btn, int msg) {
    auto pModel = btn->title.SafePtr();
    *pModel = "Something Done!";
  }
};
