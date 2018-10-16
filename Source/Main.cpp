#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <STB/stb_image.h>
#include <Classes/Shader.h>
#include <Classes/MTLReader.h>
#include <Classes/OBJReader.h>

enum ArgTypes {
	INVALID,
	OBJ_FILE
};

typedef std::map<std::string, ArgTypes> ArgTypeMap;

ArgTypeMap argTypeMap = {
	{"-obj", ArgTypes::OBJ_FILE}
};

const unsigned int
	WINDOW_WIDTH = 500,
	WINDOW_HEIGHT = 500;

const char*
	WINDOW_TITLE = "OBJ Viewer";

// Lighting
	glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

// Camera
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f,  3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

	bool firstMouse = true;
	float yaw   = -90.0f;
	float pitch =  0.0f;
	float lastX =  800.0f / 2.0;
	float lastY =  600.0 / 2.0;
	float fov   =  45.0f;

// Timing (frame control)
	float
		deltaTime = 0.0f,
		lastFrame = 0.0f;


GLFWwindow *window;
std::vector<Material*> materials;

int InitializeGLFW();
void Read(std::string filename);
void DrawScene(Mesh* mesh);
void SetupScene(Mesh* mesh);
void SetupObjects(Mesh* mesh);
Material* GetMaterialByID(std::string materialID);

// Callback functions
	void KeyCallback();
	void ResizeCallback(GLFWwindow* window, int width, int height);
	void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

int InitializeGLFW() {
	if (!glfwInit()) {
		std::cerr << "Could not start GLFW." << std::endl;
		
		return EXIT_FAILURE;
	}

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if (!window) {
		std::cerr << "Could not open application's window using GLFW." << std::endl;
		
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, ResizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	return EXIT_SUCCESS;
}

void Read(std::string filename) {
	OBJReader* objReader = new OBJReader();
	Mesh* mesh = objReader -> Read(filename);

	SetupScene(mesh);
	
	while (!glfwWindowShouldClose(window)) {
		DrawScene(mesh);
	}
}

void SetupScene(Mesh* mesh) {
	std::string mtlFilename = mesh -> GetMaterialLibraryFilename();
	
	MTLReader* mtlReader = new MTLReader();
	materials = mtlReader -> Read(mtlFilename);

	if (InitializeGLFW() != EXIT_SUCCESS) {
		std::cerr << "Error while initializing GLFW and GLEW. Application will terminate." << std::endl;

		exit(EXIT_FAILURE);
	}

	SetupObjects(mesh);
}

Material* GetMaterialByID(std::string materialID) {
	for (int materialIndex = 0; materialIndex < materials.size(); materialIndex++)
		if (materials[materialIndex] -> GetMaterialID() == materialID)
			return materials[materialIndex];
	
	return nullptr;
}

void SetupObjects(Mesh* mesh) {
	std::vector<float> drawVector;

	Shader* shader = new Shader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");

	for (int groupIndex = 0; groupIndex < mesh -> GetGroups().size(); groupIndex++) {
		for (int faceIndex = 0; faceIndex < mesh -> GetGroup(groupIndex) -> GetFaces().size(); faceIndex++) {
			std::vector<int> verticesIndexes, normalsIndexes, texturesIndexes;
			
			verticesIndexes = mesh -> GetGroup(groupIndex) -> GetFace(faceIndex) -> GetVertices();
			normalsIndexes = mesh -> GetGroup(groupIndex) -> GetFace(faceIndex) -> GetNormals();
			texturesIndexes = mesh -> GetGroup(groupIndex) -> GetFace(faceIndex) -> GetTextures();

			if (verticesIndexes.size() != normalsIndexes.size()) {
				std::cerr << "Vertex and normal quantities differ." << std::endl;

				exit(EXIT_FAILURE);
			}

			for (int index = 0; index < verticesIndexes.size(); index++) {
				glm::vec3 vertex, normal;
				glm::vec2 texture;

				vertex = mesh -> GetVertex(verticesIndexes[index]);
				normal = mesh -> GetNormal(normalsIndexes[index]);
				texture = mesh -> GetMapping(texturesIndexes[index]);

				drawVector.push_back(vertex.x);
				drawVector.push_back(vertex.y);
				drawVector.push_back(vertex.z);
				drawVector.push_back(normal.x);
				drawVector.push_back(normal.y);
				drawVector.push_back(normal.z);
				drawVector.push_back(texture.x);
				drawVector.push_back(texture.y);
			}
		}

		GLuint VAO, VBO = 0;

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
	
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, drawVector.size() * sizeof(float), drawVector.data(), GL_STATIC_DRAW);
		
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		// Texture
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		mesh -> GetGroup(groupIndex) -> SetVAOIndex(VAO);

		if (mesh -> GetGroup(groupIndex) -> GetMaterial() != "") {
			Material* material = GetMaterialByID(mesh -> GetGroup(groupIndex) -> GetMaterial());

			if ((material != nullptr) && (material -> GetTextureFilename() != "")) {
				unsigned int textureID;

				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				int mapWidth, mapHeight, mapChannelCount;
				unsigned char *data = stbi_load(material -> GetTextureFilename().c_str(), &mapWidth, &mapHeight, &mapChannelCount, 0);
				
				if (!data)
					std::cerr << "Texture failed to load at path: " << material -> GetTextureFilename().c_str() << std::endl;
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mapWidth, mapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
				}

				mesh -> GetGroup(groupIndex) -> SetTextureIndex(textureID);
				stbi_image_free(data);
			}
		}
		glBindVertexArray(0);
	}
}

void DrawScene(Mesh* mesh) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	KeyCallback();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader* shader = new Shader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");

	shader -> use();
	shader -> setVec3("light.position", lightPosition);
	shader -> setVec3("viewPosition", cameraPosition);

	glm::mat4 projection = glm::perspective(glm::radians(fov), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	shader -> setMat4("projection", projection);
	shader -> setMat4("view", view);

	glm::mat4 model;
	shader -> setMat4("model", model);

	for (int groupIndex = 0; groupIndex < mesh -> GetGroups().size(); groupIndex++) {
		GLuint VAO = mesh -> GetGroup(groupIndex) -> GetVAOIndex();
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh -> GetGroup(groupIndex) -> GetTextureIndex());

		if (mesh -> GetGroup(groupIndex) -> GetMaterial() != "") {
			Material* material = GetMaterialByID(mesh -> GetGroup(groupIndex) -> GetMaterial());

			shader -> setFloat("material.shiness", material -> GetNS());
			shader -> setVec3("light.ambient", material -> GetKA());
			shader -> setVec3("light.diffuse", material -> GetKD());
			shader -> setVec3("light.specular", material -> GetKS());
		}

		glDrawArrays(GL_TRIANGLES, 0, mesh -> GetGroup(groupIndex) -> GetFaces().size() * 3);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();

}

void KeyCallback() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 15 * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void ResizeCallback(GLFWwindow* window, int width, int height) {
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
    
	if (pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;
    
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    
	front.y = sin(glm::radians(pitch));
    
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
	cameraFront = glm::normalize(front);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 45.0f)
		fov = 45.0f;
	else if (fov >= 1.0f)
		fov -= yoffset;
	else if (fov <= 1.0f)
		fov = 1.0f;
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