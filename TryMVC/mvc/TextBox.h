#pragma once

#include "Types.h"
#include "View.h"
#include "ModelRef.h"

namespace mvc {
  class TextBox : public View<TextBox> {

  private:
    ID2D1SolidColorBrush* m_pBorderBrush;
    ID2D1SolidColorBrush* m_pTextBrush;
    ID2D1SolidColorBrush* m_pBackgroundBrush;
    IDWriteTextFormat* m_pTextFormat;

    HCURSOR outerCursor;
    HCURSOR ibeamCursor;

    static const int MAX_CHARS = 256;
    wchar_t m_font[MAX_CHARS + 1];
    float m_fontSize;
    DWRITE_FONT_WEIGHT m_fontWeight;
    DWRITE_FONT_STYLE m_fontStyle;
    DWRITE_FONT_STRETCH m_fontStretch;
    UINT32 m_color;

    // controller method
    static LRESULT Handle_CHAR(shared_ptr<TextBox> tbx, WPARAM wParam, LPARAM lParam) {
      TCHAR wch = static_cast<TCHAR>(wParam);
      tbx->text->append({ wch });
      return 0;
    }

  protected:
    virtual void MouseEnter(double x, double y) {
      // change mouse cursor.
      ID2D1HwndRenderTarget* pRndrTgt = *m_ppRenderTarget;
      outerCursor = (HCURSOR)GetClassLongPtr(pRndrTgt->GetHwnd(), GCL_HCURSOR);
      SetClassLongPtr(pRndrTgt->GetHwnd(), GCL_HCURSOR, (LONG)ibeamCursor);
    }

    virtual void MouseLeft(double x, double y) {
      // change mouse cursor.
      ID2D1HwndRenderTarget* pRndrTgt = *m_ppRenderTarget;
      SetClassLongPtr(pRndrTgt->GetHwnd(), GCL_HCURSOR, (LONG)outerCursor);
    }

    virtual void CreateD2DResource() {
      ID2D1HwndRenderTarget* pRndrTgt = *m_ppRenderTarget;
      HRESULT hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0xffffff),
        &m_pBackgroundBrush);

      if (!SUCCEEDED(hr)) {
        throw std::runtime_error("Failed to create the background brush.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(0x006600),
        &m_pBorderBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pBackgroundBrush);
        throw std::runtime_error("Failed to create the border brush.");
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
        SafeRelease(m_pBackgroundBrush);
        SafeRelease(m_pBorderBrush);
        throw new std::runtime_error("Failed to create the text format.");
      }

      hr = pRndrTgt->CreateSolidColorBrush(
        D2D1::ColorF(m_color),
        &m_pTextBrush);

      if (!SUCCEEDED(hr)) {
        SafeRelease(m_pBackgroundBrush);
        SafeRelease(m_pBorderBrush);
        SafeRelease(m_pTextFormat);
        throw new std::runtime_error("Failed to create the text brush.");
      }
    }

    virtual void DestroyD2DResource() {
      SafeRelease(m_pBackgroundBrush);
      SafeRelease(m_pBorderBrush);
      SafeRelease(m_pTextFormat);
      SafeRelease(m_pTextBrush);
    }

  public:
    ModelRef<wstring> text;

    TextBox(wstring ttl) : text{ this, ttl } {
      m_color = 0x333333;
      m_fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
      m_fontStyle = DWRITE_FONT_STYLE_NORMAL;
      m_fontStretch = DWRITE_FONT_STRETCH_NORMAL;
      wcscpy_s(m_font, MAX_CHARS + 1, L"Source Code Pro");
      m_fontSize = 16.0;

      AddEventHandler(WM_CHAR, Handle_CHAR);

      outerCursor = 0;
      ibeamCursor = LoadCursor(nullptr, IDC_IBEAM);
    }

    ~TextBox() {
    }

    virtual void DrawSelf() {
      D2D1_RECT_F textRect = RectD(m_left, m_top, m_right, m_bottom);
      (*m_ppRenderTarget)->FillRectangle(textRect, m_pBackgroundBrush);
      (*m_ppRenderTarget)->DrawRectangle(textRect, m_pBorderBrush);

      (*m_ppRenderTarget)->DrawText(
        text->c_str(),
        text->length(),
        m_pTextFormat,
        textRect,
        m_pTextBrush);
    }

  };
}
