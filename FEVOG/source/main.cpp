#include"FEV_Eng.h"
#include"editor/editor.h"

valk::Engine *engine = NULL;
Editor *editor = NULL;

void CALLBACK init(void *parametr) {
	editor->init();
}

void CALLBACK process(void *parametr) {
	editor->process();
}

void CALLBACK render(void *parametr) {
	editor->render();
}

void CALLBACK destroy(void *parametr) {
	editor->destroy();
}

#ifdef PLATFORM_WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	if (GetEngine(engine)) {
		engine->InitRenderSystem(valk::ERA_OGL43);
		if (editor == NULL) {
			editor = new Editor();
		}

		engine->AddProcedure(valk::EPT_INIT, &init);
		engine->AddProcedure(valk::EPT_PROCESS, &process);
		engine->AddProcedure(valk::EPT_RENDER, &render);
		engine->AddProcedure(valk::EPT_DESTRPY, &destroy);
		engine->InitEngine(800,700, "File Editor for Valkyrie", valk::EIF_DEFAULT);
		DestroyEngine();

		return 0;
	} else {
		return 1;
	}
}
#endif