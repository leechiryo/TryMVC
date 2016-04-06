﻿#pragma once

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

    // controller method
    static LRESULT Handle_LBUTTONDOWN(shared_ptr<Button> btn, WPARAM wParam, LPARAM lParam) {
      btn->m_pBackgroundBrush = btn->m_pClickBackgroundBrush;

      RECT rect;
      btn->GetPixelRect(rect);

      ID2D1HwndRenderTarget* pRndrTgt = *(btn->m_ppRenderTarget);
      InvalidateRect(pRndrTgt->GetHwnd(), &rect, 0);
      return 0;
    }

    static LRESULT Handle_LBUTTONUP(shared_ptr<Button> btn, WPARAM wParam, LPARAM lParam) {
      btn->m_pBackgroundBrush = btn->m_pHoverBackgroundBrush;

      RECT rect;
      btn->GetPixelRect(rect);

      ID2D1HwndRenderTarget* pRndrTgt = *(btn->m_ppRenderTarget);
      InvalidateRect(pRndrTgt->GetHwnd(), &rect, 0);
      return 0;
    }

  protected:
    virtual void MouseEnter(double x, double y) {
      m_pBackgroundBrush = m_pHoverBackgroundBrush;

      RECT rect;
      GetPixelRect(rect);

      ID2D1HwndRenderTarget* pRndrTgt = *m_ppRenderTarget;
      InvalidateRect(pRndrTgt->GetHwnd(), &rect, 0);
    }

    virtual void MouseLeft(double x, double y) {
      m_pBackgroundBrush = m_pNormalBackgroundBrush;

      RECT rect;
      GetPixelRect(rect);

      ID2D1HwndRenderTarget* pRndrTgt = *m_ppRenderTarget;
      InvalidateRect(pRndrTgt->GetHwnd(), &rect, 0);
    }

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

      m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
      m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

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

      m_pBackgroundBrush = m_pNormalBackgroundBrush;
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
      m_color = 0x333333;
      m_fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
      m_fontStyle = DWRITE_FONT_STYLE_NORMAL;
      m_fontStretch = DWRITE_FONT_STRETCH_NORMAL;
      wcscpy_s(m_font, MAX_CHARS + 1, L"Arial");
      m_fontSize = 16.0;

      AddEventHandler(WM_LBUTTONDOWN, Handle_LBUTTONDOWN);
      AddEventHandler(WM_LBUTTONUP, Handle_LBUTTONUP);
    }

    ~Button() {
    }

    virtual void DrawSelf() {
      D2D1_RECT_F textRect = D2D1::RectF(m_left, m_top, m_right, m_bottom);
      (*m_ppRenderTarget)->FillRectangle(textRect, m_pBackgroundBrush);

      (*m_ppRenderTarget)->DrawText(
        title->c_str(),
        title->length(),
        m_pTextFormat,
        textRect,
        m_pBrush);
    }
  };
}
