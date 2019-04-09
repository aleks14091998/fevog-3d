#include"managers/ResourceManager.h"
#include"Material.h"

Material::Material() : texture(NULL){

}

Material::~Material(){
	delete texture;
}
void Material::SetTexture(char* filename, E_TEXTURE_TYPE type) {
	CResourceManager::AddTexture(texture, filename, type);
}

ITexture* Material::GetTextureDefault() {
	return texture;
}

void Material::BindTextures(IShader *shader) {
	if (texture) {
		texture->BindTexture(shader);
	}
}