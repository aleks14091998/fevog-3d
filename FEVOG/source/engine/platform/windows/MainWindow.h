#ifndef _WINAPI_H
#define _WINAPI_H

#include"common.h"

class CMainWindow : public IMainWindow {
private:
	Engine				*engine;
	HINSTANCE			_hInst;
	HWND				_hWnd, _hRenderWnd, _hObjectListWnd, _hObjectParamsWnd, _hObjList;
	HWND				split1, split2, split3;
	bool				_bSplit1, _bSplit2, _bSplit3;
	HDC					_hDC;
	ProcDelegate		*delMainLoop;
	MsgProcDelegate		*delMessageProc;
	HGLRC				_hRC;
	bool				_bIsLooping;

	const uint _c_uiMSAASamples;
	const bool _c_bVSync;
	
	int	WINAPI			_wWinMain(HINSTANCE hInstance);
	static LRESULT CALLBACK s_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK mv_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ol_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HRESULT InitRender();
	HRESULT CreateChildWindow();
public:
	CMainWindow(Engine *engine);
	~CMainWindow();

	HRESULT InitWindow(ProcDelegate *delMainLoop, MsgProcDelegate *delMsgProc);
	HRESULT ConfigureWindow(uint uiResX, uint uiResY, bool bFScreen);
	HRESULT SendMessage(const TWinMessage &stMsg);
	HRESULT SetTitle(const char *pcTxt);
	HRESULT BeginMainLoop();
	HRESULT KillWindow();
	HRESULT Destroy();
	HRESULT GetWindowHandle(WindowHandle& result);
	HRESULT GetRenderHandle(WindowHandle& result);
	HRESULT GetClientRect(int32 &l, int32 &r, int32 &t, int32 &b);
	int GetWindowDataInPercent(int percent, char type);
	void UpdateObjectList(std::vector<IObject*> objList);
	void EventSplit(MSG *msg);

};

#endif // !_WINAPI_H