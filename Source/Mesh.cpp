#include <Classes/Mesh.h>

Mesh::Mesh() {
}

void Mesh::AddVertex(float x, float y, float z) {
	glm::vec3 vertex(x, y, z);

	this -> vertices.push_back(vertex);
}

void Mesh::AddNormal(float x, float y, float z) {
	glm::vec3 normal(x, y, z);

	this -> normals.push_back(normal);
}

void Mesh::AddMapping(float x, float y) {
	glm::vec2 mapping(x, y);

	this -> mappings.push_back(mapping);
}

void Mesh::AddGroup(std::string groupName) {
	Group* newGroup = new Group(groupName);

	this -> groups.push_back(newGroup);

	this -> SetActiveGroupIndex(this -> groups.size() - 1);
}

std::vector<glm::vec3> Mesh::GetVertices() {
	return this -> vertices;
}

glm::vec3 Mesh::GetVertex(int vertexIndex) {
	return this -> vertices.at(vertexIndex);
}

std::vector<glm::vec3> Mesh::GetNormals() {
	return this -> normals;
}

glm::vec3 Mesh::GetNormal(int normalIndex) {
	return this -> normals.at(normalIndex);
}

std::vector<glm::vec2> Mesh::GetMappings() {
	return this -> mappings;
}

glm::vec2 Mesh::GetMapping(int mappingIndex) {
	return this -> mappings.at(mappingIndex);
}

std::vector<Group*> Mesh::GetGroups() {
	return this -> groups;
}

Group* Mesh::GetGroup(int groupIndex) {
	return this -> groups.at(groupIndex);
}

bool Mesh::GroupExists(std::string groupName) {
	for (std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); it++)
		if (groupName == (*it) -> GetName())
			return true;

	return false;
}

void Mesh::SetActiveGroupIndex(int activeGroupIndex) {
	this -> activeGroupIndex = activeGroupIndex;
}

int Mesh::GetActiveGroupIndex() {
	return this -> activeGroupIndex;
}

void Mesh::AddFaceToActiveGroup(std::vector<int> vertices, std::vector<int> normals, std::vector<int> textures) {
	this -> groups.at(GetActiveGroupIndex()) -> AddFace(vertices, normals, textures);
}

