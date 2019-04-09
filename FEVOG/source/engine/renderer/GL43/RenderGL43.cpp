#include"core/core.h"
#include"RenderGL43.h" 
#include"GeometryBufferGL43.h"
#include"ShaderGL43.h"

uint vaoGrid;
ShaderGL43 *shaderGrid;

CRenderGL43::CRenderGL43(CCore *core) :
	core(core),
	_c_fFov(100.f), _c_fZNear(0.1f), _c_fZFar(100.f)
{
	core->GetMessageProc()->addFunc(&s_MessageProc, (void*)this);

	Projection = glm::perspective(45.0f, 4.0f / 3.0f, _c_fZNear, _c_fZFar);
}

CRenderGL43::~CRenderGL43() {
	core->GetMessageProc()->removeFunc(&s_MessageProc, (void*)this);
}

void CRenderGL43::_MessageProc(const TWinMessage &stMsg) {
	if (stMsg.uiMsgType == WMT_SIZE || stMsg.uiMsgType == WMT_RESTORED)
	{
		_OnResize();
	}

}

void CRenderGL43::Init() {
	_OnResize();
	InitGrid();
}

void CRenderGL43::StartFrame() {
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	gl::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	gl::Enable(gl::DEPTH_TEST);

	/*
	gl::Enable(gl::BLEND); //Прозрачность
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	gl::Hint(gl::POLYGON_SMOOTH_HINT, gl::FASTEST);
	gl::Enable(gl::POLYGON_SMOOTH);*/

///	gl::DepthFunc(gl::LESS);
///	gl::Enable(gl::CULL_FACE); // отсечение
	
}

void CRenderGL43::EndFrame() {	
	DrawGrid();
}

void CRenderGL43::_OnResize() {
	int32 l, r, t, b;
	core->GetWindow()->GetClientRect(l, r, t, b);

	int width = r - l;
	int height = b - t;

	if (height == 0) {
		height = 1; 
	}
	gl::Viewport(0, 0, width, height);
	Projection = glm::perspective(45.0f, (float)width / height, _c_fZNear, _c_fZFar);
}



void CALLBACK CRenderGL43::s_MessageProc(void *pParametr, const TWinMessage &stMsg) {
	((CRenderGL43*)pParametr)->_MessageProc(stMsg);
}

void CRenderGL43::SetDefaultStates() {

}

void CRenderGL43::Draw(valk::render::ResourceList *list) {
	ShaderGL43 *shader = (ShaderGL43*)list->Shader;
	if (list->Shader) {
		shader->Apply();
		/********************transform************************/
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(
			list->transform.translate[0],
			list->transform.translate[1],
			list->transform.translate[2]));

		transform = glm::rotate(transform, list->transform.rotateAngle, glm::vec3(
			list->transform.rotate[0],
			list->transform.rotate[1],
			list->transform.rotate[2]));

		transform = glm::scale(transform, glm::vec3(
			list->transform.scale[0],
			list->transform.scale[1],
			list->transform.scale[2]));
		/********************transform************************/
		shader->setMat4("model", transform);
		shader->setMat4("projection", Projection);
		shader->setMat4("view", View);

		if (list->Mesh) {
			list->Material->BindTextures(shader);
			list->Mesh->bind();
		}
	}
}

void CRenderGL43::DrawCamera(valk::render::CameraParameters *list) {
	View = glm::lookAt(
		glm::vec3(list->Position[0], list->Position[1], list->Position[2]),
		glm::vec3(list->Position[0] + list->CenterPoint[0], list->Position[1] + list->CenterPoint[1], list->Position[2] + list->CenterPoint[2]),
		glm::vec3(0, 1, 0)
	);
}

// Grid

void CRenderGL43::InitGrid() {
	float y = 0.0f;
	float verticesG[56 * 3] = {
			-6, y, 1, 6, y, 1, -6, y, 2, 6, y, 2, -6, y, 3,	6, y, 3, -6, y, 4,	6, y, 4, -6, y, 5,	6, y, 5,
			-6, y, 6, 6, y, 6, -6, y, 7, 6, y, 7, -6, y, 8,	6, y, 8, -6, y, 9,	6, y, 9, -6, y, 10,	6, y, 10,
			-6, y, 11, 6, y, 11, -6, y, 12, 6, y, 12, -6, y, 13, 6, y, 13, -6, y, 14, 6, y, 14, -6, y, 1, 
			-6, y, 14, -5, y, 1, -5, y, 14, -4, y, 1, -4, y, 14, -3, y, 1, -3, y, 14, -2, y, 1, -2, y, 14,
			 -1, y, 1, -1, y, 14, 0, y, 1, 0, y, 14, 1, y, 1, 1, y, 14, 2, y, 1, 2, y, 14, 3, y, 1,
			3, y, 14, 4, y, 1, 	4, y, 14, 4, y, 1, 4, y, 14, 5, y, 1, 5, y, 14, 6, y, 1, 6, y, 14
	};

	valk::uint vboG;
	gl::GenVertexArrays(1, &vaoGrid);
	gl::GenBuffers(1, &vboG);
	gl::BindVertexArray(vaoGrid);
	gl::BindBuffer(gl::ARRAY_BUFFER, vboG);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(verticesG), verticesG, gl::STATIC_DRAW);


	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 3 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);

	shaderGrid = new ShaderGL43((char*)"resource/shaders/gl43/grid");
}
void CRenderGL43::DrawGrid() {
	shaderGrid->Apply();
	shaderGrid->setMat4("projection", Projection);
	shaderGrid->setMat4("view", View);

	gl::BindVertexArray(vaoGrid);
	gl::DrawArrays(gl::LINES, 0, 56);
	gl::BindVertexArray(0);
}
