#include"WindowManager.h"

WindowManager::WindowManager(CCore	*core) : core(core) {
	mainWindow = new CMainWindow(core);
	_cInput = new CInput(core);
}

WindowManager::~WindowManager() {
	delete _cInput;
	mainWindow->Destroy(); // delete mainWindow
}

void WindowManager::KillMainWindow() {
	mainWindow->KillWindow();
}

HRESULT WindowManager::Init(const char *name, ProcDelegate *pDelMainLoop, MsgProcDelegate *pDelMsgProc) {
	mainWindow->InitWindow(pDelMainLoop, pDelMsgProc);
	_cInput->Init(mainWindow);
	mainWindow->SetTitle(name);
	return S_OK;
}

HRESULT WindowManager::Configure(uint uiResX, uint uiResY, bool bFScreen) {
	mainWindow->ConfigureWindow(uiResX, uiResY, bFScreen);
	_cInput->Configure((valk::E_INPUT_CONFIGURATION_FLAGS)(valk::ICF_DEFAULT));
	return S_OK;
}

HRESULT WindowManager::StartLoop() {
	return mainWindow->BeginMainLoop();
}

IMainWindow* WindowManager::GetWindow() {
	return (IMainWindow*)mainWindow;
}

HRESULT WindowManager::GetInput(IInput *&input) {
	if (!_cInput) {
		return E_ABORT;
	}
	else {
		input = (IInput *&)_cInput;
		return S_OK;
	}
}

void WindowManager::UpdateObjectList(std::vector<IObject*> objList) {
	mainWindow->UpdateObjectList(objList);
}