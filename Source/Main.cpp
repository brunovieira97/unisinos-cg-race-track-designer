#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <vector>
#include <iostream>
#include <Classes/Shader.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

using namespace std;

#define PI  3.14159265359
#define HALF_PI PI/2.0

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
vector<glm::vec3*>* CreateBSpline(vector<glm::vec3*>* points);
vector<glm::vec3*>* CreateExternalCurve(vector<glm::vec3*>* points, bool external);
void draw();
float EuclideanDistance(float xa, float ya, float xb, float yb);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void FillControlPointsFloat(vector<glm::vec3*>* points);

vector<float> controlPointsFloat;
vector<float> internalCurve;
vector<float> externalCurve;
vector<float> bSplineCurve;
vector<float> colors;
vector<glm::vec3*>* selectedPoints = new vector<glm::vec3*>();
vector<glm::vec3*>* controlPointsVec = new vector<glm::vec3*>();
vector<glm::vec3*>* externalCurveVec = new vector<glm::vec3*>();
vector<glm::vec3*>* internalCurveVec = new vector<glm::vec3*>();


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int selectedIndex;



glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

GLuint vaoC, vboC, vboColors;
GLuint vaoI, vboI;
GLuint vaoE, vboE;
GLuint vaoB, vboB;


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS){		
		float z = selectedPoints->at(selectedIndex)->z;
		if (0.0f <= z && z < 1.0f){
			z = z +  0.1f;
			selectedPoints->at(selectedIndex)->z = z;
		}		
		cout << "controlPointsFloat size = " << controlPointsFloat.size() << endl;
		controlPointsFloat.clear();
		cout << "selectedPoints size = " << selectedPoints->size() << endl;
		FillControlPointsFloat(selectedPoints);
		cout << "controlPointsFloat size = " << controlPointsFloat.size() << endl;
	}

	if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS){		
		float z = selectedPoints->at(selectedIndex)->z;
		if (0.0f < z && z <= 2.0f){
			z = z - 0.1f;
			if(z < 0)
				z = 0.0;
			selectedPoints->at(selectedIndex)->z = z;
			cout << "z aumentou = " << z << endl;
		}		
		controlPointsFloat.clear();
		cout << "selectedPoints size = " << selectedPoints->size() << endl;
		FillControlPointsFloat(selectedPoints);
	}
	draw();
}

void FillControlPointsFloat(vector<glm::vec3*>* points){

	for(int i = 0; i < points -> size(); i++)
	{
		controlPointsFloat.push_back(points->at(i)->x);
		controlPointsFloat.push_back(points->at(i)->y);
		controlPointsFloat.push_back(points->at(i)->z);
	}

}

void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		
		glm::vec3* point = new glm::vec3(xpos, ypos, 0.0);
		selectedPoints->push_back(point);

		controlPointsFloat.push_back(xpos);
		controlPointsFloat.push_back(ypos);
		controlPointsFloat.push_back(0.0f);
		cout << "ponto registrado" << endl;
		cout << "x = " << xpos << endl;
		cout << "y = " << ypos << endl;		

		glBindVertexArray(vaoC);
		glBindBuffer(GL_ARRAY_BUFFER, vboC);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*controlPointsFloat.size(), controlPointsFloat.data(), GL_STATIC_DRAW);		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);		
		glEnableVertexAttribArray(0);

		if(controlPointsFloat.size() > 9){

			controlPointsVec = CreateBSpline(selectedPoints);
			glBindVertexArray(vaoB);
			glBindBuffer(GL_ARRAY_BUFFER, vboB);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bSplineCurve.size(), bSplineCurve.data(), GL_STATIC_DRAW);		
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


			internalCurveVec = CreateExternalCurve(controlPointsVec, false);	
			internalCurve.clear();
			for(int i = 0; i < internalCurveVec -> size() - 1; i++)
			{
				internalCurve.push_back(internalCurveVec->at(i)->x);
				internalCurve.push_back(internalCurveVec->at(i)->y);
				internalCurve.push_back(internalCurveVec->at(i)->z);
			}
			glBindVertexArray(vaoI);
			glBindBuffer(GL_ARRAY_BUFFER, vboI);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*internalCurve.size(), internalCurve.data(), GL_STATIC_DRAW);		
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);			 
	

			externalCurveVec = CreateExternalCurve(controlPointsVec, true);	
			externalCurve.clear();
			for(int i = 0; i < externalCurveVec -> size() - 1; i++)
			{
				externalCurve.push_back(externalCurveVec->at(i)->x);
				externalCurve.push_back(externalCurveVec->at(i)->y);
				externalCurve.push_back(externalCurveVec->at(i)->z);
			}	
			glBindVertexArray(vaoE);
			glBindBuffer(GL_ARRAY_BUFFER, vboE);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*externalCurve.size(), externalCurve.data(), GL_STATIC_DRAW);		
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		}

	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int nearestIndex = 0;
		float nearestIndexDistance = EuclideanDistance(xpos, ypos, selectedPoints->at(0)->x, selectedPoints->at(0)->y);
		for(int i = 1; i < selectedPoints->size(); i++){
			float distance = EuclideanDistance(xpos, ypos, selectedPoints->at(i)->x, selectedPoints->at(i)->y);
			if(distance < nearestIndexDistance){
				nearestIndex = i;
				nearestIndexDistance = distance;
			}
		}
	
		selectedIndex = nearestIndex;
		cout << "selectedIndex " << selectedIndex << endl;			

	}
}

float EuclideanDistance(float xa, float ya, float xb, float yb){
	return sqrt(((xb - xa) * (xb - xa)) + ((yb - ya) * (yb - ya)));
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

	glfwSetKeyCallback(window, KeyCallback);
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

	glGenVertexArrays(1, &vaoC);
	glBindVertexArray(vaoC);
	glGenBuffers(1, &vboC);
	glGenBuffers(1, &vboColors);

	glGenVertexArrays(1, &vaoI);
	glBindVertexArray(vaoI);
	glGenBuffers(1, &vboI);

	glGenVertexArrays(1, &vaoE);
	glBindVertexArray(vaoE);
	glGenBuffers(1, &vboE);

	glGenVertexArrays(1, &vaoB);
	glBindVertexArray(vaoB);
	glGenBuffers(1, &vboB);


	colors.push_back(0.5);
	colors.push_back(0.5);
	colors.push_back(0.5);	
		
	Shader shader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	Shader shaderControlPoints("Shaders/VertexControlPoints.glsl", "Shaders/FragmentControlPoints.glsl");

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

		shader.use();

		shader.setMat4("projection", projection);
		// shaderControlPoints.setMat4("projection", projection);
		glEnable(GL_POINT_SMOOTH);
		glPointSize(15);
		// shader.setMat4("projection", projection);
		//glDrawArrays(GL_POINTS, 0, finalPointsFloat.size());
		// glDrawArrays(GL_LINE_STRIP, 0, finalPointsFloat.size() / 3);
		draw();
		glDisable(GL_POINT_SMOOTH);
        glfwSwapBuffers(window);
        glfwPollEvents();

		
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void draw(){

		glBindVertexArray(vaoC);
		glBindBuffer(GL_ARRAY_BUFFER, vboC);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*controlPointsFloat.size(), controlPointsFloat.data(), GL_STATIC_DRAW);		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);	
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_POINTS, 0, controlPointsFloat.size() / 3);

		glBindVertexArray(vaoI);
		glBindBuffer(GL_ARRAY_BUFFER, vboI);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*internalCurve.size(), internalCurve.data(), GL_STATIC_DRAW);		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glDrawArrays(GL_LINE_STRIP, 0, internalCurve.size() / 3);
	
		glBindVertexArray(vaoB);
		glBindBuffer(GL_ARRAY_BUFFER, vboB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bSplineCurve.size(), bSplineCurve.data(), GL_STATIC_DRAW);		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindVertexArray(vaoB);
		glDrawArrays(GL_LINE_STRIP, 0, bSplineCurve.size() / 3);


		glBindVertexArray(vaoE);
		glBindBuffer(GL_ARRAY_BUFFER, vboE);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*externalCurve.size(), externalCurve.data(), GL_STATIC_DRAW);		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glDrawArrays(GL_LINE_STRIP, 0, externalCurve.size() / 3);
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

	bSplineCurve.clear();
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
			bSplineCurve.push_back(x);
			bSplineCurve.push_back(y);
			bSplineCurve.push_back(0);

			//bspline->push_back(new glm::vec3(1.0, 1.0, 1.0));
		}	
	}

	return bspline;
}

vector<glm::vec3*>* CreateExternalCurve(vector<glm::vec3*>* points, bool external) {
	vector<glm::vec3*>* calculatedCurve = new vector<glm::vec3*>();

	for (int j = 0; j < points->size() - 1; j += 2) {

		glm::vec3* a = points->at(j);
		glm::vec3* b;

		if (j == points->size() - 2) {
			b = points->at(0);
		}
		else {
			b = points->at(j + 2);
		}

		GLfloat w = b->x - a->x;
		GLfloat h = b->y - a->y;

		if (w == 0 || h == 0) {
			w = b->x - points->at(j - 2)->x;
			h = b->y - points->at(j - 2)->y;
		}

		GLfloat angle = glm::atan(h, w);

		if (external) {
			angle += HALF_PI;
		}
		else {
			angle -= HALF_PI;
		}

		GLfloat cx = (glm::cos(angle) * 30);
		GLfloat cy = (glm::sin(angle) * 30);
		
		glm::vec3* pointGenerated = new glm::vec3(a->x + cx, a->y + cy, 0.0);
		calculatedCurve->push_back(pointGenerated);

	}

	calculatedCurve->push_back(calculatedCurve->at(0));
	calculatedCurve->push_back(calculatedCurve->at(1));
	calculatedCurve->push_back(calculatedCurve->at(2));

	return calculatedCurve;
}