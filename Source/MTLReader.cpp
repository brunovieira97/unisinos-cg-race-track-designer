#include <Classes/MTLReader.h>

MTLReader::MTLReader(std::string filename) {
	if (filename != "")
		this -> Read(filename);
}

void MTLReader::Read(std::string filename) {
	std::ifstream file;

	file.open(filename);

	if (!file.is_open()) {
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

		MTLTypes typeToken = mtlTypeMap[token];

		switch(typeToken) {
			case NEW_MATERIAL: {
				std::cout << "New material found at line " + std::to_string(lineCount) + "." << std::endl;
				
				break;
			}

			case MAP_KD: {
				std::cout << "Map KD found at line " + std::to_string(lineCount) + "." << std::endl;
				
				break;
			}

			case KA: {
				std::cout << "KA found at line " + std::to_string(lineCount) + "." << std::endl;
				
				break;
			}

			case KS: {
				std::cout << "KS found at line " + std::to_string(lineCount) + "." << std::endl;
				
				break;
			}

			case KD: {
				std::cout << "KD found at line " + std::to_string(lineCount) + "." << std::endl;
				
				break;
			}

			case NS: {
				std::cout << "NS found at line " + std::to_string(lineCount) + "." << std::endl;
				
				break;
			}

			default: {
				// Do nothing here
				break;
			}

		}
	}

}