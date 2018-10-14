#include <Classes/OBJReader.h>

OBJReader::OBJReader(std::string filename) {
	if (filename != "")
		this -> Read(filename);
}

void OBJReader::Read(std::string filename) {
	std::ifstream file;

	file.open(filename);

	if(!file.is_open()) {
		std::cerr << "Unable to open " + filename + " file!" << std::endl;
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

				break;
			}

			case VERTEX_NORMAL: {
				std::cout << "Vertex normal found at line " + std::to_string(lineCount) + "." << std::endl;

				break;
			}

			case VERTEX_TEXTURE: {
				std::cout << "Vertex texture found at line " + std::to_string(lineCount) + "." << std::endl;

				break;
			}

			case FACE: {
				std::cout << "Face found at line " + std::to_string(lineCount) + "." << std::endl;

				break;
			}

			case GROUP: {
				std::cout << "Group found at line " + std::to_string(lineCount) + "." << std::endl;

				break;
			}

			case MATERIAL_LIBRARY: {
				std::cout << "Material library found at line " + std::to_string(lineCount) + "." << std::endl;

				break;
			}

			case USE_MATERIAL: {
				std::cout << "Material usage found at line " + std::to_string(lineCount) + "." << std::endl;

				break;
			}

			default: {
				// Do nothing here
				break;
			}
		}

	}
}