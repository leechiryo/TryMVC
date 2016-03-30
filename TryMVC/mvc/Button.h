#pragma once

#include "Types.h"
#include "View.h"
#include "ModelRef.h"


namespace mvc {
  class Button : public View<Button>
  {

  private:

    ID2D1SolidColorBrush* m_pNormalBackgroundBrush;
    ID2D1SolidColorBrush* m_pHoverBackgroundBrush;
    ID2D1SolidColorBrush* m_pClickBackgroundBrush;
    ID2D1SolidColorBrush* m_pBackgroundBrush;

    static const int MAX_CHARS = 256;
    wchar_t m_font[MAX_CHARS + 1];
    wchar_t m_locale[MAX_CHARS + 1];

    DWRITE_FONT_WEIGHT m_fontWeight;
    DWRITE_FONT_STYLE m_fontStyle;
    DWRITE_FONT_STRETCH m_fontStretch;
    float m_fontSize;

    DWRITE_TEXT_ALIGNMENT m_alignment;

    UINT32 m_color;

    ID2D1SolidColorBrush* m_pBrush;
    IDWriteTextFormat* m_pTextFormat;

  protected:
    virtual void CreateD2DResource() {
      ID2D1HwndRenderTarget* pRndrTgt = *m_ppRenderTarget;
      HRESULT hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0xcccccc),
        &m_pNormalBackgroundBrush);

      if (!SUCCEEDED(hr)) {
        throw std::runtime_error("Failed to create the background brush.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0x999999),
        &m_pHoverBackgroundBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pNormalBackgroundBrush);
        throw std::runtime_error("Failed to create the background brush.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0x666666),
        &m_pClickBackgroundBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pNormalBackgroundBrush);
        SafeRelease(m_pHoverBackgroundBrush);
        throw std::runtime_error("Failed to create the background brush.");
      }

      hr = App::s_pDWriteFactory->CreateTextFormat(
        m_font,
        NULL,
        m_fontWeight,
        m_fontStyle,
        m_fontStretch,
        m_fontSize,
        L"ja-JP",
        &m_pTextFormat);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pNormalBackgroundBrush);
        SafeRelease(m_pHoverBackgroundBrush);
        SafeRelease(m_pClickBackgroundBrush);
        throw new std::runtime_error("Failed to create the text format.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(m_color),
        &m_pBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pNormalBackgroundBrush);
        SafeRelease(m_pHoverBackgroundBrush);
        SafeRelease(m_pClickBackgroundBrush);
        SafeRelease(m_pTextFormat);
        throw new std::runtime_error("Failed to create the brush.");
      }
    }

    virtual void DestroyD2DResource() {
      SafeRelease(m_pNormalBackgroundBrush);
      SafeRelease(m_pHoverBackgroundBrush);
      SafeRelease(m_pClickBackgroundBrush);
      SafeRelease(m_pTextFormat);
      SafeRelease(m_pBrush);
    }


  public:
    ModelRef<wstring> title;

    Button(wstring ttl) : title{ this, ttl } {
      // 在Constructor中调用虚函数。本来，如此调用虚函数并不会激发
      // 对象的多态调用（即调用派生对象的虚函数）。但在此处每个类都负责
      // 自身所需的D2D资源的创建，所以并不需要调用派生对象的虚函数。
      // 这么做并没有什么问题。
      CreateD2DResource();
    }

    ~Button() {
      DestroyD2DResource();
    }

    virtual void DrawSelf() {
      D2D1_RECT_F textRect = D2D1::RectF(m_left, m_top, m_right, m_bottom);

      (*m_ppRenderTarget)->DrawText(
        title->c_str(),
        title->length(),
        m_pTextFormat,
        textRect,
        m_pBrush);
    }
  };
}
