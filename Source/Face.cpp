#include <Classes/Face.h>

Face::Face() {

}

Face::Face(std::vector<int> vertices, std::vector<int> normals, std::vector<int> textures) {
	this -> vertices = vertices;
	this -> normals = normals;
	this -> textures = textures;
}

void Face::SetVertices(std::vector<int> vertices) {
	this -> vertices = vertices;	
}

void Face::SetNormals(std::vector<int> normals) {
	this -> normals = normals;	
}

void Face::SetTextures(std::vector<int> textures) {
	this -> textures = textures;	
}

std::vector<int> Face::GetVertices() {
	return this -> vertices;	
}

std::vector<int> Face::GetNormals() {
	return this -> normals;	
}

std::vector<int> Face::GetTextures() {
	return this -> textures;	
}
