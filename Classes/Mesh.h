#ifndef MESH
#define MESH

#include <GLM/gtc/type_ptr.hpp>
#include <Classes/Group.h>
#include <Classes/Material.h>

class Mesh {
	private:
		std::vector<glm::vec3> vertices, normals;
		std::vector<glm::vec2> mappings;

		std::vector<Group*> groups;
		std::vector<Material*> materials;

		int activeGroupIndex;
		std::string materialLibraryFilename;

	public:
		Mesh();
		
		void AddVertex(float x, float y, float z);
		std::vector<glm::vec3> GetVertices();
		glm::vec3 GetVertex(int vertexIndex);

		void AddNormal(float x, float y, float z);
		std::vector<glm::vec3> GetNormals();
		glm::vec3 GetNormal(int normalIndex);

		void AddMapping(float x, float y);
		std::vector<glm::vec2> GetMappings();
		glm::vec2 GetMapping(int mappingIndex);

		void AddGroup(std::string groupName);
		std::vector<Group*> GetGroups();
		Group* GetGroup(int groupindex);

		void SetActiveGroupIndex(int groupIndex);
		int GetActiveGroupIndex();

		void SetMaterialLibraryFilename(std::string materialLibraryFilename);
		std::string GetMaterialLibraryFilename();

		bool GroupExists(std::string groupName);

		void AddFaceToActiveGroup(std::vector<int> vertices, std::vector<int> normals, std::vector<int> textures);

		void SetActiveGroupMaterial(std::string materialID);
};

#endif