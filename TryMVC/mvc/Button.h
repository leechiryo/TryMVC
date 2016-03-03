#pragma once

#include "View.h"

class Button : public View<Button>
{
private:
  string m_title;

public:
  Button(string ttl) : m_title(ttl){
  }

  void DrawSelf(){
  }

  string GetTitle() {
    return m_title;
  }

  void SetTitle(string newval) {
    m_title = newval;
  }
};