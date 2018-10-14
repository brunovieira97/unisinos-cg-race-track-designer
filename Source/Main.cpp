#include <Classes/OBJReader.h>

enum ArgTypes {
	INVALID,
	OBJ_FILE
};

typedef std::map<std::string, ArgTypes> ArgTypeMap;

ArgTypeMap argTypeMap = {
	{"-obj", ArgTypes::OBJ_FILE}
};

void Read();
void DrawScene();

void Read(std::string filename) {
	OBJReader objReader = OBJReader(filename);
}

void DrawScene() {

}

int main(int argc, char const *argv[]) {
	if (argc < 3) {
		std::cout << "Please use -obj and inform OBJ file's path to continue." << std::endl;
		
		return EXIT_FAILURE;
	}

	ArgTypes argType;

	for (int i = 1; i < argc; i++) {
		argType = argTypeMap[argv[i]];

		switch (argType) {
			case OBJ_FILE: {
				if ((argv[i + 1] == NULL) || (argv[i + 1] == "")) {
					std::cerr << "OBJ file not informed!" << std::endl;

					return EXIT_FAILURE;
				}

				i++;
				
				Read(argv[i]);

				break;
			}

			default: {
				std::cerr << "Invalid parameters!" << std::endl;

				return EXIT_FAILURE;
			}
		}
	}

	return EXIT_SUCCESS;
	
}