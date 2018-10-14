#include <Classes/MTLReader.h>

MTLReader::MTLReader() {
}

std::vector<Material*> MTLReader::Read(std::string filename) {
	std::vector<Material*> materials;
	
	std::ifstream file;

	file.open(filename);

	if (!file.is_open()) {
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

		MTLTypes typeToken = mtlTypeMap[token];

		switch(typeToken) {
			case NEW_MATERIAL: {
				std::cout << "[MTL] New material found at line " + std::to_string(lineCount) + "." << std::endl;
				
				std::string materialName;
				lineStream >> materialName;

				Material* material = new Material(materialName);

				this -> materials.push_back(material);

				this -> currentMaterialIndex = this -> materials.size() - 1;

				break;
			}

			case MAP_KD: {
				std::cout << "[MTL] Map KD found at line " + std::to_string(lineCount) + "." << std::endl;
				
				std::string textureFilename;
				lineStream >> textureFilename;

				this -> materials[currentMaterialIndex] -> SetTextureFilename(textureFilename);

				break;
			}

			case KA: case KS: case KD: {
				std::cout << "[MTL] KA found at line " + std::to_string(lineCount) + "." << std::endl;
				
				float red, green, blue;
				lineStream >> red >> green >> blue;

				if (token == "Ka")
					this -> materials[currentMaterialIndex] -> SetKA(red, green, blue);
				else if (token == "Ks")
					this -> materials[currentMaterialIndex] -> SetKS(red, green, blue);
				else
					this -> materials[currentMaterialIndex] -> SetKD(red, green, blue);

				break;
			}

			case NS: {
				std::cout << "[MTL] NS found at line " + std::to_string(lineCount) + "." << std::endl;
				
				float ns;
				lineStream >> ns;

				if (ns < 0)
					ns = ns* - 1;
				
				this -> materials[currentMaterialIndex] -> SetNS(ns);

				break;
			}

			default: {
				// Do nothing here
				break;
			}
		}
	}
	
	return this -> materials;

}