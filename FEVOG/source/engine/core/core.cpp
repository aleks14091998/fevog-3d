#include"core.h"

#include"managers/WindowManager.h"
#include"managers/ResourceManager.h"
#include"renderer/GL43/RenderGL43.h"

CCore *core = NULL;

bool GetEngine(Engine *&engine) {
	if (!core) {
		core = new CCore();
		engine = (Engine *&)core;
		return true;
	}
	else {
		engine = (Engine *&)core;
		return false;
	}
}

void DestroyEngine() {
	if (core) {
		delete core;
		core = NULL;
	}
}

CCore::CCore() : doExit(false), _uiProcInterval(25), windowManager(NULL) {
	delLoop.addFunc(&s_mainLoop, this);
	delProc.addFunc(&s_messageProc, this);

	windowManager = new WindowManager(this);
}

CCore::~CCore() {
	if (_cRender) {
		delete _cRender;
	}
	if (windowManager) {
		delete windowManager;
	}
}

void CCore::mainLoop() {
	if (doExit) {
		windowManager->KillMainWindow();
		return;
	}

	uint64 time = GetPerfTimer() / 1000;
	uint64 time_delta = time - _ui32TimeOld;
	uint fix_time = (uint)time_delta;

	while (fix_time >= _uiProcInterval)
	{
		if (!delProcess.IsNull())
			delProcess.Invoke();
		fix_time -= _uiProcInterval;
	}

	if (fix_time > 0) {
		_ui32TimeOld = time - fix_time;
	}
	_cRender->StartFrame();
	delRender.Invoke();
	_cRender->EndFrame();

}

void CCore::messageProc(const TWinMessage &stMsg) {
	switch (stMsg.uiMsgType) {
		case WMT_REDRAW:
			delLoop.Invoke();
			break;
		case WMT_CLOSE:
			doExit = true;
			break;

		case WMT_DESTROY:
			if (!delDestroy.IsNull()) {
				delDestroy.Invoke();
			}
			break;
	}
}

void CCore::s_mainLoop(void *parametr) {
	((CCore*)parametr)->mainLoop();
}

void CCore::s_messageProc(void *parametr, const TWinMessage &stMsg) {
	((CCore*)parametr)->messageProc(stMsg);
}

HRESULT CALLBACK CCore::InitEngine(uint uiResX, uint uiResY, const char* pcApplicationName, E_ENGINE_INIT_FLAGS eInitFlags) {
	if (!(eInitFlags & EIF_NO_LOGGING))	{
		TSysTimeAndDate time;
		GetLocalTimaAndDate(time);
	}

	if (SUCCEEDED(windowManager->Init(pcApplicationName, &delLoop, &delProc))) {
		if (!_cRender) {
			return E_ABORT;
		}
		_cRender->Init();

		if ((eInitFlags & EIF_NATIVE_RESOLUTION) && (eInitFlags & EIF_FULL_SCREEN)) {
			GetDisplaySize(uiResX, uiResY);
		}

		if (FAILED(windowManager->Configure(uiResX, uiResY, eInitFlags & EIF_FULL_SCREEN))) {
			return E_ABORT;
		}

		if (!delInit.IsNull()) {
			delInit.Invoke();
		}

		_ui32TimeOld = GetPerfTimer() / 1000 - _uiProcInterval;

		return windowManager->StartLoop();
	}
	else {
		return E_ABORT;
	}
}

HRESULT CALLBACK CCore::QuitEngine() {
	doExit = true;
	return S_OK;
}

HRESULT CALLBACK CCore::SetProcessInterval(uint processInterval) {
	_uiProcInterval = processInterval;
	return S_OK;
}

HRESULT CALLBACK CCore::AddProcedure(E_ENGINE_PROCEDURE_TYPE eProcType, void (CALLBACK *proc)(void *parametr), void *parametr) {
	switch (eProcType)
	{
	case EPT_PROCESS:
		delProcess.addFunc(proc, parametr);
		break;
	case EPT_RENDER:
		delRender.addFunc(proc, parametr);
		break;
	case EPT_INIT:
		delInit.addFunc(proc, parametr);
		break;
	case EPT_DESTRPY:
		delDestroy.addFunc(proc, parametr);
		break;
	default: return E_INVALIDARG;
	}
	return S_OK;
}

HRESULT CALLBACK CCore::RemoveProcedure(E_ENGINE_PROCEDURE_TYPE eProcType, void (CALLBACK *pProc)(void *pParametr), void *pParametr) {
	switch (eProcType)
	{
	case EPT_PROCESS:
		delProcess.removeFunc(pProc, pParametr);
		break;
	case EPT_RENDER:
		delRender.removeFunc(pProc, pParametr);
		break;
	case EPT_INIT:
		delInit.removeFunc(pProc, pParametr);
		break;
	case EPT_DESTRPY:
		delDestroy.removeFunc(pProc, pParametr);
		break;
	default: return E_INVALIDARG;
	}

	return S_OK;
}

HRESULT CALLBACK CCore::AddToLog(const char *pcTxt, bool bError) {
	//here
	return S_OK;
}

IMainWindow* CCore::GetWindow() const {
	return windowManager->GetWindow();
}

HRESULT CALLBACK CCore::GetWinManager(IWindowManager *&wm) {
	if (!windowManager) {
		return E_ABORT;
	}
	else {
		wm = (IWindowManager *&)windowManager;
		return S_OK;
	}
}

HRESULT CALLBACK CCore::GetRender(IRenderAPI *&render) {
	if (!_cRender) {
		return E_ABORT;
	}
	else {
		render = (IRenderAPI *&)_cRender;
		return S_OK;
	}
}

HRESULT CALLBACK CCore::InitRenderSystem(E_RENDER_API RenderType) {
	if (!_cRender) {
#ifndef PLATFORM_WINDOWS
		if (RenderType == valk::ERA_DX9 || RenderType == valk::ERA_DX11) {
			RenderType = ERA_OGL43;
		}
#endif
		CResourceManager::SetRenderAPI(RenderType);
		switch (RenderType) {
			case ERA_OGL43:
				_cRender = new CRenderGL43(this);
				return S_OK;
			case ERA_DX9:
				exit(0);
				return S_OK;
			case ERA_DX11:
				exit(0);
				return S_OK;
			}
	}
	return E_ABORT;
}