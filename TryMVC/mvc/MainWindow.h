#pragma once

#include "View.h"

#include <vector>

class MainWindow : public View<MainWindow>
{
public:
  MainWindow(){}
  MainWindow(const vector<PtrView> & subViews) { }

  void DrawSelf(){
    MessageBoxA(0, "MainWindow is drawed", "Test", 0);
  }
};