#include"scene.h"

CScene::CScene(Editor *editor) :
	renderer(editor->GetRender())
{
	addObj((IObject*)new CCamera(editor));
}

CScene::~CScene() {
	for (std::size_t i = 0; i < objectList.size(); i++) {
		delete objectList[i];
	}
	///objectList.clear();
}

void CScene::addObj(IObject *obj) {
	objectList.push_back(obj);
}

void CScene::removeObj(IObject *obj) {
	for (std::size_t i = 0; i < objectList.size(); i++) {
		if ((IObject *&)objectList[i] == obj) {
			objectList.erase(objectList.begin() + i);
			break;
		}
	}
}

void CScene::init() {
	for (std::size_t i = 0; i < objectList.size(); i++) {
		objectList[i]->init();
	}
}

void CScene::process() {
	for (std::size_t i = 0; i < objectList.size(); i++) {
		objectList[i]->process();
	}
}

void CScene::render() {
	for (std::size_t i = 0; i < objectList.size(); i++) {
		objectList[i]->render();
	}
}

void CScene::destroy() {
	for (std::size_t i = 0; i < objectList.size(); i++) {
		objectList[i]->destroy();
	}
}