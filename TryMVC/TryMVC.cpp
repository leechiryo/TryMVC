// TryMVC.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "TryMVC.h"

#include "mvc\Button.h"
#include "mvc\mvc.h"
#include "mvc\ModelRef.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X
WCHAR szTitle[MAX_LOADSTRING];                  // �^�C�g�� �o�[�̃e�L�X�g
WCHAR szWindowClass[MAX_LOADSTRING];            // ���C�� �E�B���h�E �N���X��

struct C{
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
bool objsptr(T * obj, void *ptr){
  char * objaddr = reinterpret_cast<char*>(obj);
  char * ptraddr = reinterpret_cast<char*>(ptr);
  return (ptraddr > objaddr) && ((ptraddr - objaddr) < sizeof(T));
}

// test the model delete.
void test_delete() {
  auto &model = mvc::m<C>("delete");
  App::RemoveModel("delete");  // -- delete the model here.
}

void stubfunc(shared_ptr<Model<C>> ptr) {
  
}

// test the model delete.
void test_delete2() {
  auto &model = mvc::m<C>("delete2");
  stubfunc(App::GetModel<C>("delete2"));

  ModelRef<double> refd{ 0.0 }; // define a new model ref
  refd.Bind<C>(App::GetModel<C>("delete2"), &C::d);

  auto &accd = refd.GetAccessor(); // accd to d

  accd = 2.0;

  App::RemoveModel("delete2");

  accd = 5.0;
} // -- delete the model here.

// test the model delete.
void test_delete3() {
  auto &model = mvc::m<C>("delete3");
  stubfunc(App::GetModel<C>("delete3"));

  ModelRef<double> refd{ 0.0 }; // define a new model ref
  refd.Bind<C>(App::GetModel<C>("delete3"), &C::d);

  refd.GetAccessor() = 2.0;

  App::RemoveModel("delete3"); // -- model deleted here.

  refd.GetAccessor() = 5.0;  // will change a value of inner fallback.
                             // model won't be changed which is already destoried.
}

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
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

  // test model and modelref and modelaccessor
  auto &m9 = mvc::m<C>("me");
  ModelRef<double> r1(0.0);
  auto spm9 = App::GetModel<C>("me");
  r1.Bind<C>(spm9, &C::d);
  auto m9ac = r1.GetAccessor();
  m9ac = 1.2;
  
  ModelRef<C> cref;
  cref.Bind<C>(spm9);
  cref.GetAccessor()->a = 10;

  //test delete
  test_delete();

  test_delete2();

  test_delete3();


  // TODO: �����ɃR�[�h��}�����Ă��������B
  Model<string> b("this");
  auto &m1 = mvc::m<string>("id", "value");
  MessageBoxA(0, m1.c_str(), "test", 0);

  m1 = "new value";
  MessageBoxA(0, m1.c_str(), "test", 0);

  auto &m2 = mvc::getm<string>("id");
  MessageBoxA(0, m2.c_str(), "test", 0);

  auto &btn = mvc::v<Button>("btn1", "My Button");
  MessageBoxA(0, btn.GetTitle().c_str(), "test", 0);

  btn.SetTitle("Your Button");
  MessageBoxA(0, btn.GetTitle().c_str(), "test", 0);

  auto &btn2 = mvc::getv<Button>("btn1");
  MessageBoxA(0, btn2.GetTitle().c_str(), "test", 0);

  // �O���[�o������������������Ă��܂��B
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_TRYMVC, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // �A�v���P�[�V�����̏����������s���܂�:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRYMVC));

  MSG msg;

  // ���C�� ���b�Z�[�W ���[�v:
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
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

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
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E�̕`��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // �I�����ꂽ���j���[�̉��:
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
    // TODO: HDC ���g�p����`��R�[�h�������ɒǉ����Ă�������...
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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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
