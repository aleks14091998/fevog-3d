#include"Camera.h"

float horizontalAngle, verticalAngle;
int width, height, oldXCursorPos, oldYCursorPos;
bool isChangeMouseCoords = false;

CCamera::CCamera(Editor *editor) :
	input(editor->GetInput()),
	components(new CCameraComponents(editor)),
	upMatrix(glm::vec3(0, 1, 0)),
	speed(0.5f)
{
	SetName((char*)"Camera");
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
}

CCamera::~CCamera() {
	delete components;
}

void CCamera::init() {
	input->_GetWindowSize(width, height);

	components->SetPosition(0.0f, 0.0f, 3.0f);
	components->SetCenterPoint(0, 0, -1);
}

void CCamera::process() {
	// Keyboard control
	bool keyPress;
	components->GetCenterPoint(viewMatrix.x, viewMatrix.y, viewMatrix.z);
	components->GetPosition(pos.x, pos.y, pos.z);
	viewMatrix = glm::normalize(viewMatrix);

	// press -- W --
	input->GetKeyState(valk::E_KEYBOARD_KEY_CODES::KEY_W, keyPress);
	if (keyPress) {
		pos += speed * viewMatrix;
	}
	// press -- S --
	input->GetKeyState(valk::E_KEYBOARD_KEY_CODES::KEY_S, keyPress);
	if (keyPress) {
		pos -= speed * viewMatrix;
	}
	// press -- A --
	input->GetKeyState(valk::E_KEYBOARD_KEY_CODES::KEY_A, keyPress);
	if (keyPress) {
		pos -= speed *  glm::cross(viewMatrix, upMatrix);
	}
	// press -- D --
	input->GetKeyState(valk::E_KEYBOARD_KEY_CODES::KEY_D, keyPress);
	if (keyPress) {
		pos += speed *  glm::cross(viewMatrix, upMatrix);
	}

	components->SetPosition(pos.x, pos.y, pos.z);
	// Mouse control
	input->GetMouseStates(mouse);
	if (mouse.bRightButton) {
		if (isChangeMouseCoords) {
			input->_GetWindowSize(width, height);
			float deltaX = float((width >> 1) - mouse.iX);
			float deltaY = float((height >> 1) - mouse.iY);
			/*
			char message[2000];
			sprintf(message, "... format %d ------- %d\n", (width >> 1) - mouse.iX, (height >> 1) - mouse.iY);
			OutputDebugString((LPCSTR)message);
			*/

			horizontalAngle += deltaX / 6;
			verticalAngle += deltaY / 6;

			if (verticalAngle > 89.0f) {
				verticalAngle = 89.0f;
			}
			if (verticalAngle < -89.0f) {
				verticalAngle = -89.0f;
			}

			viewMatrix.x += sin(horizontalAngle / 180 * 3.141592653);
			viewMatrix.y += tan(verticalAngle / 180 * 3.141592653);
			viewMatrix.z += cos(horizontalAngle / 180 * 3.141592653);

			if (viewMatrix.z == 0) {
				viewMatrix = viewMatrix - glm::vec3(0.0, 0.0, -1.0);
			}
			components->SetCenterPoint(viewMatrix.x, viewMatrix.y, viewMatrix.z);
		}
		else {
			oldXCursorPos = mouse.iX;
			oldYCursorPos = mouse.iY;
			input->_HideCursor();
			isChangeMouseCoords = true;
		}
		input->_SetCursorPos(true);
	}
	else {
		if (isChangeMouseCoords) {
			input->_SetCursorPos(false, oldXCursorPos, oldYCursorPos);
			input->_ShowCursor();
			isChangeMouseCoords = false;
		}
	}
	/**********************MOUSE****************************/


}

void CCamera::render() {
	components->render();
}

void CCamera::destroy() {

}