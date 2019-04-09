#include"editor.h"
#include"scene.h"
//test
CScene *scene;
// test

Editor::Editor() : Input(NULL), windowManager(NULL), Render(NULL){
	/// get param engin
	GetEngine(engine);
	engine->GetWinManager(windowManager);
	engine->GetRender(Render);
	windowManager->GetInput(Input);

	//test
	scene = new CScene(this);
	for (int i = 0; i < 10; i++) {
		IObject *obj1 = (IObject*)new CObject(this);
		obj1->SetName((char*)"Cube");
		scene->addObj(obj1);
	}
	//test
	//test
	//test
}
Editor::~Editor() {
	
}

void Editor::init() {
	windowManager->UpdateObjectList(scene->GetObjectList());
	//test
	scene->init();
	//test
}

void Editor::process() {
	/** TEST */
	scene->process();
	/** TEST */
	
}

void Editor::render() {
	//test
	scene->render();
	//test
}

void Editor::destroy() {

	delete this;
}