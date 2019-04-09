#ifndef _SHADER_H
#define _SHADER_H

#include"Resource.h"

class IShader : public IResource {
private:
public:
	virtual void Apply() = 0;
};

#endif
