#include <Classes/OBJReader.h>
#include <Classes/Shader.h>
#include <GLFW/glfw3.h>

enum ArgTypes {
	INVALID,
	OBJ_FILE
};

typedef std::map<std::string, ArgTypes> ArgTypeMap;

ArgTypeMap argTypeMap = {
	{"-obj", ArgTypes::OBJ_FILE}
};

void Read(std::string filename);
void DrawScene(Mesh* mesh);
int InitializeGLFW();
int TerminateApplication(bool errorOccurred = false);
void SetupScene();

// Callback functions
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void ScrollCallback(GLFWwindow* window, double offsetX, double offsetY);

const unsigned int 
	WIDTH = 640, 
	HEIGHT = 480;

// Camera variables
	glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

	bool firstMouse = true;
	float yaw   = -90.0f;
	float pitch =  0.0f;
	float lastX =  800.0f / 2.0;
	float lastY =  600.0 / 2.0;
	float fieldOfView   =  45.0f;

// Timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

int screenWidth, screenHeight;
GLFWwindow* window;

void Read(std::string filename) {
	OBJReader* objReader = new OBJReader();
	Mesh* mesh = objReader -> Read(filename);

	DrawScene(mesh);
}

void DrawScene(Mesh* mesh) {

	

}

int TerminateApplication(bool errorOccurred) {
	// Deallocate objects and prepare for destroying application
	glfwTerminate();

	if (errorOccurred) {
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

int InitializeGLFW() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT, "OBJ Viewer", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "ERROR: Failed to create application's main window." << std::endl;
		return TerminateApplication(true);
	}

	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR: Failed to initialize GLEW." << std::endl;
		return TerminateApplication(true);
	}

	return EXIT_SUCCESS;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}

void ScrollCallback(GLFWwindow* window, double offsetX, double offsetY) {
	if (fieldOfView >= 45.0f)
		fieldOfView = 45.0f;
	else if (fieldOfView >= 1.0f)
		fieldOfView -= offsetY;
	else
		fieldOfView = 1.0f;
}

int main(int argc, char const *argv[]) {
	std::string objFilename;

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
				objFilename = argv[i];

				break;
			}

			default: {
				std::cerr << "Invalid parameters!" << std::endl;

				return EXIT_FAILURE;
			}
		}
	}

	Read(objFilename);

	return EXIT_SUCCESS;	
}