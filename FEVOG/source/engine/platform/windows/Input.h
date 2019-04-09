#ifndef _INPUT_H
#define _INPUT_H

#include"core/core.h"
#include "common.h"

class CCore;

class CInput : public IInput {
private:
	CCore *core;

	// WINAPI
	HCURSOR _hCursor;
	HWND _hWnd;

	bool _bExclusive;
	bool _bHideCursor;
	bool _bFocused;
	bool _bCurBeyond;

	bool		 _bIsTxtInput;
	char		*_pcBuffer;
	string		 _clInputTxt;
	uint		 _uiBufSize;

	bool		 _abKeys[256];
	MouseStates _stMsts;
	int			 _uiOldMouseX, _uiOldMouseY;

	void		 _fClipCursor();
	void		 _GetInputClientRect(int32 &l, int32 &r, int32 &t, int32 &b);
	void		 MessageProc(const TWinMessage &stMsg);
	void		 Loop();
	void		 Process();

	static void CALLBACK s_MessageProc(void *pParametr, const TWinMessage &stMsg);
	static void CALLBACK s_Process(void *pParametr);
	static void CALLBACK s_Loop(void *pParametr);

public:
	CInput(CCore *core);
	~CInput();

	HRESULT CALLBACK Init(IMainWindow* MainWindow);
	HRESULT CALLBACK Configure(E_INPUT_CONFIGURATION_FLAGS flag);
	HRESULT CALLBACK GetMouseStates(MouseStates &stMs) const;
	HRESULT CALLBACK GetKeyState(E_KEYBOARD_KEY_CODES key, bool &state) const;
	HRESULT CALLBACK BeginTextInput(char *buffer, uint bfferSize);
	HRESULT CALLBACK EndTextInput();
	void CALLBACK _SetCursorPos(bool toCenter, int x, int y);
	void CALLBACK _GetWindowSize(int &width, int &height);
	void CALLBACK _HideCursor();
	void CALLBACK _ShowCursor();

	IJTS_BASE_IMPLEMENTATION(IInput);
};

#endif //_INPUT_H