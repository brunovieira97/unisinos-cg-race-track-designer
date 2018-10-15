#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <Classes/Face.h>

class Group {
	private:
		std::vector<Face*> faces;
		std::string name, material;

	public:
		Group();
		Group(std::string name);

		void AddFace(Face* face);
		void AddFace(std::vector<int> vertices, std::vector<int> normals, std::vector<int> textures);

		void SetName(std::string name);
		void SetMaterial(std::string material);

		std::string GetName();
		std::string GetMaterial();
		
		std::vector<Face*> GetFaces();
		Face* GetFace(int faceIndex);
};

#endif