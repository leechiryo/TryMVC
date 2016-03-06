// TryMVC.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "TryMVC.h"

#include "mvc\Button.h"
#include "mvc\mvc.h"
#include "mvc\ModelRef.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

struct C {
  int a;
  int b;
  char c;
  double d;
  ~C() {

    stringstream ss;
    ss << "C object is deleted. [" << this << "]"<< endl;

    MessageBoxA(0, ss.str().c_str(), "test", 0);
  }
};

bool operator==(const C &x, const C &y) {
  return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}

bool operator!=(const C &x, const C &y) {
  return !(x == y);
}

template<typename T>
bool objsptr(T * obj, void *ptr) {
  char * objaddr = reinterpret_cast<char*>(obj);
  char * ptraddr = reinterpret_cast<char*>(ptr);
  return (ptraddr > objaddr) && ((ptraddr - objaddr) < sizeof(T));
}

// test the model delete.
void test_delete() {
  mvc::m<C>("delete");  // create a new model.
  App::RemoveModel("delete");  // ** model deleted here.
}

void stubfunc(ModelAccessor<C> ptr) {
  
}

// test the model delete.
void test_delete2() {
  mvc::m<C>("delete2");                   // create a new model.
  stubfunc(App::GetModel<C>("delete2"));  // get a shared pointer of the model.

  ModelRef<double> refd{ 0.0 };           // define a new model ref
  refd.Bind<C>("delete2", &C::d);         // bind the ref to the new model.
  auto &accd = refd.Access();             // get accessor to the new model from the model ref.

  accd = 2.0;                             // access the model from the accessor.

  App::RemoveModel("delete2");            // release the shared pointer.

  accd = 5.0;                             // access the model from the accessor.
} // ** model deleted here.

// test the model delete.
void test_delete3() {
  mvc::m<C>("delete3");
  stubfunc(App::GetModel<C>("delete3"));

  ModelRef<double> refd{ 0.0 }; // define a new model ref
  refd.Bind<C>("delete3", &C::d);

  refd.Access() = 2.0;

  App::RemoveModel("delete3"); // ** model deleted here.

  refd.Access() = 5.0;  // will change a value of inner fallback.
                        // the destroied model won't be accessed.
}

void do_test() {
  // test model and modelref and modelaccessor
  auto m9 = mvc::m<C>("me");   // create new model.
  m9->d = 2.4;                 // update the model value.
  ModelRef<double> r1(0.0);    // create a model reference.
  r1.Bind<C>("me", &C::d);     // bind the model reference to "a field of a model".
  auto m9ac = r1.Access();     // get the access to binding field.
  m9ac = 1.2;                  // set the binding field value.

  double dval = m9ac;

  ModelRef<C> cref;            // create a model reference.
  cref.Bind("me");             // bind the model reference to "model itself".
  cref.Access()->a = 10;       // access the model.

  //test delete
  test_delete();
  test_delete2();
  test_delete3();

  // TODO: ここにコードを挿入してください。
  auto m1 = mvc::m<string>("id", "value");  // create a new model.
  MessageBoxA(0, m1->c_str(), "test", 0);     // the output should be "value".

  m1 = "new value";                          // update the model value.
  MessageBoxA(0, m1->c_str(), "test", 0);     // the output should be "new value".

  auto m2 = mvc::getm<string>("id");        // get the model from its id.
  MessageBoxA(0, m2->c_str(), "test", 0);     // the output should be "new value".

  auto btn = mvc::v<Button>("btn1", "My Button"); // create a new view.
  btn->title.Bind("id");                    // bind the view's field with a model.
  MessageBoxA(0, btn->title.Access()->c_str(), "test", 0);  // the output should be "new value" (the model's value).

  m2 = "button value";                                     // update the model.
  MessageBoxA(0, btn->title.Access()->c_str(), "test", 0);  // the output of the view should be "button value" (updated with the model).
}

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // do some test about the model and view.
  do_test();


  // グローバル文字列を初期化しています。
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_TRYMVC, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // アプリケーションの初期化を実行します:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRYMVC));

  MSG msg;

  // メイン メッセージ ループ:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRYMVC));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TRYMVC);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

  HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd)
  {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // 選択されたメニューの解析:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: HDC を使用する描画コードをここに追加してください...
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}
