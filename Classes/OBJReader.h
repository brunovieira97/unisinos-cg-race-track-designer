#ifndef OBJREADER_H
#define OBJREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <Classes/Mesh.h>

class OBJReader {
	enum OBJTypes {
		INVALID,
		VERTEX,
		VERTEX_NORMAL,
		VERTEX_TEXTURE,
		FACE,
		GROUP,
		MATERIAL_LIBRARY,
		USE_MATERIAL
	};

	typedef std::map<std::string, OBJTypes> OBJTypeMap;

	OBJTypeMap objTypeMap = {
		{"v", OBJTypes::VERTEX},
		{"vn", OBJTypes::VERTEX_NORMAL},
		{"vt", OBJTypes::VERTEX_TEXTURE},
		{"f", OBJTypes::FACE},
		{"g", OBJTypes::GROUP},
		{"mtllib", OBJTypes::MATERIAL_LIBRARY},
		{"usemtl", OBJTypes::USE_MATERIAL},
	};

	private:
		int lineCount;
		Mesh* mesh;
	
	public:
		OBJReader();

		Mesh* Read(std::string filename);
};

#endif