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
// �v���g�^�C�v�錾
//
//********************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��
bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight);
void Uninit(void);
void Update(void);
void Draw(void);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//********************************************************************************
//
//�O���[�o���ϐ�
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
	// �g���Ă��Ȃ������̏���
	lpCmdLine = lpCmdLine;
	hPrevinstance = hPrevinstance;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	// �E�B���h�E�N���X�̓o�^
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

	RegisterClassEx(&wcex);		// WNDCLASSEX�^��wcex�̒��g��o�^

	// �N���C�A���g�T�C�Y�̐ݒ�
	DWORD WStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// ��`�f�[�^
	AdjustWindowRect(&wr, WStyle, FALSE);

	int nWindowWidth = wr.right - wr.left;
	int nWindowHeight = wr.bottom - wr.top;
	// �E�B���h�E�̏ꏊ�𒆉��ɕύX
	// �f�X�N�g�b�v�T�C�Y�̎擾
	RECT DesktopRect;

	GetWindowRect(GetDesktopWindow(), &DesktopRect);
	int nWindowPosX = (DesktopRect.right - nWindowWidth) / 3;
	int nWindowPosY = (DesktopRect.bottom - nWindowHeight) / 3;

	(DesktopRect.right - DesktopRect.left) < nWindowPosX ? nWindowPosX = 0 : nWindowPosX;
	(DesktopRect.bottom - DesktopRect.top) < nWindowPosY ? nWindowPosY = 0 : nWindowPosY;

	// �E�B���h�E�̍쐬
	HWND hWnd = CreateWindowEx(0,
		CLASS_NAME.c_str(),
		WINDOW_NAME.c_str(),
		WStyle,
		nWindowPosX,
		nWindowPosY,
		nWindowWidth + GetSystemMetrics(SM_CXDLGFRAME) * 2, //�Ȃ�?
		nWindowHeight + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION), //�Ȃ�?
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W&�Q�[�����[�v
	MSG msg;
	DWORD dwTime = timeGetTime();
	DWORD dwOldTime = timeGetTime();

	// �^�C�}�[�̐��\���グ��
	timeBeginPeriod(1);
	// ������(�`�F�b�N����)
	if (!Init(hInstance, hWnd, true, nWindowWidth, nWindowHeight))
	{
		MessageBox(hWnd, "�������Ɏ��s���܂���!!", "�x��", MB_OK);

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
		{// ���b�Z�[�W����
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
		{// �Q�[������
			ImGui_ImplDX9_NewFrame();

			// �X�V
			Update();
			// �`��
			Draw();

			ImGui::EndFrame();
		}
	}

	ImGui::Shutdown();
	// �I��
	Uninit();

	timeEndPeriod(1);

	return (int)msg.wParam;
}

//================================================================================
//
// �����������֐�
//
//================================================================================

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int nWindowWidth, int nWindowHeight)
{
	// �g���Ă��Ȃ������̏���
	hInstance = hInstance;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D_ == NULL)
	{
		MessageBox(hWnd, "Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s���܂����B", NULL, MB_OK);
		return false;
	}

	D3DDISPLAYMODE d3ddm;

	if (FAILED(pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "�f�B�X�v���C���[�h�̎擾�Ɏ��s���܂����B", NULL, MB_OK);
		return false;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^(�f�o�C�X���쐬���邽�߂̃f�[�^)
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));							//�@���̂��̂�S�Ă�����������(����������擪�A�h���X, ����������T�C�Y)
	d3dpp.BackBufferWidth = nWindowWidth;				//�@�����(�o�b�N�o�b�t�@)�̕�
	d3dpp.BackBufferHeight = nWindowHeight;				//	�����(�o�b�N�o�b�t�@)�̍���
	d3dpp.BackBufferFormat = d3ddm.Format;				//	�F��
	d3dpp.BackBufferCount = 1;							//	�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//	�f���M���̓���(DISCARD�͐����������C�ɂ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						//	TRUE�ŗǂ�
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//	���͂���ŗǂ���̃t�H�[�}�b�g
	d3dpp.Windowed = bWindow;						//	TRUE�ŃE�B���h�E���[�h,FALSE�Ńt���X�N���[�����[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//	�t���X�N���[�����[�h�̎��̃��t���b�V�����[�g��Hz����ύX
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//	���j�^�[��Hz���ɍ��킹�ĕ`�悳���(IMMEDIATE���Ɛ���������҂����ɕ`�悷��)

	if (FAILED(pD3D_->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&device_
	)))
	{
		MessageBox(hWnd, "Direct3DDevice�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
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
// �I�������֐�
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
// �X�V�֐�
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
// �`��֐�
//
//================================================================================
void Draw(void)
{
	device_->Clear(0,										// RECT�\���̔z��̋�`�̐�
		NULL,									// RECT�\���̂̐擪�A�h���X(��ʑS�̂�NULL)
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),	// TARGET�͐F�̃N���A�AZBUFFER��Z�o�b�t�@�̃N���A
		D3DCOLOR_RGBA(30, 64, 192, 255),		// �N���A�J���\(TARGET������Ƃ�)
		1.0f,									// Z�o�b�t�@�̃N���A�l
		0);									// �X�e���V���l�̃N���A�l
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
// �E�B���h�E�v���V�[�W���֐�
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
	case WM_DESTROY: // �E�B���h�E�j�����b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{// �I�����b�Z�[�W

			int nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);

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
		int nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);

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