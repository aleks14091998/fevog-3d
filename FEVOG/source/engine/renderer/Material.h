#ifndef _MATERIAL_H
#define _MATERIAL_H

#include"Texture.h"

//#include"Shader.h"

class Material : public IResource {
private:
	ITexture *texture;
public:
	Material();
	~Material();
	void SetTexture(char* filename, E_TEXTURE_TYPE type);
	ITexture* GetTextureDefault();

	void BindTextures(IShader *shader);
	void UnbindTextures();
};

#endif
