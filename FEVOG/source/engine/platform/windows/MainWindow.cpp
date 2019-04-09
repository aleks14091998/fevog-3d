#include"MainWindow.h"
#include"constants.h"

#include<gl/glew-2.1.0/include/glew.h>
#include<gl/glew-2.1.0/include/wglew.h>

using namespace valk_platform;

CMainWindow::CMainWindow(Engine *engine) :
	engine(engine),
	_hInst(GetModuleHandle(NULL)),
	_hWnd(NULL),
	_hRenderWnd(NULL),
	_hRC(NULL),
	_bIsLooping(false), _c_uiMSAASamples(1u), _c_bVSync(true)
{}

CMainWindow::~CMainWindow() {
	if (_hRC) {
		if (!wglMakeCurrent(NULL, NULL)) {
		
		}
		if (!wglDeleteContext(_hRC)) {
		
		}
	}
	if (_hInst && UnregisterClass("MainWindowClass", _hInst) == FALSE) {
		_hInst = NULL;
	}
}

int CMainWindow::_wWinMain(HINSTANCE hInstance) {
	_bIsLooping = true;
	MSG st_msg = { 0 };
	
	do {
		if (!PeekMessage(&st_msg, NULL, 0, 0, PM_NOREMOVE)) {
			delMainLoop->Invoke();
			SwapBuffers(_hDC);
			continue;
		}

		while (PeekMessage(&st_msg, NULL, 0, 0, PM_REMOVE)) {
			if (WM_QUIT == st_msg.message) {
				_bIsLooping = false;
			}
			else {
				EventSplit(&st_msg);
				TranslateMessage(&st_msg);
				DispatchMessage(&st_msg);
			}
		}
	} while (_bIsLooping);

	delMessageProc->Invoke(TWinMessage(WMT_RELEASED));

	return (int)st_msg.wParam;
}

LRESULT CALLBACK CMainWindow::s_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	CMainWindow *this_ptr = (CMainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (this_ptr) {
		if (message == WM_DESTROY) {
			PostQuitMessage(0);
		}
		this_ptr->delMessageProc->Invoke(WinAPIMsgToEngMsg(message, wParam, lParam));

		if ((message == WM_SYSCOMMAND && (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)) ||
			message == WM_CLOSE) {
			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CMainWindow::InitWindow(ProcDelegate *pDelMainLoop, MsgProcDelegate *pDelMsgProc) {
	delMainLoop = pDelMainLoop;
	delMessageProc = pDelMsgProc;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)CMainWindow::mv_WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInst;
	wcex.hIcon = LoadIcon(_hInst, MAKEINTRESOURCE(APP_MAIN_ICON));
	wcex.hIconSm = LoadIcon(_hInst, MAKEINTRESOURCE(APP_MAIN_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(3);
	wcex.lpszMenuName = MAKEINTRESOURCE(ID_MAIN_MENU);
	wcex.lpszClassName = "MainWindowClass";
	if (RegisterClassEx(&wcex) == FALSE) {
		//	_pEngineCore->AddToLog("Couldn't register window class!", true);
		return E_FAIL;
	}

	//--test--
	ShowModalUserAlert(NULL, "Start File Editor for Valkyrie", "Start FEV");
	//--test--

	_hWnd = CreateWindowExA(WS_EX_APPWINDOW, "MainWindowClass", "Valkyrie Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700, NULL, NULL, _hInst, NULL);
	CreateChildWindow();

	if (!_hWnd)	{
		_hWnd = NULL;
		return E_FAIL;
	}

	if (!(_hDC = GetDC(_hWnd)))	{
		return E_FAIL;
	}
	SetWindowLongPtr(_hRenderWnd, GWLP_USERDATA, (LONG_PTR)this);

	this->InitRender();
	return S_OK;
}

HRESULT CMainWindow::SendMessage(const TWinMessage &stMsg) {
	UINT msg; WPARAM wparam; LPARAM lparam;
	EngMsgToWinAPIMsg(stMsg, msg, wparam, lparam);
	::SendMessage(_hWnd, msg, wparam, lparam);
	return S_OK;
}

HRESULT CMainWindow::SetTitle(const char *pcTxt) {
	SetWindowText(_hWnd, pcTxt);
	return S_OK;
}

HRESULT CMainWindow::BeginMainLoop() {
	return _wWinMain(GetModuleHandle(NULL)) != -1 ? S_OK : E_FAIL;
}

HRESULT CMainWindow::KillWindow() {
	if (_hDC && !ReleaseDC(_hWnd, _hDC)) {
		//error
	}

	if (DestroyWindow(_hWnd) == FALSE) {
		return E_ABORT;
	}

	return S_OK;
}

HRESULT CMainWindow::ConfigureWindow(uint uiResX, uint uiResY, bool bFScreen) {
	DWORD dw_style = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD dw_style_ex = WS_EX_APPWINDOW;

	ShowWindow(_hWnd, SW_MAXIMIZE);
	SetForegroundWindow(_hWnd);

	return S_OK;
}

HRESULT CMainWindow::Destroy() {
	delete this;
	return S_OK;
}

HRESULT CMainWindow::GetWindowHandle(WindowHandle& result) {
	result = _hWnd;
	return S_OK;
}

HRESULT CMainWindow::GetRenderHandle(WindowHandle& result) {
	result = _hRenderWnd;
	return S_OK;
}
/*
HRESULT CMainWindow::InitRender() {
	uint msaa_samples = _c_uiMSAASamples;
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
	//	PFD_SWAP_COPY |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	_hDC = GetDC(_hRenderWnd);
	uint iPixelFormat = ChoosePixelFormat(_hDC, &pfd);

	if (iPixelFormat != 0)
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat(_hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);

		if (bestMatch_pfd.cDepthBits < pfd.cDepthBits)
		{
			return E_FAIL;
		}

		if (SetPixelFormat(_hDC, iPixelFormat, &pfd) == FALSE)
		{
			DWORD dwErrorCode = GetLastError();
			return E_FAIL;
		}
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		return E_FAIL;
	}
	_hRC = wglCreateContext(_hDC);
	wglMakeCurrent(_hDC, _hRC);
	
	return S_OK;
}
*/

HRESULT CMainWindow::InitRender() {
	int pixel_format = NULL;
	uint msaa_samples = _c_uiMSAASamples;
	_hDC = GetDC(_hRenderWnd);
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Bits
		0, 0, 0, 0, 0, 0,							// Per Color Bits and Shifts Ignored
		8,											// Alpha Bits
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24 Bit Z-Buffer (Depth Buffer)
		0,											// Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (msaa_samples > 1)
	{
		HWND	temp_win_handle = NULL;
		HDC		temp_win_dc = NULL;
		HGLRC	temp_win_rc = NULL;
		int temp_pixel_format = NULL;

		if (
			!(temp_win_handle = CreateWindowEx(0, "STATIC", NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL)) ||
			!(temp_win_dc = GetDC(temp_win_handle)) ||
			!(temp_pixel_format = ChoosePixelFormat(temp_win_dc, &pfd)) ||
			!SetPixelFormat(temp_win_dc, temp_pixel_format, &pfd) ||
			!(temp_win_rc = wglCreateContext(temp_win_dc)) ||
			!wglMakeCurrent(temp_win_dc, temp_win_rc)
			)
		{
			msaa_samples = 1;
		}
		else
		{
			if (string((char*)glGetString(GL_EXTENSIONS)).find("ARB_multisample") != string::npos)
			{
				int ia_attributes[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_COLOR_BITS_ARB, 32,
					WGL_ALPHA_BITS_ARB, 8,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_STENCIL_BITS_ARB, 0,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
					WGL_SAMPLES_ARB, msaa_samples,
					0,0
				};

				float	fa_attributes[] = { 0,0 };
				uint	formats_cnt;
				int		tmp_pixel_format;

				wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
				int valid = wglChoosePixelFormatARB(temp_win_dc, ia_attributes, fa_attributes, 1, &tmp_pixel_format, &formats_cnt);
				wglChoosePixelFormatARB = NULL;

				if (valid && formats_cnt >= 1)
					pixel_format = tmp_pixel_format;
				else
				{
					msaa_samples = 1;
				}
			}
			else
				msaa_samples = 1;
		}

		if (
			!wglMakeCurrent(NULL, NULL) ||
			(temp_win_rc != NULL && !wglDeleteContext(temp_win_rc)) ||
			(temp_win_dc != NULL && !ReleaseDC(temp_win_handle, temp_win_dc)) ||
			(temp_win_handle != NULL && !DestroyWindow(temp_win_handle))
			)
			int test = 4;
	}

	if (pixel_format == NULL && !(pixel_format = ChoosePixelFormat(_hDC, &pfd)))
	{
		return E_ABORT;
	}

	if (!SetPixelFormat(_hDC, pixel_format, &pfd))
	{
		return E_ABORT;
	}

	if (!(_hRC = wglCreateContext(_hDC))) {
		return E_ABORT;
	}

	if (!wglMakeCurrent(_hDC, _hRC)) {
		return E_ABORT;
	}

	GLenum glew_res = glewInit();

	if (glew_res != GLEW_OK)
	{
		string cause = "";

		switch (glew_res)
		{
		case GLEW_ERROR_NO_GL_VERSION:
			cause = "Reason: Can't get OpenGL version.";
			break;
		case GLEW_ERROR_GL_VERSION_10_ONLY:
			cause = "Reason: OpenGL 1.1 or higher required.";
			break;
		}

		return E_ABORT;
	}

	if (WGLEW_EXT_swap_control && _c_bVSync)
		wglSwapIntervalEXT(1);

	return S_OK;
}

HRESULT CMainWindow::GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom) {
	if (!_hRenderWnd) {
		return E_FAIL;
	}
	RECT rect;
	::GetClientRect(_hRenderWnd, &rect);

	POINT lt, rb;

	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;

	ClientToScreen(_hRenderWnd, &lt);
	ClientToScreen(_hRenderWnd, &rb);

	left = lt.x;
	right = rb.x;
	top = lt.y;
	bottom = rb.y;

	return S_OK;
}

HRESULT CMainWindow::CreateChildWindow() {
	WNDCLASSEX wcexRender;
	// render window
	wcexRender.cbSize = sizeof(WNDCLASSEX);
	wcexRender.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcexRender.lpfnWndProc = (WNDPROC)CMainWindow::s_WndProc;
	wcexRender.cbClsExtra = 0;
	wcexRender.cbWndExtra = 0;
	wcexRender.hInstance = _hInst;
	wcexRender.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcexRender.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexRender.hbrBackground = (HBRUSH)(0);
	wcexRender.lpszMenuName = NULL;
	wcexRender.lpszClassName = "RenderWindowClass";
	wcexRender.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// object list window
	WNDCLASSA wcexObjectList;
	memset(&wcexObjectList, 0, sizeof(wcexObjectList));
	wcexObjectList.lpfnWndProc = (WNDPROC)CMainWindow::ol_WndProc;
	wcexObjectList.hInstance = _hInst;
	wcexObjectList.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexObjectList.hbrBackground = (HBRUSH)GetStockObject(3);
	wcexObjectList.lpszClassName = "ObjectListClass";

	// object params window
	WNDCLASSA wcexObjectParams;
	memset(&wcexObjectParams, 0, sizeof(wcexObjectParams));
	wcexObjectParams.lpfnWndProc = (WNDPROC)CMainWindow::mv_WndProc;
	wcexObjectParams.hInstance = _hInst;
	wcexObjectParams.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexObjectParams.hbrBackground = (HBRUSH)GetStockObject(3);
	wcexObjectParams.lpszClassName = "ObjectParamsClass";
	// spliters
	WNDCLASS wcSPlitV;
	ZeroMemory(&wcSPlitV, sizeof(WNDCLASS));
	wcSPlitV.hInstance = _hInst;
	wcSPlitV.lpszClassName = "SplitterVClass";
	wcSPlitV.lpfnWndProc = (WNDPROC)CMainWindow::mv_WndProc;
	wcSPlitV.hbrBackground = (HBRUSH)GetStockObject(2);
	wcSPlitV.hCursor = LoadCursor((HINSTANCE)NULL, IDC_SIZEWE);

	WNDCLASS wcSPlitH;
	ZeroMemory(&wcSPlitH, sizeof(WNDCLASS));
	wcSPlitH.hInstance = _hInst;
	wcSPlitH.lpszClassName = "SplitterHClass";
	wcSPlitH.lpfnWndProc = (WNDPROC)CMainWindow::mv_WndProc;
	wcSPlitH.hbrBackground = (HBRUSH)GetStockObject(2);
	wcSPlitH.hCursor = LoadCursor((HINSTANCE)NULL, IDC_SIZENS);

	if (RegisterClass(&wcSPlitV) == FALSE || RegisterClass(&wcSPlitH) == FALSE) {
		return E_FAIL;
	}

	if (RegisterClassEx(&wcexRender) == FALSE || RegisterClass(&wcexObjectList) == FALSE || RegisterClass(&wcexObjectParams) == FALSE) {
		//	_pEngineCore->AddToLog("Couldn't register window class!", true);
		return E_FAIL;
	}

	DWORD dwExStyleSplit = WS_EX_WINDOWEDGE;
	DWORD dwStyleSplit = WS_CHILD | WS_VISIBLE;

	int part1X = 0;
	int part1Y = 0;
	int part1W = GetWindowDataInPercent(75, 'w');
	int part1H = GetWindowDataInPercent(70, 'h');

	int part2X = 0;
	int part2Y = part1H;
	int part2W = part1W;
	int part2H = GetWindowDataInPercent(30, 'h');

	int part3X = 0;
	int part3Y = part1Y;
	int part3W = GetWindowDataInPercent(25, 'w');
	int part3H = GetWindowDataInPercent(45, 'h');

	int part4X = part1W;
	int part4Y = part1H;
	int part4W = part3W;
	int part4H = GetWindowDataInPercent(45, 'h');

	// create splits
	split1 = CreateWindowEx(dwExStyleSplit, "SplitterVClass", (LPCTSTR)NULL, dwStyleSplit, part1W, 0, 3, GetWindowDataInPercent(100, 'h'), _hWnd, (HMENU)NULL, _hInst, (LPVOID)NULL);
	split2 = CreateWindowEx(dwExStyleSplit, "SplitterHClass", (LPCTSTR)NULL, dwStyleSplit, 0, part1H, part1W, 3, _hWnd, (HMENU)NULL, _hInst, (LPVOID)NULL);
	split3 = CreateWindowEx(dwExStyleSplit, "SplitterHClass", (LPCTSTR)NULL, dwStyleSplit, part1W, part3H, part3W, 3, _hWnd, (HMENU)NULL, _hInst, (LPVOID)NULL);

	// create children
	_hRenderWnd = CreateWindow("RenderWindowClass", "Render", WS_CHILD | WS_BORDER, 0, 0, part1W, part1H, _hWnd, NULL, _hInst, NULL);
	_hObjectListWnd = CreateWindow("ObjectListClass", "Object List", WS_CHILD | WS_BORDER, part1W - 1, 0, part3W, part3H, _hWnd, NULL, _hInst, NULL);
	
	_hObjList = CreateWindow("LISTBOX", "CarCatalogListBox", LBS_EXTENDEDSEL | WS_CHILD | WS_VSCROLL | WS_VISIBLE | LBS_USETABSTOPS, 0, 0, part3W, part3H, _hObjectListWnd, NULL, NULL, NULL);
	

	ShowWindow(_hRenderWnd, SW_SHOWNORMAL);
	ShowWindow(_hObjectListWnd, SW_SHOWNORMAL);

	return S_OK;
}

LRESULT CALLBACK CMainWindow::mv_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_COMMAND: {
			switch (wParam)	{
				case ID_MVM_HELP_ABOUT:
					ShowModalUserAlert(hWnd, "Author: Alexander Ursul\nOpenGL Version: 4.3", "About");
				break;

				case ID_MVM_FILE_EXIT:
					uint result = MessageBox(hWnd, "Do you really want to leave", "Exit", MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
					if (result == IDYES) {
						CMainWindow::s_WndProc(hWnd, WM_CLOSE, 0, 0);
					}
					
				break;
			}
		}
		case WM_KEYDOWN:

		break;
		case WM_MOUSEMOVE:
			if (GetFocus() != hWnd) {
				SetFocus(hWnd);
			}
		break;

		case WM_CLOSE:
		/*	uint result = MessageBox(hWnd, "Do you really want to leave", "Exit", MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);
			if (result == IDYES) {
				CMainWindow::s_WndProc(hWnd, WM_CLOSE, 0, 0);
			}*/
		break;
		
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK CMainWindow::ol_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CTLCOLORLISTBOX: {
			LRESULT color = (LRESULT)GetStockObject(3);
			SetBkMode((HDC)wParam, color);
			SetTextColor((HDC)wParam, RGB(180, 180, 180));
			return color;
		}

		case WM_CREATE:
		
		break;

		case WM_MOUSEMOVE: 
			if (GetFocus() != hWnd) {
				SetFocus(hWnd);
			}
		break;
			
		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

}

int CMainWindow::GetWindowDataInPercent(int percent, char type) {
	HDC tempHdc = GetDC(NULL);
	switch (type) {
		case 'w':			
			if (percent == 100) {
				return GetDeviceCaps(tempHdc, HORZRES);
			}
			return GetDeviceCaps(tempHdc, HORZRES) * percent / 100;
		case 'h':
			if (percent == 100) {
				return GetDeviceCaps(tempHdc, VERTRES);
			}
			return GetDeviceCaps(tempHdc, VERTRES) * percent / 100;
	}
}

void CMainWindow::EventSplit(MSG *msg) {
	switch (msg->message) {
		case WM_LBUTTONDOWN:
			if (GetFocus() == split1) {
				_bSplit1 = true;
			} else if (GetFocus() == split2){
				_bSplit2 = true;
			} else if (GetFocus() == split3) {
				_bSplit3 = true; 
			}
			break;
		case WM_LBUTTONUP:
			_bSplit1 = false;
			_bSplit2 = false;
			_bSplit3 = false;
		///	SetForegroundWindow(_hRenderWnd);
			break;
	}
	if (_bSplit1) {
		POINT ptPos;
		RECT part1Rect, part2Rect, mainRect, mainClientRect;
		GetCursorPos(&ptPos);
		GetWindowRect(_hWnd, &mainRect);
		::GetClientRect(_hWnd, &mainClientRect);
		GetWindowRect(_hRenderWnd, &part1Rect);
		GetWindowRect(_hObjectListWnd, &part2Rect);
		int part2W = part2Rect.bottom - part2Rect.top;
		int part2Split = (mainRect.right - mainRect.left) - (part1Rect.right - part1Rect.left);
		int paddingX = mainRect.right - mainClientRect.right - 7;
		ptPos.x = ptPos.x - paddingX;
		SetWindowPos(split1, NULL, ptPos.x, 0, 0, 0, SWP_NOSIZE);
		SetWindowPos(split2, NULL, 0, 0, ptPos.x, 3, SWP_NOMOVE | SWP_NOZORDER);
		SetWindowPos(split3, NULL, ptPos.x - 1, part2W, part2Split, 3, SWP_NOZORDER);

		SetWindowPos(_hRenderWnd, NULL, 0, 0, ptPos.x, part1Rect.bottom - part1Rect.top, SWP_NOMOVE | SWP_NOZORDER);
		SetWindowPos(_hObjectListWnd, NULL, ptPos.x-1, 0, part2Split, part2W, SWP_NOZORDER);
		SetWindowPos(_hObjList, NULL, 0, 0, part2Split - 15, part2W, SWP_NOMOVE | SWP_NOZORDER);

		InvalidateRect(_hObjList, 0, true);

	} else if (_bSplit2) {
		POINT ptPos;
		RECT part1Rect, part2Rect, mainRect, mainClientRect;
		GetCursorPos(&ptPos);
		GetWindowRect(_hWnd, &mainRect);
		::GetClientRect(_hWnd, &mainClientRect);
		GetWindowRect(_hRenderWnd, &part1Rect);
		GetWindowRect(_hObjectListWnd, &part2Rect);
		int paddingY = mainRect.bottom - mainClientRect.bottom - 7;
		int part2W = part2Rect.bottom - part2Rect.top;
		int part2Split = (mainRect.right - mainRect.left) - (part1Rect.right - part1Rect.left);
		ptPos.y = ptPos.y - paddingY;
		SetWindowPos(split2, NULL, 0, ptPos.y, 0, 0, SWP_NOSIZE);
		SetWindowPos(_hRenderWnd, NULL, 0, 0, part1Rect.right - part1Rect.left, ptPos.y, SWP_NOMOVE | SWP_NOZORDER);
	
	} else if (_bSplit3) {

	}
	if(GetFocus() == split1)
	OutputDebugString("true\n");
	else
	OutputDebugString("false\n");

}

void CMainWindow::UpdateObjectList(std::vector<IObject*> objList) {
	::SendMessage(_hObjList, LB_RESETCONTENT, 0, 0);
	for (uint i = 0; i < objList.size(); i++) {
		::SendMessage(_hObjList, LB_ADDSTRING, 0, (LPARAM)(LPSTR)objList[i]->GetName());
	}
}