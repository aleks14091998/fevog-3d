#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

#include<vector>

//test
#include"renderer/GL43/GeometryBufferGL43.h"
#include"renderer/GL43/ShaderGL43.h"
#include"renderer/GL43/TextureGL43.h"
//test


// template -- RENDER API --
template <valk::E_RENDER_API>
struct RenderComponent;

template<>
struct RenderComponent<valk::ERA_OGL43> {
	typedef CGeometryBufferGL43 Mesh;
	typedef ShaderGL43 Shader;
	typedef TextureGL43 Texture;
};

template<>
struct RenderComponent<valk::ERA_DX11> {
	typedef CGeometryBufferGL43 Mesh;
	typedef ShaderGL43 Shader;
	typedef TextureGL43 Texture;
};

class CResourceManager {
private:
	static valk::E_RENDER_API render_api;

	static std::vector<IShader*> shaders;
	static std::vector<IGeometryBuffer*> meshes;
	static std::vector<ITexture*> textures;
//	std::vector<IResource*> materials;

	//void create_resource(IResource *resource, char* fileName, RESOURCE_TYPE type);

public:
	static bool AddShader(IShader *&shader, char* fileName);
	static bool AddMesh(IGeometryBuffer *&mesh, char* fileName);
	static bool AddTexture(ITexture *&texture, char* fileName, E_TEXTURE_TYPE type);

	static bool GetShader(IShader*& shader, char* fileName);//-----------
	static bool GetMesh(IGeometryBuffer *&mesh, char* fileName);
	static bool GetTexture(ITexture *&texture, char* fileName);
	
//	bool RemoveMesh(char* fileName);
//	bool RemoveTexture(char* fileName);
//	bool RemoveMaterial(char* fileName);

	static bool IsExistResource(char* fileName);
	static void SetRenderAPI(valk::E_RENDER_API api);
	static valk::E_RENDER_API GetRenderAPI();
};

#endif