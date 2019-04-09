#ifndef _GEOMETRY_BUFFER_GL43_H
#define _GEOMETRY_BUFFER_GL43_H

#include"FEV_Eng.h"
#include"renderer/GeometryBuffer.h"
#include"GL43.h"

class CGeometryBufferGL43 : public IGeometryBuffer {
private:
	valk::uint VAO, VBO;
	//float vertices[];		/// изменить

	
public:
	CGeometryBufferGL43(char* fileName);
	~CGeometryBufferGL43();

	void setup();
	void bind();
};

#endif