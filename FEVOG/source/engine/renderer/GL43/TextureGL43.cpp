#include"TextureGL43.h"
#include"ShaderGL43.h"

#define STB_IMAGE_IMPLEMENTATION
#include"gl/stb_image.h"

TextureGL43::TextureGL43(char* fileName, E_TEXTURE_TYPE type) {
	this->SetTexType(type);
	gl::GenTextures(1, &id);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);//resource/img/container.png

	GLenum format;
	if (nrChannels == 1)
		format = gl::RED;
	else if (nrChannels == 3)
		format = gl::RGB;
	else if (nrChannels == 4)
		format = gl::RGBA;

	gl::BindTexture(gl::TEXTURE_2D, id);
	gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGB, width, height, 0, format, gl::UNSIGNED_BYTE, data);
	gl::GenerateMipmap(gl::TEXTURE_2D);
	
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);


	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
	stbi_image_free(data);
}
TextureGL43::~TextureGL43() {

}

void TextureGL43::BindTexture(IShader *shader) {
	ShaderGL43 *tempShader = (ShaderGL43*)shader;
	tempShader->setInt("ourTexture", 0);

	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, id);
}

void TextureGL43::UnbindTexture() {
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, 0);
}