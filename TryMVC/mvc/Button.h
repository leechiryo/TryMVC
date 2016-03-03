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
};