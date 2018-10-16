#include <Classes/OBJReader.h>

OBJReader::OBJReader() {
	this -> mesh = new Mesh();
}

Mesh* OBJReader::Read(std::string filename) {
	std::ifstream file;

	file.open(filename);

	if(!file.is_open()) {
		std::cerr << "Unable to open " + filename + ". Application will terminate." << std::endl;
		exit(EXIT_FAILURE);
	}

	lineCount = 0;

	while(!file.eof()) {
		std::string currentLine, token;
		std::stringstream lineStream;
		
		lineCount++;

		getline(file, currentLine);
		lineStream << currentLine;
		lineStream >> token;

		OBJTypes typeToken = objTypeMap[token];

		switch(typeToken) {
			case VERTEX: {
				std::cout << "Vertex found at line " + std::to_string(lineCount) + "." << std::endl;

				float x, y, z;
				lineStream >> x >> y >> z;

				this -> mesh -> AddVertex(x, y, z);

				break;
			}

			case VERTEX_NORMAL: {
				std::cout << "Vertex normal found at line " + std::to_string(lineCount) + "." << std::endl;

				float x, y, z;
				lineStream >> x >> y >> z;

				this -> mesh -> AddNormal(x, y, z);

				break;
			}

			case VERTEX_TEXTURE: {
				std::cout << "Vertex texture found at line " + std::to_string(lineCount) + "." << std::endl;

				float x, y;
				lineStream >> x >> y;

				this -> mesh -> AddMapping(x, y);

				break;
			}

			case FACE: {
				std::cout << "Face found at line " + std::to_string(lineCount) + "." << std::endl;

				std::vector<int> vertices, normals, textures;

				while (lineStream.rdbuf() -> in_avail()) {
					std::string faceToken, v, n, t;
					lineStream >> faceToken;

					if (faceToken == "")
						break;
					
					std::stringstream faceStream(faceToken);

					std::getline(faceStream, v, '/');
					std::getline(faceStream, t, '/');
					std::getline(faceStream, n, '/');

					if (vertices.size() == 3) {
						this -> mesh -> AddFaceToActiveGroup(vertices, normals, textures);
						vertices[1] = stoi(v) - 1;
						normals[1]	= stoi(n) - 1;
						textures[1] = stoi(t) - 1;
					} else {
						vertices.push_back(stoi(v) - 1);
						normals.push_back (stoi(n) - 1);
						textures.push_back(stoi(t) - 1);
					}
				}

				this -> mesh -> AddFaceToActiveGroup(vertices, normals, textures);

				break;
			}

			case GROUP: {
				std::cout << "Group found at line " + std::to_string(lineCount) + "." << std::endl;

				std::string groupName;
				lineStream >> groupName;

				if (groupName == "")
					groupName = "defaultGroup";

				if (!mesh -> GroupExists(groupName)) {
					mesh -> AddGroup(groupName);
				}

				break;
			}

			case MATERIAL_LIBRARY: {
				std::cout << "Material library found at line " + std::to_string(lineCount) + "." << std::endl;

				std::string materialLibraryFilename;
				lineStream >> materialLibraryFilename;

				this -> mesh -> SetMaterialLibraryFilename(materialLibraryFilename);

				break;
			}

			case USE_MATERIAL: {
				std::cout << "Material usage found at line " + std::to_string(lineCount) + "." << std::endl;

				std::string materialID;
				lineStream >> materialID;

				this -> mesh -> SetActiveGroupMaterial(materialID);

				break;
			}

			default: {
				// Do nothing here
				break;
			}
		}
	}
	
	return this -> mesh;
}