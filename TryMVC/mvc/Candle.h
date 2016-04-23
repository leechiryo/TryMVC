#pragma once

#include "Types.h"
#include "View.h"
#include "ModelRef.h"
#include "BarPrice.h"

namespace mvc {
  class Candle : public View<Candle> {

  private:
    ModelRef<BarPrice> m_price;
    ModelRef<double> m_width;

    ID2D1SolidColorBrush* m_pUpBarBrush;
    ID2D1SolidColorBrush* m_pDnBarBrush;
    ID2D1SolidColorBrush* m_pBorderBrush;

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
        D2D1::ColorF(0x660000),  // red down bar
        &m_pDnBarBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pUpBarBrush);
        throw std::runtime_error("Failed to create the background brush.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0x202020),   // gray boader
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

    Candle(const BarPrice &b, const double &width)
      : m_price{ this, b }, m_width{ this,width } {
    }

    ~Candle() {
    }

    virtual void DrawSelf() {

      auto pRndrTgt = *m_ppRenderTarget;

      double entityHeight = abs(m_price->Open - m_price->Close);
      double upperHeight = m_price->High - max(m_price->Open, m_price->Close);
      double lowerHeight = min(m_price->Open, m_price->Close) - m_price->Low;

      D2D1_RECT_F textRect = D2D1::RectF(m_left, m_top, m_right, m_bottom);

      if (m_price->Open > m_price->Close) {
        pRndrTgt->FillRectangle(textRect, m_pDnBarBrush);
      }
      else {
        pRndrTgt->FillRectangle(textRect, m_pUpBarBrush);
      }
    }


  };
}
