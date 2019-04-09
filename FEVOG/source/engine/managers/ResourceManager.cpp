#include"ResourceManager.h"

valk::E_RENDER_API CResourceManager::render_api = valk::ERA_OGL43;
std::vector<IShader*> CResourceManager::shaders;
std::vector<IGeometryBuffer*> CResourceManager::meshes;
std::vector<ITexture*> CResourceManager::textures;

bool create_shader(IShader *&shader, char* fileName);
bool create_mesh(IGeometryBuffer *&mesh, char* fileName);
bool create_texture(ITexture *&texture, char* fileName, E_TEXTURE_TYPE type);

// Add
bool CResourceManager::AddMesh(IGeometryBuffer*& mesh, char* fileName) {
	if (!IsExistResource(fileName)) {
		create_mesh(mesh, fileName);
		meshes.push_back(mesh);
		return true;
	}
	if (GetMesh(mesh, fileName)) {
		return true;
	}
	return false;
}

bool CResourceManager::AddShader(IShader*& shader, char* fileName) {
	char way[255] = "resource/shaders/gl43/";
	strcat(way, fileName);
	if (!IsExistResource(way)) {
		create_shader(shader, way);
		shaders.push_back(shader);
		return true;
	}
	if (GetShader(shader, way)) {
		return true;
	}
	return false;
}

bool CResourceManager::AddTexture(ITexture *&texture, char* fileName, E_TEXTURE_TYPE type) {
	if (!IsExistResource(fileName)) {
		create_texture(texture, fileName, type);
		textures.push_back(texture);
		return true;
	}
	if (GetTexture(texture, fileName)) {
		return true;
	}
	return false;
}

// Get
bool CResourceManager::GetShader(IShader*& shader, char* fileName) {
	for (int i = 0; i < shaders.size(); i++) {
		if (strcmp(shaders[i]->GetName(), fileName) == 0) {
			shader = shaders[i];
			return true;
		}
	}
	return false;
}

bool CResourceManager::GetMesh(IGeometryBuffer *&mesh, char* fileName) {
	for (int i = 0; i < meshes.size(); i++) {
		if (strcmp(meshes[i]->GetName(), fileName) == 0) {
			mesh = meshes[i];
			return true;
		}
	}
	return false;
}

bool CResourceManager::GetTexture(ITexture *&texture, char* fileName) {
	for (int i = 0; i < textures.size(); i++) {
		if (strcmp(textures[i]->GetName(), fileName) == 0) {
			texture = textures[i];
			return true;
		}
	}
	return false;
}

// Remove
/*
bool CResourceManager::RemoveMesh(char* fileName) {
	for (int i = 0; i < meshes.size(); i++) {
		if (meshes[i]->GetName() == fileName) {
			meshes.erase(meshes.begin() + i);
			return true;
		}
	}
	return false;
}

bool CResourceManager::RemoveTexture(char* fileName) {
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i]->GetName() == fileName) {
			textures.erase(textures.begin() + i);
			return true;
		}
	}
	return false;
}

bool CResourceManager::RemoveMaterial(char* fileName) {
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i]->GetName() == fileName) {
			materials.erase(materials.begin() + i);
			return true;
		}
	}
	return false;
}
*/
bool CResourceManager::IsExistResource(char* fileName) {
	for (int i = 0; i < shaders.size(); i++) {
		if (strcmp(shaders[i]->GetName(), fileName) == 0) {
			return true;
		}
	}

	for (int i = 0; i < meshes.size(); i++) {
		if (strcmp(meshes[i]->GetName(), fileName) == 0) {
			return true;
		}
	}
	
	for (int i = 0; i < textures.size(); i++) {
		if (strcmp(textures[i]->GetName(), fileName) == 0) {
			return true;
		}
	}
	/*
	for (int i = 0; i < materials.size(); i++) {
		if (strcmp(materials[i]->GetName(), fileName) == 0) {
			return true;
		}
	}*/

	return false;
}

void CResourceManager::SetRenderAPI(valk::E_RENDER_API api) {
	render_api = api;
}

valk::E_RENDER_API CResourceManager::GetRenderAPI() {
	return render_api;
}

bool create_shader(IShader *&shader, char* fileName) {
	switch (CResourceManager::GetRenderAPI()){
	case valk::ERA_OGL43:
		shader = new RenderComponent<valk::ERA_OGL43>::Shader(fileName);
		break;
	case valk::ERA_DX11:
		shader = new RenderComponent<valk::ERA_DX11>::Shader(fileName);
		break;
	default:
		return false;
	}
	shader->SetName(fileName);
	return true;
}

bool create_mesh(IGeometryBuffer *&mesh, char* fileName) {
	switch (CResourceManager::GetRenderAPI()) {
	case valk::ERA_OGL43:
		mesh = new RenderComponent<valk::ERA_OGL43>::Mesh(fileName);
		break;
	case valk::ERA_DX11:
		mesh = new RenderComponent<valk::ERA_DX11>::Mesh(fileName);
		break;
	default:
		return false;
	}
	mesh->SetName(fileName);
	return true;
}

bool create_texture(ITexture *&texture, char* fileName, E_TEXTURE_TYPE type) {
	switch (CResourceManager::GetRenderAPI()) {
	case valk::ERA_OGL43:
		texture = new RenderComponent<valk::ERA_OGL43>::Texture(fileName, type);
		break;
	case valk::ERA_DX11:
		texture = new RenderComponent<valk::ERA_DX11>::Texture(fileName, type);
		break;
	default:
		return false;
	}
	texture->SetName(fileName);
	return true;
}