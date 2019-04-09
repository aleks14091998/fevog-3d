#ifndef _GEOMETRY_BUFFER_H
#define _GEOMETRY_BUFFER_H

#include"Resource.h"

class IGeometryBuffer : public IResource {
private:
public:
	virtual void setup() = 0;
	virtual void bind() = 0;
};

#endif