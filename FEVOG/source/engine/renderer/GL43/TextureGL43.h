#ifndef _TEXTURE_GL43_H
#define _TEXTURE_GL43_H

#include"renderer/Texture.h"
#include"GL43.h"

class TextureGL43 : public ITexture {
private:
	unsigned int id;
public:
	TextureGL43(char* fileName, E_TEXTURE_TYPE type);
	~TextureGL43();

	void BindTexture(IShader *shader);
	void UnbindTexture();
};

#endif
