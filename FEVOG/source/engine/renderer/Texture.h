#ifndef _TEXTURE_H
#define _TEXTURE_H

#include"Resource.h"
#include"Shader.h"

enum E_TEXTURE_TYPE {
	DEFAULT = 1,
	NORMAL = 1
};

class ITexture : public IResource {
private:
	E_TEXTURE_TYPE type;
public:
	virtual void BindTexture(IShader *shader) = 0;
	inline void SetTexType(E_TEXTURE_TYPE type) {
		this->type = type; 
	}
};

#endif
