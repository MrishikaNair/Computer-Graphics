//Assignment 03: Lighting and shading

/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/

#include "utils.h"

#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//Globals
int screen_width = 640, screen_height = 640;
GLint vModel_uniform, vView_uniform, vProjection_uniform;
GLint vColor_uniform, light_pos;

glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations

double oldX, oldY, currentX, currentY;
bool isDragging = false;

//void createCubeObject(unsigned int &, unsigned int &);

void setupModelTransformation(unsigned int &);
void setupViewTransformation(unsigned int &);
void setupProjectionTransformation(unsigned int &);
glm::vec3 getTrackBallVector(double x, double y);

const float PI = 3.14159265359;
int main(int, char**)
{
	// Setup window
	GLFWwindow *window = setupWindow(screen_width, screen_height);
	ImGuiIO& io = ImGui::GetIO(); // Create IO object

	ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");

	//Get handle to color variable in shader
	vColor_uniform = glGetUniformLocation(shaderProgram, "vColor");
	//if (vColor_uniform == -1) {
	//	fprintf(stderr, "Could not bind location: vColor\n");
	//	//exit(0);
	//}

	int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
	/*if (vVertex_attrib == -1) {
		fprintf(stderr, "Could not bind location: vVertex\n");
		exit(0);
	}*/

	int vNormal_attrib = glGetAttribLocation(shaderProgram, "vertex_norm");
	/*if (vNormal_attrib == -1) {
		fprintf(stderr, "Could not bind location: vertex_norm\n");
		exit(0);
	}*/
	int vNormal_uniform = glGetUniformLocation(shaderProgram, "vNormal");
	/*if (vNormal_uniform == -1) {
		fprintf(stderr, "Could not bind location: vNormal_uniform\n");
		exit(0);
	}*/

	light_pos = glGetUniformLocation(shaderProgram, "l_pos");

	int u = 20;
	int Vertices = 1200;
	GLfloat *shape_vertices = new GLfloat[(Vertices) * 3];
	GLfloat *vertex_normals = new GLfloat[(Vertices) * 3];
	/*
	GLfloat *nor=new GLfloat[(Vertices)*3];
	for(int i=0;i<Vertices/3;i++){
		nor[i*3]=1.0;
		nor[i*3+1]=1.0;
		nor[i*3+2]=0.5;
	}*/

	float v = 2 * PI / 60;
	float j = 0;
	shape_vertices[0] = 0;
	shape_vertices[1] = 0;
	shape_vertices[2] = u/400;
	glm::vec3 dfdv = glm::vec3(shape_vertices[0], shape_vertices[1], shape_vertices[2] / u);
	glm::vec3 dfdu = glm::vec3(u*(-1)*sin(v / 2), u*cos(v / 2), 0);
	glm::vec3 n = glm::vec3(cross(glm::vec3(dfdu), glm::vec3(dfdv)));
	//n=(glm::vec3(n)/glm::length(glm::vec3(n)));
	glm::vec3 n_normal = glm::normalize(n);
	vertex_normals[0] = 0;
	vertex_normals[1] = 0;
	vertex_normals[2] = 0;

	shape_vertices[3] = u * cos(v);
	shape_vertices[4] = u * sin(v);
	shape_vertices[5] = u * (2) / 400;
	dfdv = glm::vec3(shape_vertices[3] / u, shape_vertices[4] / u, shape_vertices[5] / u);
	dfdu = glm::vec3(u*(-1)*sin(v), u*cos(v), 0);
	n = glm::vec3(cross(glm::vec3(dfdu), glm::vec3(dfdv)));
	n_normal = glm::normalize(n);
	vertex_normals[3] = n_normal.x;
	vertex_normals[4] = n_normal.y;
	vertex_normals[5] = n_normal.z;

	shape_vertices[6] = u * cos(2 * v);
	shape_vertices[7] = u * sin(2 * v);
	shape_vertices[8] = u * (3) / 400;
	dfdv = glm::vec3(shape_vertices[6] / u, shape_vertices[7] / u, shape_vertices[8] / u);
	dfdu = glm::vec3(u*(-2)*sin(2 * v), u*2*cos(2 * v), 0);
	n = glm::vec3(cross(glm::vec3(dfdu), glm::vec3(dfdv)));
	n_normal = glm::normalize(n);
	vertex_normals[6] = n_normal.x;
	vertex_normals[7] = n_normal.y;
	vertex_normals[8] = n_normal.z;

	j++;
	for (int i = 1; i < Vertices / 3; i++, j++) {
		/*
		shape_vertices[i*9]=0;
		shape_vertices[i*9+1]=0;
		shape_vertices[i*9+2]=u*(3*j+1)/30;
		shape_vertices[i*9+3]=10*u*cos((i+1)*v);
		shape_vertices[i*9+4]=10*u*sin((i+1)*v);
		shape_vertices[i*9+5]=u*(3*j+2)/30;
		shape_vertices[i*9+6]=10*u*cos((i+2)*v);
		shape_vertices[i*9+7]=10*u*sin((i+2)*v);
		shape_vertices[i*9+8]=u*(3*j+3)/30;*/

		shape_vertices[i * 9] = 0;
		shape_vertices[i * 9 + 1] = 0;
		shape_vertices[i * 9 + 2] = shape_vertices[(i - 1) * 9 + 2];
		dfdv = glm::vec3(shape_vertices[i * 9], shape_vertices[i * 9 + 1], shape_vertices[i * 9 + 2] / u);
		dfdu = glm::vec3(0, 0, 0);
		n = glm::vec3(cross(glm::vec3(dfdu), glm::vec3(dfdv)));
		//n=(glm::vec3(n)/glm::length(glm::vec3(n)));
		n_normal = glm::normalize(n);
		vertex_normals[i * 9] = 0;
		vertex_normals[i * 9 + 1] = 0;
		vertex_normals[i * 9 + 2] = 0;

		shape_vertices[i * 9 + 3] = shape_vertices[(i - 1) * 9 + 6];
		shape_vertices[i * 9 + 4] = shape_vertices[(i - 1) * 9 + 7];
		shape_vertices[i * 9 + 5] = shape_vertices[(i - 1) * 9 + 8];
		dfdv = glm::vec3(shape_vertices[(i - 1) * 9 + 6], shape_vertices[(i - 1) * 9 + 7], shape_vertices[(i - 1) * 9 + 8] / u);
		dfdu = glm::vec3(u*(-1)*i*sin(i*v), u*i*cos(i*v), 0);
		n = glm::vec3(cross(glm::vec3(dfdu), glm::vec3(dfdv)));
		//n=(glm::vec3(n)/glm::length(glm::vec3(n)));
		n_normal = glm::normalize(n);
		vertex_normals[i * 9 + 3] = n_normal.x;
		vertex_normals[i * 9 + 4] = n_normal.y;
		vertex_normals[i * 9 + 5] = n_normal.z;

		shape_vertices[i * 9 + 6] = u * cos((i + 1)*v);
		shape_vertices[i * 9 + 7] = u * sin((i + 1)*v);
		shape_vertices[i * 9 + 8] = u * (3 * j + 2) / 400;
		dfdv = glm::vec3(shape_vertices[i * 9 + 6], shape_vertices[i * 9 + 7], shape_vertices[i * 9 + 8] / u);
		dfdu = glm::vec3(u*(-1)*(i+1)*sin((i + 1)*v), u*(i+1)*cos((i + 1)*v), 0);
		n = glm::vec3(cross(glm::vec3(dfdu), glm::vec3(dfdv)));
		//n=(glm::vec3(n)/glm::length(glm::vec3(n)));
		n_normal = glm::normalize(n);
		vertex_normals[i * 9 + 6] = n_normal.x;
		vertex_normals[i * 9 + 7] = n_normal.y;
		vertex_normals[i * 9 + 8] = n_normal.z;


	}
	glUseProgram(shaderProgram);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint vertex_VBO;
	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices * 3 * sizeof(GLfloat), shape_vertices, GL_STATIC_DRAW); //else tri_points
	glEnableVertexAttribArray(static_cast<unsigned int>(vVertex_attrib));
	glVertexAttribPointer(static_cast<unsigned int>(vVertex_attrib), 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	GLuint normal_VBO;
	glGenBuffers(1, &normal_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices * 3 * sizeof(GLfloat), vertex_normals, GL_STATIC_DRAW); //else tri_points
	//glBufferData(GL_ARRAY_BUFFER, Vertices*3*sizeof(GLfloat), nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);

	setupModelTransformation(shaderProgram);
	setupViewTransformation(shaderProgram);
	setupProjectionTransformation(shaderProgram);

	//createCubeObject(shaderProgram, VAO);

	oldX = oldY = currentX = currentY = 0.0;
	int prevLeftButtonState = GLFW_RELEASE;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Get current mouse position
		int leftButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if (leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_RELEASE) {
			isDragging = true;
			currentX = oldX = x;
			currentY = oldY = y;
		}
		else if (leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_PRESS) {
			currentX = x;
			currentY = y;
		}
		else if (leftButtonState == GLFW_RELEASE && prevLeftButtonState == GLFW_PRESS) {
			isDragging = false;
		}

		// Rotate based on mouse drag movement
		prevLeftButtonState = leftButtonState;
		if (isDragging && (currentX != oldX || currentY != oldY))
		{
			glm::vec3 va = getTrackBallVector(oldX, oldY);
			glm::vec3 vb = getTrackBallVector(currentX, currentY);

			float angle = acos(std::min(1.0f, glm::dot(va, vb)));
			glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
			glm::mat3 camera2object = glm::inverse(glm::mat3(viewT*modelT));
			glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
			modelT = glm::rotate(modelT, angle, axis_in_object_coord);
			glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));

			oldX = currentX;
			oldY = currentY;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glUseProgram(shaderProgram);

		{
			ImGui::Begin("Information");
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);

		glUniform3f(vColor_uniform, 1.0, 0.5, 0.5);
		glUniform3f(light_pos, 7.0, 0.0, 0.0);
		glUniform3f(vNormal_uniform, 1.0, 0.0, 0.0);

		glDrawArrays(GL_TRIANGLES, 0, 1200);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

	}

	// Cleanup
	cleanup(window);

	return 0;
}

void createCubeObject(unsigned int &program, unsigned int &cube_VAO)
{
	glUseProgram(program);

	//Bind shader variables
	int vVertex_attrib = glGetAttribLocation(program, "vVertex");
	if (vVertex_attrib == -1) {
		fprintf(stderr, "Could not bind location: vVertex\n");
		exit(0);
	}

	//Cube data
	GLfloat cube_vertices[] = { 10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10, //Front
				   10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10 }; //Back
	GLushort cube_indices[] = {
				0, 1, 2, 0, 2, 3, //Front
				4, 7, 5, 5, 7, 6, //Back
				1, 6, 2, 1, 5, 6, //Left
				0, 3, 4, 4, 7, 3, //Right
				0, 4, 1, 4, 5, 1, //Top
				2, 6, 3, 3, 6, 7 //Bottom
	};

	//Generate VAO object
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);

	//Create VBOs for the VAO
	//Position information (data + format)
	int nVertices = (6 * 2) * 3; //(6 faces) * (2 triangles each) * (3 vertices each)
	GLfloat *expanded_vertices = new GLfloat[nVertices * 3];
	for (int i = 0; i < nVertices; i++) {
		expanded_vertices[i * 3] = cube_vertices[cube_indices[i] * 3];
		expanded_vertices[i * 3 + 1] = cube_vertices[cube_indices[i] * 3 + 1];
		expanded_vertices[i * 3 + 2] = cube_vertices[cube_indices[i] * 3 + 2];
	}
	GLuint vertex_VBO;
	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vVertex_attrib);
	glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	delete[]expanded_vertices;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void setupModelTransformation(unsigned int &program)
{
	//Modelling transformations (Model -> World coordinates)
	modelT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));//Model coordinates are the world coordinates

	//Pass on the modelling matrix to the vertex shader
	glUseProgram(program);
	vModel_uniform = glGetUniformLocation(program, "vModel");
	if (vModel_uniform == -1) {
		fprintf(stderr, "Could not bind location: vModel\n");
		exit(0);
	}
	glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}


void setupViewTransformation(unsigned int &program)
{
	//Viewing transformations (World -> Camera coordinates
	//Camera at (0, 0, 100) looking down the negative Z-axis in a right handed coordinate system
	viewT = glm::lookAt(glm::vec3(40.0, -40.0, 40.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	//Pass-on the viewing matrix to the vertex shader
	glUseProgram(program);
	vView_uniform = glGetUniformLocation(program, "vView");
	if (vView_uniform == -1) {
		fprintf(stderr, "Could not bind location: vView\n");
		exit(0);
	}
	glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}

void setupProjectionTransformation(unsigned int &program)
{
	//Projection transformation
	projectionT = glm::perspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 1000.0f);

	//Pass on the projection matrix to the vertex shader
	glUseProgram(program);
	vProjection_uniform = glGetUniformLocation(program, "vProjection");
	if (vProjection_uniform == -1) {
		fprintf(stderr, "Could not bind location: vProjection\n");
		exit(0);
	}
	glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

glm::vec3 getTrackBallVector(double x, double y)
{
	glm::vec3 p = glm::vec3(2.0*x / screen_width - 1.0, 2.0*y / screen_height - 1.0, 0.0); //Normalize to [-1, +1]
	p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

	float mag2 = p.x*p.x + p.y*p.y;
	if (mag2 <= 1.0f)
		p.z = sqrtf(1.0f - mag2);
	else
		p = glm::normalize(p); //Nearest point, close to the sides of the trackball
	return p;
}