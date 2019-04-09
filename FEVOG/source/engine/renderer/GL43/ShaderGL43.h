#ifndef _SHADER_GL43_H
#define _SHADER_GL43_H

#include<string>
#include<fstream>
#include<sstream>

#include"GL43.h"

class ShaderGL43 : public IShader{
public:
	unsigned int ID;

	ShaderGL43(const char* fileName) {
		/********************************************************/
		std::string  fileNameVertex = fileName + (std::string)".vert";
		std::string  fileNameFrag = fileName + (std::string)".frag";
		char* geometryPath = NULL;
		/********************************************************/
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vShaderFile.open(fileNameVertex);
			fShaderFile.open(fileNameFrag);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			if (geometryPath != NULL) {
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}

		catch (std::ifstream::failure e) {

		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		vertex = gl::CreateShader(gl::VERTEX_SHADER);
		gl::ShaderSource(vertex, 1, &vShaderCode, NULL);
		gl::CompileShader(vertex);
	//	checkCompileErrors(vertex, "VERTEX");

		fragment = gl::CreateShader(gl::FRAGMENT_SHADER);
		gl::ShaderSource(fragment, 1, &fShaderCode, NULL);
		gl::CompileShader(fragment);
	//	checkCompileErrors(fragment, "FRAGMENT");

		unsigned int geometry;
		if (geometryPath != NULL)
		{
			const char * gShaderCode = geometryCode.c_str();
			geometry = gl::CreateShader(gl::GEOMETRY_SHADER);
			gl::ShaderSource(geometry, 1, &gShaderCode, NULL);
			gl::CompileShader(geometry);
		//	checkCompileErrors(geometry, "GEOMETRY");
		}

		ID = gl::CreateProgram();
		gl::AttachShader(ID, vertex);
		gl::AttachShader(ID, fragment);

		if (geometryPath != NULL)
			gl::AttachShader(ID, geometry);
		gl::LinkProgram(ID);
	//	checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		gl::DeleteShader(vertex);
		gl::DeleteShader(fragment);
		if (geometryPath != NULL) {
			gl::DeleteShader(geometry);
		}
	}


	void Apply() {
		gl::UseProgram(ID);
	}

	void setBool(const std::string &name, bool value) const
	{
		gl::Uniform1i(gl::GetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setUInt(const std::string &name, GLuint value) const
	{
		gl::Uniform1i(gl::GetUniformLocation(ID, name.c_str()), value);
	}

	void setInt(const std::string &name, int value) const
	{
		gl::Uniform1i(gl::GetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string &name, float value) const
	{
		gl::Uniform1f(gl::GetUniformLocation(ID, name.c_str()), value);
	}

	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		gl::Uniform2fv(gl::GetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		gl::Uniform2f(gl::GetUniformLocation(ID, name.c_str()), x, y);
	}

	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		gl::Uniform3fv(gl::GetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		gl::Uniform3f(gl::GetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		gl::Uniform4fv(gl::GetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		gl::Uniform4f(gl::GetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		gl::UniformMatrix2fv(gl::GetUniformLocation(ID, name.c_str()), 1, gl::FALSE_, &mat[0][0]);
	}

	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		gl::UniformMatrix3fv(gl::GetUniformLocation(ID, name.c_str()), 1, gl::FALSE_, &mat[0][0]);
	}

	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		gl::UniformMatrix4fv(gl::GetUniformLocation(ID, name.c_str()), 1, gl::FALSE_, &mat[0][0]);
	}




	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			gl::GetShaderiv(shader, gl::COMPILE_STATUS, &success);
			if (!success) {
				gl::GetShaderInfoLog(shader, 1024, NULL, infoLog);
			}
		}
		else
		{
			gl::GetProgramiv(shader, gl::LINK_STATUS, &success);
			if (!success) {
				gl::GetProgramInfoLog(shader, 1024, NULL, infoLog);
			}
		}
	}
};


#endif
