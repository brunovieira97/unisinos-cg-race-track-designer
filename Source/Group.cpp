#include <Classes/Group.h>

Group::Group() {
}

Group::Group(std::string name) {
	this -> SetName(name);
}

void Group::AddFace(Face* face) {
	this -> faces.push_back(face);
}

void Group::AddFace(std::vector<int> vertices, std::vector<int> normals, std::vector<int> textures) {
	Face* newFace = new Face(vertices, normals, textures);

	this -> faces.push_back(newFace);
}

void Group::SetName(std::string name) {
	this -> name = name;
}

void Group::SetMaterial(std::string material) {
	this -> material = material;
}

std::string Group::GetName() {
	return this -> name;
}

std::string Group::GetMaterial() {
	return this -> material;
}

std::vector<Face*> Group::GetFaces() {
	return this -> faces;
}

Face* Group::GetFace(int faceIndex) {
	return this -> faces.at(faceIndex);
}

void Group::SetVAOIndex(int vaoIndex) {
	this -> vaoIndex = vaoIndex;
}

int Group::GetVAOIndex() {
	return this -> vaoIndex;
}

void Group::SetTextureIndex(int textureIndex) {
	this -> textureIndex = textureIndex;
}

int Group::GetTextureIndex() {
	return this -> textureIndex;
}