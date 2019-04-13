#ifndef _PLATFORMAPI_H
#define _PLATFORMAPI_H

#include"FEV_Eng.h"
#include"FuncDelegate.h"

using namespace valk;

// {E9A45C97-722A-4ED1-875D-A8D4277E7050}
static const GUID IID_IMainWindow =
{ 0xe9a45c97, 0x722a, 0x4ed1, { 0x87, 0x5d, 0xa8, 0xd4, 0x27, 0x7e, 0x70, 0x50 } };

namespace valk_platform {
	const enum MESSAGE_INFO_TYPE {
		EMIT_ERROR = 1,
		EMIT_WARNING = 2,
		EMIT_INFO = 3,
		EMIT_QUESTION = 4
	};	
	
	const enum MESSAGE_DIALOG_TYPE {
		EMDT_OK = 1,
		EMDT_OK_CANCEL = 2,
		EMDT_RETRY_CANCEL = 3,
		EMDT_YES_NO = 4,
		EMDT_YES_NO_CANCEL = 5,
		EMDT_ABORT_RETRY_IGNORE = 6
	};
}

using namespace valk_platform;

class IMainWindow
{
public:
	virtual HRESULT InitWindow(ProcDelegate *delMainLoop, MsgProcDelegate *delMsgProc) = 0;
	virtual HRESULT ConfigureWindow(uint uiResX, uint uiResY, bool bFScreen) = 0;
	virtual HRESULT SetTitle(const char *pcTxt) = 0;
	virtual	HRESULT SendMessage(const TWinMessage &stMsg) = 0;
	virtual HRESULT BeginMainLoop() = 0;
	virtual HRESULT KillWindow() = 0;
	virtual HRESULT Destroy() = 0;
	virtual HRESULT GetWindowHandle(WindowHandle& result) = 0;
	virtual HRESULT GetClientRect(int32 &l, int32 &r, int32 &t, int32 &b) = 0;
	virtual HRESULT InitRender() = 0;
	virtual void UpdateObjectList(std::vector<IObject*> objList) = 0;

};

struct TSysTimeAndDate
{
	uint16 ui16Year;
	uint16 ui16Month;
	uint16 ui16Day;
	uint16 ui16Hour;
	uint16 ui16Minute;
	uint16 ui16Second;
	uint16 ui16Milliseconds;
};

#ifdef PLATFORM_WINDOWS
TWinMessage WinAPIMsgToEngMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
void EngMsgToWinAPIMsg(const TWinMessage &msg, UINT &Msg, WPARAM &wParam, LPARAM &lParam);
#endif

uint32 GetPerfTimer();
void GetLocalTimaAndDate(TSysTimeAndDate &time);
void GetDisplaySize(uint &width, uint &height);
void ShowModalUserAlert(valk::WindowHandle hwnd, const char *pcTxt, const char *pcCaption, const MESSAGE_INFO_TYPE messageType = EMIT_INFO, const MESSAGE_DIALOG_TYPE typeDialog = EMDT_OK, void(CALLBACK *okHandler)() = NULL);
void Terminate();

#endif