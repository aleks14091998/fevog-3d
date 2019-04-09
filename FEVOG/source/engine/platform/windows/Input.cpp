#include "Input.h"

CInput::CInput(CCore * core):
	core(core),
	_bFocused(false),
	_bIsTxtInput(false),
	_pcBuffer(NULL),
	_clInputTxt("")
{}

HRESULT CInput::Init(IMainWindow* window) {
	core->GetMessageProc()->addFunc(&s_MessageProc, (void*)this);
	core->GetLoopProc()->addFunc(&s_Loop, (void*)this);
	core->AddProcedure(EPT_PROCESS, &s_Process, (void*)this);

	ZeroMemory(_abKeys, 256);

	_stMsts.iDeltaX = 0;
	_stMsts.iDeltaY = 0;
	_stMsts.iX = 0;
	_stMsts.iY = 0;
	_stMsts.iDeltaWheel = 0;
	_stMsts.bLeftButton = false;
	_stMsts.bRightButton = false;
	_stMsts.bMiddleButton = false;

	_bExclusive = false;
	_bHideCursor = true;

	window->GetRenderHandle(_hWnd);

	BYTE ANDmaskIcon[128], XORmaskIcon[128];

	for (int i = 0; i < 128; i++)
	{
		ANDmaskIcon[i] = 0xFF;
		XORmaskIcon[i] = 0x00;
	}

	_hCursor = CreateCursor(GetModuleHandle(NULL), 1, 1, 32, 32, ANDmaskIcon, XORmaskIcon);
	_bFocused = GetFocus() == _hWnd;
	if (!_hCursor) {
		///	Can't create blank cursor
	}

	/// Input Subsystem initialized
}

CInput::~CInput() {
	if (_hCursor) {
		if (DestroyCursor(_hCursor)) {
			/// Can't destroy blank cursor.
		}
	}
	if (_bExclusive) {
		ClipCursor(NULL);
	}

	core->GetMessageProc()->removeFunc(&s_MessageProc, (void*)this);
	core->GetLoopProc()->removeFunc(&s_Loop, (void*)this);
	core->RemoveProcedure(EPT_PROCESS, &s_Process, (void*)this);

	/// Input Subsystem finalized
}

void CInput::_GetInputClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom) {
	RECT rect;
	GetClientRect(_hWnd, &rect);

	POINT lt, rb;

	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;

	ClientToScreen(_hWnd, &lt);
	ClientToScreen(_hWnd, &rb);

	left = lt.x;
	right = rb.x;
	top = lt.y;
	bottom = rb.y;
}

void CInput::Loop() {
	// обработчик
}

void CInput::Process() {
	_stMsts.iDeltaX = _stMsts.iX - _uiOldMouseX;
	_stMsts.iDeltaY = _stMsts.iY - _uiOldMouseY;
	_uiOldMouseX = _stMsts.iX;
	_uiOldMouseY = _stMsts.iY;
	_stMsts.iDeltaWheel = 0;
}

void CInput::_fClipCursor() {
	RECT rect;
	_GetInputClientRect(rect.left, rect.right, rect.top, rect.bottom);
	ClipCursor(&rect);
}

void CInput::MessageProc(const TWinMessage &stMsg) {
	switch (stMsg.uiMsgType) {
		case WMT_CLOSE:
			if (!_bHideCursor) {
				SetClassLong(_hWnd, GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
			}
			break;

		case WMT_DEACTIVATED:
			if (!_bFocused) {
				break;
			}
			ZeroMemory(_abKeys, 256);
			_stMsts.bLeftButton = false;
			_stMsts.bRightButton = false;
			_stMsts.bMiddleButton = false;
			_bFocused = false;
			if (_bExclusive) {
				ClipCursor(NULL);
			}
			break;

		case WMT_ACTIVATED:
			if (_bFocused) {
				break;
			}
			_bFocused = true;
			if (_bExclusive) {
				_fClipCursor();
			}
			break;

		case WMT_KEY_DOWN:
			_abKeys[stMsg.ui32Param1] = true;
			break;

		case WMT_KEY_UP:
			_abKeys[stMsg.ui32Param1] = false;
			break;

		case WMT_MOUSE_MOVE:
			_stMsts.iX = stMsg.ui32Param1;
			_stMsts.iY = stMsg.ui32Param2;
			// test
			if (GetFocus() != _hWnd) {
				SetFocus(_hWnd);
			}
			// test
			break;

		case WMT_MOUSE_DOWN:
			if (stMsg.ui32Param1 == 0) {
				_stMsts.bLeftButton = true;
			}
			else {
				if (stMsg.ui32Param1 == 2) {
					_stMsts.bRightButton = true;
				}
				else if(stMsg.ui32Param1 == 3) {
					_stMsts.bMiddleButton = true;
				}
			}
			break;

		case WMT_MOUSE_UP:
			if (stMsg.ui32Param1 == 0) {
				_stMsts.bLeftButton = false;
			}
			else
				if (stMsg.ui32Param1 == 2) {
					_stMsts.bRightButton = false;
				}
				else if(stMsg.ui32Param1 == 3) {
					_stMsts.bMiddleButton = false;
				}
			break;

		case WMT_MOUSE_WHEEL:
			_stMsts.iDeltaWheel = *((int*)stMsg.pParam3);
			break;

		case WMT_ENTER_CHAR:
			if (_bIsTxtInput) {
				if (stMsg.ui32Param1 > 31) {
					_clInputTxt += (char)stMsg.ui32Param1;
				}
				else {
					if (stMsg.ui32Param1 == 8 && _clInputTxt.length() > 0)
						_clInputTxt.erase(_clInputTxt.length() - 1, 1);
				}

				if (_uiBufSize > _clInputTxt.size()) {
					strcpy(_pcBuffer, _clInputTxt.c_str());
				}
				else {
					EndTextInput();
				}
			}
			break;
	}
}

HRESULT CALLBACK CInput::Configure(E_INPUT_CONFIGURATION_FLAGS flag) {
	_bExclusive = (flag & ICF_EXCLUSIVE) != 0;

	_bHideCursor = (flag & ICF_HIDE_CURSOR) != 0;

	if (_bExclusive) {
		_fClipCursor();
	}
	else {
		ClipCursor(NULL);
	}

	if (_bHideCursor) {
		SetClassLong(_hWnd, GCLP_HCURSOR, (LONG)_hCursor);
	}
	else {
		SetClassLong(_hWnd, GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
	}
	return S_OK;
}

HRESULT CALLBACK CInput::GetMouseStates(MouseStates &stMs) const {
	stMs = _stMsts;
	return S_OK;
}

HRESULT CALLBACK CInput::GetKeyState(E_KEYBOARD_KEY_CODES key, bool &state) const {
	state = _abKeys[key];
	return S_OK;
}

HRESULT CALLBACK CInput::BeginTextInput(char *cBuffer, uint uiBufferSize) {
	_pcBuffer = cBuffer;
	_uiBufSize = uiBufferSize;
	_bIsTxtInput = true;
	_clInputTxt = "";
	strcpy(_pcBuffer, "");
	return S_OK;
}

HRESULT CALLBACK CInput::EndTextInput() {
	_bIsTxtInput = false;
	return S_OK;
}

void CALLBACK CInput::s_MessageProc(void *pParametr, const TWinMessage &stMsg)
{
	((CInput*)pParametr)->MessageProc(stMsg);
}

void CALLBACK CInput::s_Loop(void *pParametr) {
	((CInput*)pParametr)->Loop();
}

void CALLBACK CInput::s_Process(void *pParametr) {
	((CInput*)pParametr)->Process();
}

void CALLBACK CInput::_SetCursorPos(bool toCenter, int x = 0, int y = 0) {
	if (toCenter) {
		RECT rect, pos;
		GetClientRect(_hWnd, &rect);
		_GetInputClientRect(pos.left, pos.right, pos.top, pos.bottom);
		x = ((rect.right - rect.left) >> 1) + pos.left;
		y = ((rect.bottom - rect.top) >> 1) + pos.top;
		SetCursorPos(x, y);
	}
	else {
		RECT pos;
		_GetInputClientRect(pos.left, pos.right, pos.top, pos.bottom);
		int nx = x +  pos.left;
		int ny = y + pos.top;
		SetCursorPos(nx, ny);
	}
}

void CALLBACK CInput::_GetWindowSize(int &width, int &height) {
	RECT rect;
	GetClientRect(_hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
}

inline void CALLBACK CInput::_HideCursor() {
	SetClassLong(_hWnd, GCLP_HCURSOR, (LONG)_hCursor);
	_bHideCursor = true;
}

inline void CALLBACK CInput::_ShowCursor() {
	SetClassLong(_hWnd, GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
	_bHideCursor = false;
}