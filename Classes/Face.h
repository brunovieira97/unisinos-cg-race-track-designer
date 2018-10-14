#include <vector>

class Face {
	private:
		std::vector<int> vertices, normals, textures;
	
	public:
		Face();
		Face(std::vector<int> vertices, std::vector<int> normals, std::vector<int> textures);

		void SetVertices(std::vector<int> vertices);
		void SetNormals(std::vector<int> normals);
		void SetTextures(std::vector<int> textures);

		std::vector<int> GetVertices();
		std::vector<int> GetNormals();
		std::vector<int> GetTextures();
};