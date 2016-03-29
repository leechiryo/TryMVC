// TryMVC.cpp : アプリケーションのエントリ ポイントを定義します。
//


#include "stdafx.h"
#include "TryMVC.h"

#include "mvc\Button.h"
#include "mvc\Window.h"
#include "mvc\mvc.h"
#include "mvc\ModelRef.h"
#include "mvc\ConstructorProxy.h"

#include "MyController.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

using namespace mvc;

struct C {
  int a;
  int b;
  char c;
  double d;
 
  ~C() {

    stringstream ss;
    ss << "C object is deleted. [" << this << "]" << endl;

    MessageBoxA(0, ss.str().c_str(), "test", 0);
  }
};

bool operator==(const C &x, const C &y) {
  return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}

bool operator!=(const C &x, const C &y) {
  return !(x == y);
}

void funcusec(const C& c){
    stringstream ss;
    ss << "C'c value is [" << c.c << "]" << endl;

    MessageBoxA(0, ss.str().c_str(), "test", 0);
}

template<typename T>
bool objsptr(T * obj, void *ptr) {
  char * objaddr = reinterpret_cast<char*>(obj);
  char * ptraddr = reinterpret_cast<char*>(ptr);
  return (ptraddr > objaddr) && ((ptraddr - objaddr) < sizeof(T));
}

// test the model delete.
void test_delete() {
  m<C>("delete", {});     // create a new model.
  App::RemoveModel("delete");  // ** model deleted here.
}

void stubfunc(ModelSafePtr<C> ptr) { 
}

// test the model delete.
void test_delete2() {
  m<C>("delete2", {});                    // create a new model.
  stubfunc(App::GetModel<C>("delete2"));  // get a shared pointer of the model.

  ModelRef<double> refd{ nullptr, 0.0 };  // define a new model ref
  refd.Bind("delete2", &C::d);            // bind the ref to the new model.
  auto spModel = refd.SafePtr();          // get accessor to the new model from the model ref.

  *spModel = 2.0;                         // access the model from the accessor.

  App::RemoveModel("delete2");            // release the shared pointer.

  *spModel = 5.0;                         // access the model from the accessor.
} // ** model deleted here.

// test the model delete.
void test_delete3() {
  m<C>("delete3", {});
  stubfunc(App::GetModel<C>("delete3"));

  ModelRef<double> refd{ nullptr, 0.0 };  // define a new model ref
  refd.Bind("delete3", &C::d);

  refd = 2.0;

  App::RemoveModel("delete3");            // ** model deleted here.

  refd = 5.0;                  // will change a value of inner fallback.
  // the destroied model won't be accessed.
}

void do_test() {
  // test model and modelref and ModelSafePtr
  auto spModel = m<C>("me", {});    // create new model.
  spModel->c = 12;                  // update the model value.
  ModelRef<char> r1(0);             // create a model reference.
  r1.Bind("me", &C::c);             // bind the model reference to "a field of a model".
  auto spCinModel = r1.SafePtr();   // get safe pointer to binding field.
  *spCinModel = 28;                 // set the binding field value.

  double dval = *spCinModel;

  ModelRef<C> cref{ nullptr }; // create a model reference.
  cref.Bind("me");             // bind the model reference to "model itself".
  cref.SafePtr()->a = 10;      // access the model.

  //test delete
  test_delete();
  test_delete2();
  test_delete3();

  // TODO: ここにコードを挿入してください。
  auto spM1 = m<string>("id", { "value" });   // create a new model.
  MessageBoxA(0, spM1->c_str(), "test", 0);   // the output should be "value".

  *spM1 = "new value";                        // update the model value.
  MessageBoxA(0, spM1->c_str(), "test", 0);   // the output should be "new value".

  auto spM2 = getm<string>("id");             // get the model from its id.
  MessageBoxA(0, spM2->c_str(), "test", 0);   // the output should be "new value".

  // create a new view, with btn1 as the id and the following parameter used to initialize the view.
  auto btn = v<Button>("btn1", L"My Button");

  // output the text in the view.
  MessageBox(0, btn->title->c_str(), L"test", 0);

  // bind the view's field with a model.
  btn->title.Bind("id");

  // register event handlers.
  btn->AddEventHandler(1, MyController::DoSomething);
  btn->AddEventHandler(2, MyController::DoSomething2);

  // the output should be "new value" (the model's value).
  MessageBox(0, btn->title->c_str(), L"test", 0);

  // update the model.
  *spM2 = "button value";

  // the output of the view should be "button value" (updated with the model).
  MessageBox(0, btn->title->c_str(), L"test", 0);

  // Fire the event. will redraw the button.
  btn->FireEvent(1);

  // Fire the event again. this time will not redraw because the value of the model was not changed.
  btn->FireEvent(1);

  // Fire the event. will redraw the button.
  btn->FireEvent(2);

  // unbind the field from the model.
  btn->title.UnBind();

  // Fire the event again. this time will not redraw because the binding between 
  // the view and model was not exist and then no model will be changed.
  btn->FireEvent(1);
  btn->FireEvent(2);
}

void do_test2() {
  auto view = v<Window>("wndmain",   //id
                        // 本来应该用{}将下面的参数括起来，但是Window的构造函数只有一个，所以可以省略掉{}。
                        WPViewSet{v<Button>("btnok", L"OK"), v<Button>("btncancel", L"CANCEL")} //sub views
                       );
  auto mok = m<wstring>("mok", L"That's OK!");
  auto mcancel = m<wstring>("mcancel", L"That's Cancel!");

  // bind
  auto btnok = getv<Button>("btnok");
  btnok->title.Bind("mok");
  btnok->SetPos(10, 10, 30, 20);
  auto btncancel = getv<Button>("btncancel");
  btncancel->title.Bind("mcancel");
  btncancel->SetPos(80, 10, 100, 20);

  view->Show();
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
  App::Initialize();
  // do some test about the model and view.
  // funcusec({10, 20, 'Z', 5.8 });
  do_test2();


  App::Uninitialize();
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
