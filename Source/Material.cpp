#include <Classes/Material.h>

Material::Material() {
}

Material::Material(std::string materialID) {
	this -> SetMaterialID(materialID);
}

void Material::SetMaterialID(std::string materialID) {
	this -> materialID = materialID;
}

std::string Material::GetMaterialID() {
	return this -> materialID;
}

void Material::SetTextureFilename(std::string textureFilename) {
	this -> textureFilename = textureFilename;
}

std::string Material::GetTextureFilename() {
	return this -> textureFilename;
}

void Material::SetKA(float red, float green, float blue) {
	glm::vec3 newKa(red, green, blue);

	this -> ka = newKa;
}

glm::vec3 Material::GetKA() {
	return this -> ka;
}

void Material::SetKS(float red, float green, float blue) {
	glm::vec3 newKs(red, green, blue);

	this -> ks = newKs;
}

glm::vec3 Material::GetKS() {
	return this -> ks;
}

void Material::SetKD(float red, float green, float blue) {
	glm::vec3 newKd(red, green, blue);

	this -> kd = newKd;
}

glm::vec3 Material::GetKD() {
	return this -> kd;
}

void Material::SetNS(float ns) {
	this -> ns = ns;
}

float Material::GetNS() {
	return this -> ns;
}
