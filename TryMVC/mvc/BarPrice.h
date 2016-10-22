#pragma once

struct BarPrice {
  int High;
  int Low;
  int Open;
  int Close;

  BarPrice(int o, int h, int l, int c) {
    High = h;
    Low = l;
    Open = o;
    Close = c;
  }
};