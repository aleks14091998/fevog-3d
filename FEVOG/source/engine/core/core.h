#ifndef _CORE_H
#define _CORE_H

#include"common.h"

class WindowManager;
class CCore : public Engine {
private:
	ProcDelegate		delInit,
						delProcess,
						delRender,
						delLoop,
						delDestroy;	
	MsgProcDelegate		delProc;

	bool doExit;
	std::fstream		logFile;
	uint				_uiProcInterval;
	uint32				_ui32TimeOld;

	WindowManager		*windowManager;
	IRenderAPI			*_cRender;

	void mainLoop();
	void messageProc(const TWinMessage &stMsg);

	static void CALLBACK s_mainLoop(void *parametr);
	static void CALLBACK s_messageProc(void *parametr, const TWinMessage &stMsg);

public:
	CCore();
	~CCore();

	inline	MsgProcDelegate* GetMessageProc() { return &delProc; }
	inline	ProcDelegate* GetLoopProc() { return &delLoop; }

	HRESULT CALLBACK InitEngine(uint uiResX, uint uiResY, const char* pcApplicationName, E_ENGINE_INIT_FLAGS eInitFlags);
	HRESULT CALLBACK SetProcessInterval(uint uiProcessInterval);
	HRESULT CALLBACK AddProcedure(E_ENGINE_PROCEDURE_TYPE eProcType, void (CALLBACK *proc)(void *parametr), void *parametr);
	HRESULT CALLBACK RemoveProcedure(E_ENGINE_PROCEDURE_TYPE eProcType, void (CALLBACK *proc)(void *parametr), void *parametr);
	HRESULT CALLBACK QuitEngine();
	HRESULT CALLBACK AddToLog(const char *pcTxt, bool bError = false);
	HRESULT CALLBACK GetWinManager(IWindowManager *&wm);
	HRESULT CALLBACK GetRender(IRenderAPI *&pRender);
	HRESULT CALLBACK InitRenderSystem(E_RENDER_API RenderType);
	IMainWindow* GetWindow() const;
	//void CALLBACK UpdateObjectList(std::vector<IObject*> objList);

	IJTS_BASE_IMPLEMENTATION(Engine);
};

#endif // !_CORE_H
