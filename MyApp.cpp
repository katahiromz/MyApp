// MyApp.cpp --- 私のアプリのソース
#include "MyApp.h" // 私のアプリのヘッダ
#include "AntiRE.h" // 逆工学対策

#define MYAPP_CLASSNAME   TEXT("MyApp")  // アプリのウィンドウクラス
#define MYAPP_TITLE       TEXT("MyApp Ver.0.0") // アプリのタイトル

// アプリのインスタンス
HINSTANCE g_hInst = NULL;

// メインウィンドウのハンドル
HWND g_hMainWnd = NULL;

// アプリのクラス
class MyApp : public MObject
{
public:
    MyApp();
    ~MyApp();

    BOOL init(HINSTANCE hInstance, INT argc, TCHAR **argv, INT nCmdShow);
    void reset_settings();
    BOOL load_settings();
    BOOL save_settings();
    INT run();
    void exit();

#ifdef MYAPP_IS_DIALOG // ダイアログアプリか？
    static INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#else
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

    // メッセージハンドラ
#ifdef MYAPP_IS_DIALOG // ダイアログアプリか？
    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
#else
    BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);
    void OnPaint(HWND hwnd);
    void OnDestroy(HWND hwnd);
#endif
    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

protected:
    INT m_cx = CW_USEDEFAULT, m_cy = CW_USEDEFAULT; // ウィンドウのサイズ
};

// コンストラクタ
MyApp::MyApp()
{
    mdbg_footmark();
}

// デストラクタ
MyApp::~MyApp()
{
    mdbg_footmark();
}

// アプリ初期化
BOOL MyApp::init(HINSTANCE hInstance, INT argc, TCHAR **argv, INT nCmdShow)
{
    mdbg_footmark();

    // 必要ならば設定を読み込む
    //load_settings();

    g_hInst = hInstance;

    // コモンコントロールを初期化
    ::InitCommonControls();

#ifndef MYAPP_IS_DIALOG // ダイアログアプリではない？
    // ウィンドウクラスを登録
    WNDCLASSEX wcx = { sizeof(wcx) };
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcx.lpfnWndProc = WindowProc;
    wcx.hInstance = hInstance;
    wcx.hIcon = ::LoadIcon(NULL, IDI_APPLICATION); // アプリアイコン
    wcx.hCursor = ::LoadCursor(NULL, IDC_ARROW); // 矢印カーソル
    wcx.hbrBackground = (HBRUSH)UlongToHandle(COLOR_3DFACE + 1); // 背景は灰色
    //wcx.lpszMenuName = MAKEINTRESOURCE(100); // リソースメニューを使う？
    wcx.lpszClassName = MYAPP_CLASSNAME;
    if (!::RegisterClassEx(&wcx))
    {
        ::MessageBox(NULL, TEXT("Failed to RegisterClassEx"), MYAPP_TITLE, MB_ICONERROR);
        return FALSE; // 失敗
    }

    // メインウィンドウを作成
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exstyle = 0;
    HWND hwnd = ::CreateWindowEx(exstyle, MYAPP_CLASSNAME, MYAPP_TITLE, style,
                                 CW_USEDEFAULT, CW_USEDEFAULT, m_cx, m_cy,
                                 NULL, NULL, hInstance, this);
    if (!hwnd)
    {
        ::MessageBox(NULL, TEXT("Failed to CreateWindowEx."), MYAPP_TITLE, MB_ICONERROR);
        return FALSE; // 失敗
    }

    // メインウィンドウを表示
    ::ShowWindow(hwnd, nCmdShow);
    ::UpdateWindow(hwnd);
#endif

    return TRUE; // 成功
}

// アプリの実行
INT MyApp::run()
{
    mdbg_footmark();
#ifdef MYAPP_IS_DIALOG // ダイアログアプリか？
    // このダイアログはモーダルなので、メッセージループは要らない
    return (INT)::DialogBoxParam(g_hInst, MAKEINTRESOURCE(100), NULL, DialogProc, (LPARAM)this);
#else
    // メッセージループ
    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return (INT)msg.wParam;
#endif
}

// レジストリキー
#define MYAPP_REGKEY TEXT("SOFTWARE\\Katayama Hirofumi MZ\\MyApp")

// 設定を初期化する
void MyApp::reset_settings()
{
    // TODO: 設定の初期値を与える
    m_cx = m_cy = CW_USEDEFAULT;
}

// アプリの設定を読み込む
BOOL MyApp::load_settings()
{
    reset_settings();

    HKEY hKey;
    LONG error = ::RegOpenKeyEx(HKEY_CURRENT_USER, MYAPP_REGKEY, 0, KEY_READ, &hKey);
    if (error)
        return FALSE;

    // TODO: レジストリから設定を読み込む
    assert(sizeof(m_cx) == sizeof(DWORD));
    DWORD cbValue;
    cbValue = sizeof(m_cx);
    ::RegQueryValueEx(hKey, TEXT("m_cx"), NULL, NULL, (BYTE*)&m_cx, &cbValue);
    ::RegQueryValueEx(hKey, TEXT("m_cy"), NULL, NULL, (BYTE*)&m_cy, &cbValue);

    ::RegCloseKey(hKey);
    return TRUE;
}

// アプリの設定を保存する
BOOL MyApp::save_settings()
{
    HKEY hKey;
    LONG error = ::RegCreateKeyEx(HKEY_CURRENT_USER, MYAPP_REGKEY, 0, NULL, 0,
                                  KEY_WRITE, NULL, &hKey, NULL);
    if (error)
        return FALSE;

    // TODO: レジストリに設定を書き込む
    assert(sizeof(m_cx) == sizeof(DWORD));
    ::RegSetValueEx(hKey, TEXT("m_cx"), 0, REG_DWORD, (const BYTE*)&m_cx, sizeof(m_cx));
    ::RegSetValueEx(hKey, TEXT("m_cy"), 0, REG_DWORD, (const BYTE*)&m_cy, sizeof(m_cy));

    ::RegCloseKey(hKey);
    return TRUE;
}

#ifdef MYAPP_IS_DIALOG // ダイアログアプリか？

// WM_INITDIALOG
BOOL MyApp::OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    g_hMainWnd = hwnd;
    SetWindowLongPtr(hwnd, DWLP_USER, lParam);
    return TRUE; // オートフォーカス
}

// ダイアログプロシージャ
/*static*/ INT_PTR CALLBACK
MyApp::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto pThis = reinterpret_cast<MyApp *>(GetWindowLongPtr(hwnd, DWLP_USER));
    if (!pThis && uMsg == WM_INITDIALOG)
    {
        pThis = reinterpret_cast<MyApp *>(lParam);
        SetWindowLongPtr(hwnd, DWLP_USER, reinterpret_cast<LONG_PTR>(pThis));
    }
    if (!pThis)
        return 0;

    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, pThis->OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, pThis->OnCommand);
    default:
        break;
    }

    return 0;
}

#else // ndef MYAPP_IS_DIALOG

// WM_CREATE
BOOL MyApp::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    g_hMainWnd = hwnd;

    // TODO: ウィンドウ作成時
    return TRUE;
}

// WM_SIZE
void MyApp::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    // TODO: ウィンドウサイズ変更時

    // ウィンドウサイズを保存
    RECT rc;
    ::GetWindowRect(hwnd, &rc);
    m_cx = rc.right - rc.left;
    m_cy = rc.bottom - rc.top;
}

// WM_PAINT
void MyApp::OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    if (HDC hdc = ::BeginPaint(hwnd, &ps))
    {
        RECT rc;
        ::GetClientRect(hwnd, &rc);
        assert(rc.left == 0);
        assert(rc.top == 0);

        ::SelectObject(hdc, GetStockPen(BLACK_PEN));
        ::MoveToEx(hdc, 0, 0, NULL);
        ::LineTo(hdc, rc.right, rc.bottom);
        ::MoveToEx(hdc, rc.right, 0, NULL);
        ::LineTo(hdc, 0, rc.bottom);

        ::SelectObject(hdc, GetStockPen(DC_BRUSH));
        ::SetDCBrushColor(hdc, RGB(255, 255, 0));
        ::Ellipse(hdc, 0, 0, rc.right, rc.bottom);

        ::SetTextColor(hdc, RGB(0, 0, 0));
        ::DrawTextW(hdc, L"こんにちは、ウィンドウアプリ!", -1, &rc,
                    DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOPREFIX);

        ::EndPaint(hwnd, &ps);
    }
}

// WM_DESTROY
void MyApp::OnDestroy(HWND hwnd)
{
    // TODO: ウィンドウ破棄時

    // メッセージループを終了させる
    ::PostQuitMessage(0);
}

// ウィンドウプロシージャ
/*static*/ LRESULT CALLBACK
MyApp::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto pThis = reinterpret_cast<MyApp *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (!pThis && uMsg == WM_CREATE)
    {
        pThis = reinterpret_cast<MyApp *>(LPCREATESTRUCT(lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    if (!pThis)
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam); // デフォルトの処理
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, pThis->OnCreate);
        HANDLE_MSG(hwnd, WM_COMMAND, pThis->OnCommand);
        HANDLE_MSG(hwnd, WM_SIZE, pThis->OnSize);
        HANDLE_MSG(hwnd, WM_PAINT, pThis->OnPaint);
        HANDLE_MSG(hwnd, WM_DESTROY, pThis->OnDestroy);
    default:
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam); // デフォルトの処理
    }
    return 0;
}

#endif // ndef MYAPP_IS_DIALOG 

// WM_COMMAND
void MyApp::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
#ifdef MYAPP_IS_DIALOG // ダイアログアプリか！E
    case IDOK:
    case IDCANCEL:
        // ダイアログを終?E
        ::EndDialog(hwnd, id);
        break;
#else
    case 100:
        // メインウィンドウを破?E
        ::DestroyWindow(hwnd);
        break;
#endif
    default:
        break;
    }
}

void MyApp::exit()
{
    // TODO: アプリ終了時
    mdbg_footmark();

    // 必要ならば設定を保存する
    //save_settings();
}

INT MyApp_Main(HINSTANCE hInstance, INT argc, TCHAR **argv, INT nCmdShow)
{
    // 逆工学対策ルーチン
#ifdef NDEBUG // リリース時のみ
    AntiRE_DetectDebugger(); // Detect debugger
    AntiRE_DetectDebugger2(); // Detect debugger part 2
    AntiRE_CheckDebugRegs(); // Check CPU debug registers
    //AntiRE_DetectVirtualMachine(); // Detect virtual machine (WARNING: License compliant?)
    //AntiRE_DetectTimeAttack(); // Detect time attack (WARNING: License compliant?)
#endif

    // アプリオブジェクトを動的に作成する。これにより逆工学がかなりハードになる
    INT ret = -1;
    auto app = std::make_shared<MyApp>();
    if (app->init(hInstance, argc, argv, nCmdShow))
        ret = app->run();

    // アプリ終了
    app->exit();
    return ret; // 終了コード
}

////////////////////////////////////////////////////////////////////////////////////

// オブジェクト生存数確認用の変数
LONG MObject::s_cAliveObjects = 0;

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    // オブジェクトの個数確認
    assert(MObject::s_cAliveObjects == 0);

#ifdef UNICODE // アプリがUnicode版？
    INT argc;
    LPWSTR *wargv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
    INT ret = MyApp_Main(hInstance, argc, wargv, nCmdShow);
    ::LocalFree(wargv);
#else // アプリがANSI版？
    INT ret = MyApp_Main(hInstance, __argc, __argv, nCmdShow);
#endif

    // オブジェクトの個数確認
    assert(MObject::s_cAliveObjects == 0);

    // ハンドルリークの検出
#if (_WIN32_WINNT >= 0x0500) && !defined(NDEBUG)
    TCHAR szText[MAX_PATH];
    wnsprintf(szText, _countof(szText), TEXT("GDI Objects: %ld, User Objects: %ld\n"),
        GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS),
        GetGuiResources(GetCurrentProcess(), GR_USEROBJECTS));
    OutputDebugString(szText);
#endif

    // メモリーリークの検出（Visual C++のみ）
#if defined(_MSC_VER) && !defined(NDEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // 終了コード
    return ret;
}
