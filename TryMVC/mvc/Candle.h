#pragma once

#include "Types.h"
#include "View.h"
#include "ModelRef.h"
#include "BarPrice.h"

namespace mvc {
  class Candle : public View<Candle> {

  private:
    ModelRef<BarPrice> m_price;

    ID2D1SolidColorBrush* m_pUpBarBrush;
    ID2D1SolidColorBrush* m_pDnBarBrush;
    ID2D1SolidColorBrush* m_pBorderBrush;

    double ScreenAlign(double price) {
      int ip = static_cast<int>(price);
      return ip + 0.5;
    }

  protected:

    virtual void CreateD2DResource() {
      ID2D1HwndRenderTarget* pRndrTgt = *m_ppRenderTarget;
      HRESULT hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0x006600),  // green up bar
        &m_pUpBarBrush);

      if (!SUCCEEDED(hr)) {
        throw std::runtime_error("Failed to create the up bar brush.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0xcc0000),  // red down bar
        &m_pDnBarBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pUpBarBrush);
        throw std::runtime_error("Failed to create the background brush.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0x000000),   // gray boader
        &m_pBorderBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pUpBarBrush);
        SafeRelease(m_pDnBarBrush);
        throw std::runtime_error("Failed to create the background brush.");
      }
    }

    virtual void DestroyD2DResource() {
      SafeRelease(m_pUpBarBrush);
      SafeRelease(m_pDnBarBrush);
      SafeRelease(m_pBorderBrush);
    }

  public:

    Candle(const BarPrice &b)
      : m_price{ this, b } {
    }

    ~Candle() {
    }

    virtual void DrawSelf() {

      auto pRndrTgt = *m_ppRenderTarget;

      double entityHeight = abs(m_price->Open - m_price->Close);
      double upperHeight = m_price->High - max(m_price->Open, m_price->Close);
      double lowerHeight = min(m_price->Open, m_price->Close) - m_price->Low;

      double viewMaxPrice = 15000.0;
      double viewMinPrice = 14000.0;
      double viewHeight = 600.0;

      // 实体高点，低点屏幕坐标
      double enTop = (viewMaxPrice - max(m_price->Open, m_price->Close)) * viewHeight / (viewMaxPrice - viewMinPrice);
      double enBot = (viewMaxPrice - min(m_price->Open, m_price->Close)) * viewHeight / (viewMaxPrice - viewMinPrice);

      // 上影线高点，下影线低点屏幕坐标
      double shTop = (viewMaxPrice - m_price->High) * viewHeight / (viewMaxPrice - viewMinPrice);
      double shBot = (viewMaxPrice - m_price->Low) * viewHeight / (viewMaxPrice - viewMinPrice);

      enTop = ScreenAlign(enTop);
      enBot = ScreenAlign(enBot);
      shTop = ScreenAlign(shTop);
      shBot = ScreenAlign(shBot);

      D2D1_RECT_F textRect = D2D1::RectF(ScreenAlign(m_left), enTop, ScreenAlign(m_right), enBot);
      double mid = ScreenAlign((m_left + m_right) / 2);

      // draw entity
      if (m_price->Open > m_price->Close) {
        pRndrTgt->FillRectangle(textRect, m_pDnBarBrush);
      }
      else {
        pRndrTgt->FillRectangle(textRect, m_pUpBarBrush);
      }

      // draw border and shadow lines
      pRndrTgt->DrawRectangle(textRect, m_pBorderBrush);
      pRndrTgt->DrawLine(D2D1::Point2F(mid, shTop), D2D1::Point2F(mid, enTop), m_pBorderBrush);
      pRndrTgt->DrawLine(D2D1::Point2F(mid, shBot), D2D1::Point2F(mid, enBot), m_pBorderBrush);
    }

  };
}
