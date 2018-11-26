#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <vector>
#include <iostream>
#include <Classes/Shader.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
std::vector<GLfloat>* convertToFloat(std::vector<glm::vec3*>* points);
void convertCoordinates(double &x, double &y);

vector<glm::vec3*>* selectedPoints = new vector<glm::vec3*>();
vector<GLfloat>* finalPointsFloat = new vector<GLfloat>();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint vao, vbo;

void convertCoordinates(double &x, double &y) {
	//convert resolution coordinates to graph coordinates
	if (x > (SCR_WIDTH / 2)) {
		x = x - (SCR_WIDTH / 2);
		x = x / (SCR_WIDTH / 2);
	}
	else if (x == (SCR_WIDTH / 2)) {
		x = 0;
	}
	else {
		x = -(((SCR_WIDTH / 2) - x) / (SCR_WIDTH / 2));
	}

	if (y > (SCR_HEIGHT / 2)) {
		y = y - (SCR_HEIGHT / 2);
		y = y / (SCR_HEIGHT / 2);
		y = y * (-1);
	}
	else if (y == (SCR_HEIGHT / 2)) {
		y = 0;
	}
	else {
		y = -(((SCR_HEIGHT / 2) - y) / (SCR_HEIGHT / 2));
		y = y * (-1);
	}
}

std::vector<GLfloat>* convertToFloat(std::vector<glm::vec3*>* points) {
	std:vector<GLfloat>* temp = new std::vector<GLfloat>();
	for (int i = 0; i < points->size(); i++) {
		temp->push_back(points->at(i)->x);
		temp->push_back(points->at(i)->y);
		temp->push_back(points->at(i)->z);
	}
	return temp;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		convertCoordinates(xpos, ypos);
		
		glm::vec3* point = new glm::vec3(xpos, ypos, 0.0);
		selectedPoints->push_back(point);
		cout << "ponto registrado" << endl;
		cout << "x = " << xpos << endl;
		cout << "y = " << ypos << endl;
		

	// 	int zone = getZone(xpos, ypos);
	// 	if (zone == 1) {
	// 		xpos += 0.5;
	// 		ypos += 0.5;
	// 	}
	// 	else if (zone == 2) {
	// 		xpos -= 0.5;
	// 		ypos += 0.5;
	// 	}
	// 	else if (zone == 3) {
	// 		xpos -= 0.5;
	// 		ypos -= 0.5;
	// 	}
	// 	else if (zone == 4) {
	// 		xpos += 0.5;
	// 		ypos -= 0.5;
	// 	}
	// }	
	// if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
	// 	draw = true;

	// 	originalCurve = generateCurve(selectedPoints);
	// 	externalCurve = generateExternalCurve(originalCurve, true);
	// 	internalCurve = generateExternalCurve(originalCurve, false);

	// 	externalCurveSize = externalCurve->size() / 2.0;
	// 	internalCurveSize = internalCurve->size() / 2.0;
		
	// 	OBJWriter OBJWriter;
	// 	OBJWriter.saveTextureValuesToOBJ();

	// 	finalPoints = generateFinalCurve(internalCurve, externalCurve);

		finalPointsFloat = convertToFloat(selectedPoints);

		glGenBuffers(1, &vbo);
	
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*finalPointsFloat->size(), &finalPointsFloat->at(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	 }
}

int main(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao criar janela GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	Shader shader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	shader.use();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

		glBindVertexArray(vao);

		glEnable(GL_POINT_SMOOTH);
		glPointSize(15);
		glDrawArrays(GL_POINTS, 0, finalPointsFloat->size());
		glDisable(GL_POINT_SMOOTH);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}