#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <vector>
#include <iostream>
#include <Classes/Shader.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
vector<glm::vec3*>* CreateBSpline(vector<glm::vec3*>* points);

vector<float> finalPointsFloat;
vector<glm::vec3*>* selectedPoints = new vector<glm::vec3*>();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int click = 0;

glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

GLuint vao, vbo;

void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		
		glm::vec3* point = new glm::vec3(xpos, ypos, 0.0);
		selectedPoints->push_back(point);

		// finalPointsFloat.push_back(xpos);
		// finalPointsFloat.push_back(ypos);
		// finalPointsFloat.push_back(0.0);
		cout << "ponto registrado" << endl;
		cout << "x = " << xpos << endl;
		cout << "y = " << ypos << endl;

		if (click > 2){
			CreateBSpline(selectedPoints);
		}
		

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*finalPointsFloat.size(), &finalPointsFloat[0], GL_STATIC_DRAW);		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		int size = finalPointsFloat.size();
		for(int i = 0; i < size; i++){
			printf("%f\n",finalPointsFloat[i]);
		}

		click++;
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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao criar janela GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Shader shader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	// shader.use();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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

		Shader shader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
		shader.use();

		glEnable(GL_POINT_SMOOTH);
		glPointSize(15);
		shader.setMat4("projection", projection);
		//glDrawArrays(GL_POINTS, 0, finalPointsFloat.size());
		glDrawArrays(GL_LINE_STRIP, 0, finalPointsFloat.size() / 3);
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


vector<glm::vec3*>* CreateBSpline(vector<glm::vec3*>* points) {

	vector<glm::vec3*>* bspline = new vector<glm::vec3*>();
	vector<glm::vec3*>* temp = new vector<glm::vec3*>();
	vector<glm::vec3*>* temp2 = new vector<glm::vec3*>();

	int size = points->size();
	for (int i = 0; i < size ; i++) {
		temp->push_back(new glm::vec3(points->at(i)->x, points->at(i)->y, 0));
	}


	temp->push_back(points->at(0));
	temp->push_back(points->at(1));
	temp->push_back(points->at(2));


	for (int i = 0; i < (temp->size() - 3); i++) {

		for (int j = 0; j<100; ++j){

			float t = static_cast<float>(j)/99.0;
		
			GLfloat x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*temp->at(i)->x +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*temp->at(i+1)->x +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*temp->at(i+2)->x +
				(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*temp->at(i+3)->x) / 6);

			GLfloat y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1)*temp->at(i)->y +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4)*temp->at(i+1)->y +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1)*temp->at(i+2)->y +
				(1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0)*temp->at(i+3)->y) / 6);
					
			glm::vec3* point = new glm::vec3(x, y, 0.0);
			bspline->push_back(point);
			finalPointsFloat.push_back(x);
			finalPointsFloat.push_back(y);
			finalPointsFloat.push_back(0);

			bspline->push_back(new glm::vec3(1.0, 1.0, 1.0));
		}	
	}

	return bspline;
}