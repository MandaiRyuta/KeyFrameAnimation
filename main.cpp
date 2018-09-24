#include "main.h"
#include <vector>
#include "Object\camera.h"
#include "LoadModel.h"
#include "Input\input.h"
#include "Object\grit.h"
const std::string CLASS_NAME = "Screen Segmentation";
const std::string WINDOW_NAME = "FrameWork";

//********************************************************************************
//
// プロトタイプ宣言
//
//********************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ
bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight);
void Uninit(void);
void Update(void);
void Draw(void);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//********************************************************************************
//
//グローバル変数
//
//********************************************************************************
static D3DPRESENT_PARAMETERS    g_d3dpp;
static LPDIRECT3D9 pD3D_;
static LPDIRECT3DDEVICE9 device_;
bool drawset;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
int color[4];
Camera* camera;
Player* pPlayer;
Grit* grit;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 使っていない引数の処理
	lpCmdLine = lpCmdLine;
	hPrevinstance = hPrevinstance;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	// ウィンドウクラスの登録
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME.c_str();
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);		// WNDCLASSEX型のwcexの中身を登録

	// クライアントサイズの設定
	DWORD WStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 矩形データ
	AdjustWindowRect(&wr, WStyle, FALSE);

	int nWindowWidth = wr.right - wr.left;
	int nWindowHeight = wr.bottom - wr.top;
	// ウィンドウの場所を中央に変更
	// デスクトップサイズの取得
	RECT DesktopRect;

	GetWindowRect(GetDesktopWindow(), &DesktopRect);
	int nWindowPosX = (DesktopRect.right - nWindowWidth) / 3;
	int nWindowPosY = (DesktopRect.bottom - nWindowHeight) / 3;

	(DesktopRect.right - DesktopRect.left) < nWindowPosX ? nWindowPosX = 0 : nWindowPosX;
	(DesktopRect.bottom - DesktopRect.top) < nWindowPosY ? nWindowPosY = 0 : nWindowPosY;

	// ウィンドウの作成
	HWND hWnd = CreateWindowEx(0,
		CLASS_NAME.c_str(),
		WINDOW_NAME.c_str(),
		WStyle,
		nWindowPosX,
		nWindowPosY,
		nWindowWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2, //なぜ?
		nWindowHeight + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION), //なぜ?
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージ&ゲームループ
	MSG msg;
	DWORD dwTime = timeGetTime();
	DWORD dwOldTime = timeGetTime();

	// タイマーの性能を上げる
	timeBeginPeriod(1);
	// 初期化(チェックあり)
	if (!Init(hInstance, hWnd, true, nWindowWidth, nWindowHeight))
	{
		MessageBox(hWnd, "初期化に失敗しました!!", "警告", MB_OK);

		return -1;
	}

	// Setup ImGui binding
	ImGui_ImplDX9_Init(hWnd, device_);
	// Setup style
	ImGui::StyleColorsClassic();

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{// メッセージ処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// ゲーム処理
			ImGui_ImplDX9_NewFrame();

			// 更新
			Update();
			// 描画
			Draw();

			ImGui::EndFrame();
		}
	}

	ImGui::Shutdown();
	// 終了
	Uninit();

	timeEndPeriod(1);

	return (int)msg.wParam;
}

//================================================================================
//
// 初期化処理関数
//
//================================================================================

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// 使っていない引数の処理
	hInstance = hInstance;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D_ == NULL)
	{
		MessageBox(hWnd, "Direct3Dインターフェースの取得に失敗しました。", NULL, MB_OK);
		return false;
	}

	D3DDISPLAYMODE d3ddm;

	if (FAILED(pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "ディスプレイモードの取得に失敗しました。", NULL, MB_OK);
		return false;
	}

	// デバイスのプレゼンテーションパラメータ(デバイスを作成するためのデータ)
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));							//　中のものを全てを初期化する(初期化する先頭アドレス, 初期化するサイズ)
	d3dpp.BackBufferWidth = nWindowWidth;				//　裏画面(バックバッファ)の幅
	d3dpp.BackBufferHeight = nWindowHeight;				//	裏画面(バックバッファ)の高さ
	d3dpp.BackBufferFormat = d3ddm.Format;				//	色数
	d3dpp.BackBufferCount = 1;							//	バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//	映像信号の同期(DISCARDは垂直同期を気にする)
	d3dpp.EnableAutoDepthStencil = TRUE;						//	TRUEで良い
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//	今はこれで良い上のフォーマット
	d3dpp.Windowed = bWindow;						//	TRUEでウィンドウモード,FALSEでフルスクリーンモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//	フルスクリーンモードの時のリフレッシュレートのHz数を変更
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//	モニターのHz数に合わせて描画される(IMMEDIATEだと垂直同期を待たずに描画する)

	if (FAILED(pD3D_->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device_
	)))
	{
		MessageBox(hWnd, "Direct3DDeviceの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	device_->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device_->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device_->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	device_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	camera = new Camera(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 30.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	drawset = false;
	
	pPlayer = new Player();
	pPlayer->Init(device_);

	InitKeyboard(hInstance, hWnd);
	//
	grit = new Grit();
	
	return true;
}

//================================================================================
//
// 終了処理関数
//
//================================================================================
void Uninit(void)
{
	UninitKeyboard();
	drawset = false;
	delete camera;

	delete grit;

	pPlayer->Uninit();
	delete pPlayer;
	pPlayer = NULL;

	pD3D_->Release();
	device_->Release();
}

//================================================================================
//
// 更新関数
//
//================================================================================
void Update(void)
{
	UpdateKeyboard();
	camera->Update();
	pPlayer->Update();
	grit->Update();
	ImGui::GetPositionInfomation("Camera", camera->SetEye());
	ImGui::GetPositionInfomation("Camera", camera->SetAt());
}

//================================================================================
//
// 描画関数
//
//================================================================================
void Draw(void)
{
	device_->Clear(0,										// RECT構造体配列の矩形の数
		NULL,									// RECT構造体の先頭アドレス(画面全体はNULL)
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// TARGETは色のクリア、ZBUFFERはZバッファのクリア
		D3DCOLOR_RGBA(30, 64, 192, 255),		// クリアカラ―(TARGETがあるとき)
		1.0f,									// Zバッファのクリア値
		0);									// ステンシル値のクリア値
											//DrawCall
	if (SUCCEEDED(device_->BeginScene()))
	{
		ImGui::GetFPS();
		grit->Draw();
		ImGui::Render();
		pPlayer->Draw(device_);
	}
	device_->EndScene();
	HRESULT result = device_->Present(NULL, NULL, NULL, NULL);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device_->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		device_->Reset(&g_d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}
//================================================================================
//
// ウィンドウプロシージャ関数
//
//================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_SIZE:
		if (device_ != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = device_->Reset(&g_d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY: // ウィンドウ破棄メッセージ
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{// 終了メッセージ

			int nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		break;

		}
		break;

	case WM_CLOSE:
	{
		int nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LPDIRECT3DDEVICE9 GetDevice()
{
	return device_;
}