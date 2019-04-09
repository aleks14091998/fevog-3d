#ifndef _WINDOWMANAGER_H
#define _WINDOWMANAGER_H

#include"common.h"

#ifdef PLATFORM_WINDOWS //WINAPI
#include"platform/windows/MainWindow.h"
#include"platform/windows/Input.h"
#endif // !PLATFORM_WINDOWS

class WindowManager : public IWindowManager {
private:
	CCore				*core;
	IMainWindow			*mainWindow;
	CInput				*_cInput;

public:
	WindowManager(CCore	*core);
	~WindowManager();


	HRESULT Init(const char *name, ProcDelegate *pDelMainLoop, MsgProcDelegate *pDelMsgProc);
	HRESULT Configure(uint uiResX, uint uiResY, bool bFScreens);
	HRESULT GetInput(IInput *&input);
	HRESULT StartLoop();
	IMainWindow* GetWindow();
	void KillMainWindow();
	void UpdateObjectList(std::vector<IObject*> objList);

	IJTS_BASE_IMPLEMENTATION(IWindowManager);
};

#endif // !_CORE_H
