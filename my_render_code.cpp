
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <cstdio>
#include <cassert>

#include "GL_framework.h"


#include <vector>

#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

#include <iostream>
#include <string>

extern bool loadOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals);

bool show_test_window = false;

int exercice, camPos, bombilla, toonShader;
bool day, showModels;
double timer_camera, timer_sun, timer_moon;

int totalCabins = 20;
float radiusWheel = 50.f;
float frecWheel = 0.1f;

int cabinNum = 9;

glm::vec3 ambientColor = glm::vec3(0.3, 0.3, 0.2);

void setVariables(int inExercice) {
	switch (inExercice) {
	case 1:
		showModels = false;
		break;
	case 2:
		showModels = true;
		break;
	case 3:
		camPos = 1;
		break;
	case 4:
		camPos = 3;
		break;
	case 5:
		day = true;
		break;
	case 6:
		camPos = 2;
		day = false;
		bombilla = 1;
		break;
	case 7:
		camPos = 2;
		day = false;
		bombilla = 2;
		break;
	case 9:
		toonShader = 1;
		break;
	case 10:
		toonShader = 2;
		break;
	case 11:
		toonShader = 3;
		break;
	default:
		break;
	}
}


void GUI() {
	bool show = true;
	ImGui::Begin("Simulation Parameters", &show, 0);

	// Do your GUI code here....
	{
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate

		ImGui::Text("Exercise: %.2d", exercice);
		if (ImGui::Button("Next exercise (A)")) {
			exercice++;
			if (exercice > 11) {
				exercice = 1;
			}
			setVariables(exercice);
		}
		if (ImGui::Button("Previous exercise (Z)")) {
			exercice--;
			if (exercice < 1) {
				exercice = 11;
			}
			setVariables(exercice);
		}
		if (ImGui::Button("Day/Night Transition (D)")) {
			day = !day;
		}
		switch (bombilla) {
		case 0:
			if (ImGui::Button("Light Bulb variant (B): Light Bulb OFF")) {
				camPos = 1;
				showModels = true;
				day = false;
				bombilla++;
			}
			break;
		case 1:
			if (ImGui::Button("Light Bulb variant (B): Light Bulb ON")) {
				camPos = 1;
				showModels = true;
				day = false;
				bombilla++;
			}
			break;
		case 2:
			if (ImGui::Button("Light Bulb variant (B): Light Bulb Move")) {
				day = true;
				bombilla = 0;
			}
			break;
		default:
			break;
		}
		switch (toonShader) {
		case 0:
			if (ImGui::Button("Toon Shader Variant (T): Toon Shader OFF")) {
				toonShader++;
			}
			break;
		case 1:
			if (ImGui::Button("Toon Shader Variant (T): Toon Shader Sun")) {
				toonShader++;
			}
			break;
		case 2:
			if (ImGui::Button("Toon Shader Variant (T): Toon Shader Sun/Moon")) {
				toonShader++;
			}
			break;
		case 3:
			if (ImGui::Button("Toon Shader Variant (T): Toon Shader Moon/Bulb")) {
				toonShader = 0;
			}
			break;
		default:
			break;
		}
		switch (camPos) {
			case 0:
				if (ImGui::Button("Camera Position (C): General shot")) {
					camPos++;
				}
				break;
			case 1:
				if (ImGui::Button("Camera Position (C): Shot counter shot")) {
					camPos++;
				}
				break;
			case 2:
				if (ImGui::Button("Camera Position (C): Lateral view")) {
					camPos++;
				}
				break;
			case 3:
				if (ImGui::Button("Camera Position (C): Rotating god’s eye shot")) {
					camPos = 0;
				}
				break;
			default:
				break;
		}
		if (ImGui::Button("Model transition (M)")) {
			showModels = !showModels;
		}
		if (ImGui::Button("Two Wheels Scene WIP (S)")) {
			exercice = 8;
		}
	}
	// .........................

	ImGui::End();

	// Example code -- ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 60), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

///////// fw decl
namespace ImGui {
	void Render();
}
namespace Box {
	void setupCube();
	void cleanupCube();
	void drawCube();
}
namespace Axis {
	void setupAxis();
	void cleanupAxis();
	void drawAxis();
}

namespace models3D {

	struct model {

	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		glm::vec3 color;

		glm::mat4 objMat = glm::mat4(1.0f);

		GLuint vao;
		GLuint vbo[2];
		GLuint shaders[2];
		GLuint program;

	};

	model create(std::string name, float scale, glm::vec3 initColor);
	void cleanUp(model aModel);
	void draw(model aModel);
	void drawCabins(model cabin, model trump, model chicken, float currentTime);
	void draw2Wheels(float currentTime);

	model trump, chicken, cabin, wheel, base;

}

namespace Sphere {

	struct object {

	public:

		glm::vec3 color;

		glm::vec3 pos;

		glm::mat4 objMat = glm::mat4(1.0f);

		GLuint vao;
		GLuint vbo;
		GLuint shaders[3];
		GLuint program;
		float radius;
		float intensity;

	};

	object create(float radius, glm::vec3 pos, glm::vec3 initColor, float intensity);
	void cleanup(object obj);
	void update(object obj);
	void draw(object obj);

	object sun, moon, bulb;
}

namespace Cube {
	void mySetupCube();
	void myCleanupCube();
	void myUpdateCube(const glm::mat4& transform);
	void myDrawCube();
	void myDrawCubesWheel(double currentTime);
}



////////////////

namespace RenderVars {
	const float FOV = glm::radians(65.f);
	float zNear = 0.5f;
	const float zFar = 500.f;

	glm::mat4 _projection;
	glm::mat4 _modelView;
	glm::mat4 _MVP;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	float panv[3] = { 0.f, 0.f, 0.f };
	float rota[2] = { 0.f, 0.f };
	
}
namespace RV = RenderVars;

void GLResize(int width, int height) {
	glViewport(0, 0, width, height);
	if (height != 0) RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);
	else RV::_projection = glm::perspective(RV::FOV, 0.f, RV::zNear, RV::zFar);
}

void GLmousecb(MouseEvent ev) {
	if (RV::prevMouse.waspressed && RV::prevMouse.button == ev.button) {
		float diffx = ev.posx - RV::prevMouse.lastx;
		float diffy = ev.posy - RV::prevMouse.lasty;
		switch (ev.button) {
		case MouseEvent::Button::Left: // ROTATE
			RV::rota[0] += diffx * 0.005f;
			RV::rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: // MOVE XY
			RV::panv[0] += diffx * 0.03f;
			RV::panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: // MOVE Z
			RV::panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	}
	else {
		RV::prevMouse.button = ev.button;
		RV::prevMouse.waspressed = true;
	}
	RV::prevMouse.lastx = ev.posx;
	RV::prevMouse.lasty = ev.posy;
}

void GLinit(int width, int height) {
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);

	exercice = 1;
	camPos = 0;
	timer_camera = 0;
	timer_sun = 0;
	timer_moon = 0;

	day = true;
	bombilla = 0;
	toonShader = 0;
	showModels = false;

	models3D::trump = models3D::create("trump.obj", 0.027f, glm::vec3(1.0f, 0.5f, 0.2f));
	models3D::chicken = models3D::create("chicken.obj", 0.027f, glm::vec3(1.0f, 1.0f, 0.2f));
	models3D::cabin = models3D::create("cabin.obj", 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));
	models3D::wheel = models3D::create("wheel.obj", 0.009f, glm::vec3(1.0f, 1.0f, 1.0f));
	models3D::base = models3D::create("base.obj", 0.02f, glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 rotation_wheel = glm::rotate(glm::mat4(), 90.f * 3.14f / 180.f, glm::vec3(0.0f, 1.0f, 0.0f));
	models3D::wheel.objMat = rotation_wheel * models3D::wheel.objMat;
	models3D::base.objMat = rotation_wheel * models3D::base.objMat;

	Sphere::sun = Sphere::create(1.0f, glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 80000.f);
	Sphere::moon = Sphere::create(1.0f, glm::vec3(-100.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 80000.f);
	Sphere::bulb = Sphere::create(0.1f, glm::vec3(.0f, 0.0f, 0.0f), glm::vec3(.0f, .85f, .45f), 5000.f);

	Cube::mySetupCube();

}

void GLcleanup() {
	/*Box::cleanupCube();
	Axis::cleanupAxis();*/

	models3D::cleanUp(models3D::trump);
	models3D::cleanUp(models3D::chicken);
	models3D::cleanUp(models3D::cabin);
	models3D::cleanUp(models3D::wheel);
	models3D::cleanUp(models3D::base);

	Sphere::cleanup(Sphere::sun);
	Sphere::cleanup(Sphere::moon);
	Sphere::cleanup(Sphere::bulb);

	Cube::myCleanupCube();

}

void GLrender(double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureKeyboard) {
		if (ImGui::IsKeyPressed('m', false)) {
			showModels = !showModels;
		}
		if (ImGui::IsKeyPressed('c', false)) {
			camPos++;
			if (camPos > 3) camPos = 0;
		}
		if (ImGui::IsKeyPressed('d', false)) {
			day = !day;
		}
		if (ImGui::IsKeyPressed('b', false)) {
			bombilla++;
			if (bombilla > 2) bombilla = 0;
		}
		if (ImGui::IsKeyPressed('t', false)) {
			toonShader++;
			if (toonShader > 3) toonShader = 0;
		}
		if (ImGui::IsKeyPressed('a', false)) {
			exercice++;
			setVariables(exercice);
		}
		if (ImGui::IsKeyPressed('z', false)) {
			exercice--;
			setVariables(exercice);
		}
	}

	RV::_modelView = glm::mat4(1.f);
	switch (camPos) {
		case 0:
			RV::_modelView = glm::translate(glm::mat4(), glm::vec3(0.f, -5.f, -150.f)) * glm::rotate(glm::mat4(), -30.f * 3.14159f / 180.f, glm::vec3(0.f, 1.f, 0.f));
			//RV::_modelView = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -175.f));
			break;
		case 1:
			if (currentTime - timer_camera < 2) {
				RV::_modelView = glm::translate(glm::mat4(), glm::vec3(0.f, radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * cabinNum) / totalCabins)), radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * cabinNum) / totalCabins)) - 1));
			} else if(currentTime - timer_camera < 4) {
				RV::_modelView = glm::rotate(glm::mat4(), 180.f * 3.14159f / 180.f, glm::vec3(0.f, 1.f, 0.f)) * glm::translate(glm::mat4(), glm::vec3(0.f, radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * cabinNum) / totalCabins)), radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * cabinNum) / totalCabins)) + 1));
			} else {
				timer_camera = currentTime;
			}
			break;
		case 2:
			RV::_modelView = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -100.f)) * glm::rotate(glm::mat4(), -90.f * 3.14159f / 180.f, glm::vec3(0.f, 1.f, 0.f));
			break;
		case 3:
			RV::_modelView = glm::rotate(glm::mat4(), (float)currentTime * 0.5f , glm::vec3(0.f, 0.f, 1.f)) * glm::rotate(glm::mat4(), 90.f * 3.14159f / 180.f, glm::vec3(1.f, 0.f, 0.f)) * glm::translate(glm::mat4(), glm::vec3(0.f, radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * cabinNum) / totalCabins)) - 2, radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * cabinNum) / totalCabins))));
			break;
		default:
			break;
	}


	/*RV::_modelView = glm::mat4(1.f);
	RV::_modelView = glm::translate(RV::_modelView, glm::vec3(RV::panv[0], RV::panv[1], RV::panv[2]));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[1], glm::vec3(1.f, 0.f, 0.f));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[0], glm::vec3(0.f, 1.f, 0.f));*/

	RV::_MVP = RV::_projection * RV::_modelView;

	// render code
	/*Box::drawCube();
	Axis::drawAxis();*/

	if (day) {
		float angle_sun = ((currentTime - timer_sun) / 20.0f * 360.0f) * 3.14159 / 180;
		float angle_moon = ((currentTime - timer_moon) / 18.0f * 360.0f) * 3.14159 / 180;

		if (angle_sun >= 6) {
			timer_sun = currentTime;
		}
		else if (angle_sun >= 3) {
			Sphere::sun.color = glm::vec3(0.f, 0.f, 0.f);
		}
		else if (angle_sun >= 0) {
			Sphere::sun.color = glm::vec3(abs(cos(angle_sun)) * 0.25f + 0.75f, abs(sin(angle_sun)) * 0.5f + 0.25f, 0.f);
		}

		if (angle_moon >= 6.25) {
			timer_moon = currentTime;
		}
		else if (angle_moon >= 3) {
			Sphere::moon.color = glm::vec3(0.f, 0.f, 0.5f);
		}
		else if (angle_moon >= 0) {
			Sphere::moon.color = glm::vec3(0.f, 0.f, 0.f);
		}

		Sphere::sun.pos = glm::vec3(75 * cos(angle_sun), 75 * sin(angle_sun), 0);
		Sphere::moon.pos = glm::vec3(-75 * cos(angle_moon), -75 * sin(angle_moon), 0);
		Sphere::moon.pos = glm::vec4(Sphere::moon.pos, 1.0f) * glm::rotate(glm::mat4(), 135.0f * 3.14f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		Sphere::bulb.color = glm::vec3(0.f, 0.f, 0.f);

	} else {
		Sphere::sun.color = glm::vec3(0.f, 0.f, 0.f);
		Sphere::moon.color = glm::vec3(0.f, 0.f, 0.5f);

		Sphere::sun.pos = glm::vec3(75 * cos(5), 75 * sin(5), 0);
		Sphere::moon.pos = glm::vec3(-75 * cos(5), -75 * sin(5), 0);

		if (bombilla > 0) {

			Sphere::bulb.color = glm::vec3(.0f, .85f, .45f);

			if (bombilla == 1) {
				Sphere::bulb.pos = glm::vec3(0.f, radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * 19) / totalCabins)) + 0.4f, radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * 19) / totalCabins)));
			} else {
				Sphere::bulb.pos = glm::vec3(sin(currentTime) * 0.5, radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * 19) / totalCabins)) + 0.4f - abs(cos(currentTime)) * 0.5f, radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f * 19) / totalCabins)));
			}

			Sphere::update(Sphere::bulb);
			Sphere::draw(Sphere::bulb);
		}

	}
	/*if (currentTime - timer_day < 10) { //DAY
		Sphere::sun.color = glm::vec3(abs(cos(angle_sun)) * 0.25f + 0.75f, abs(sin(angle_sun)) * 0.5f + 0.25f, 0.f);
		Sphere::moon.color = glm::vec3(0.f, 0.f, 0.f);
	} else if (currentTime - timer_day < 20) { //NIGHT
		Sphere::sun.color = glm::vec3(0.f, 0.f, 0.f);
		Sphere::moon.color = glm::vec3(0.f, 0.f, 0.5f);
	} else {
		timer_day = currentTime;
	}*/

	Sphere::update(Sphere::sun);
	Sphere::draw(Sphere::sun);

	Sphere::update(Sphere::moon);
	Sphere::draw(Sphere::moon);

	if (exercice == 8) {

		models3D::draw2Wheels(currentTime);

	} else {

		if (showModels) {

			models3D::drawCabins(models3D::cabin, models3D::trump, models3D::chicken, currentTime);

			glm::mat4 rotation_wheel = glm::rotate(glm::mat4(), -1.18f * 3.14f / 180.f, glm::vec3(1.0f, 0.0f, 0.0f));
			models3D::wheel.objMat = rotation_wheel * models3D::wheel.objMat;

			models3D::draw(models3D::wheel);
			models3D::draw(models3D::base);
		}
		else {

			Cube::myDrawCubesWheel(currentTime);
		}
	}

	ImGui::Render();
}


//////////////////////////////////// COMPILE AND LINK
GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name = "") {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStr, NULL);
	glCompileShader(shader);
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetShaderInfoLog(shader, res, &res, buff);
		fprintf(stderr, "Error Shader %s: %s", name, buff);
		delete[] buff;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
void linkProgram(GLuint program) {
	glLinkProgram(program);
	GLint res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (res == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetProgramInfoLog(program, res, &res, buff);
		fprintf(stderr, "Error Link: %s", buff);
		delete[] buff;
	}
}

////////////////////////////////////////////////// BOX
namespace Box {
	GLuint cubeVao;
	GLuint cubeVbo[2];
	GLuint cubeShaders[2];
	GLuint cubeProgram;

	float cubeVerts[] = {
		// -5,0,-5 -- 5, 10, 5
		-5.f,  0.f, -5.f,
		5.f,  0.f, -5.f,
		5.f,  0.f,  5.f,
		-5.f,  0.f,  5.f,
		-5.f, 10.f, -5.f,
		5.f, 10.f, -5.f,
		5.f, 10.f,  5.f,
		-5.f, 10.f,  5.f,
	};
	GLubyte cubeIdx[] = {
		1, 0, 2, 3, // Floor - TriangleStrip
		0, 1, 5, 4, // Wall - Lines
		1, 2, 6, 5, // Wall - Lines
		2, 3, 7, 6, // Wall - Lines
		3, 0, 4, 7  // Wall - Lines
	};

	const char* vertShader_xform =
		"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* fragShader_flatColor =
		"#version 330\n\
out vec4 out_Color;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = color;\n\
}";

	void setupCube() {
		glGenVertexArrays(1, &cubeVao);
		glBindVertexArray(cubeVao);
		glGenBuffers(2, cubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 20, cubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cubeShaders[0] = compileShader(vertShader_xform, GL_VERTEX_SHADER, "cubeVert");
		cubeShaders[1] = compileShader(fragShader_flatColor, GL_FRAGMENT_SHADER, "cubeFrag");

		cubeProgram = glCreateProgram();
		glAttachShader(cubeProgram, cubeShaders[0]);
		glAttachShader(cubeProgram, cubeShaders[1]);
		glBindAttribLocation(cubeProgram, 0, "in_Position");
		linkProgram(cubeProgram);
	}
	void cleanupCube() {
		glDeleteBuffers(2, cubeVbo);
		glDeleteVertexArrays(1, &cubeVao);

		glDeleteProgram(cubeProgram);
		glDeleteShader(cubeShaders[0]);
		glDeleteShader(cubeShaders[1]);
	}
	void drawCube() {
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		// FLOOR
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.6f, 0.6f, 0.6f, 1.f);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
		// WALLS
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.f, 0.f, 0.f, 1.f);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 4));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 8));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 12));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 16));

		glUseProgram(0);
		glBindVertexArray(0);
	}

	void drawCubesWheel() {
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		// FLOOR
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.6f, 0.6f, 0.6f, 1.f);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
		// WALLS
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.f, 0.f, 0.f, 1.f);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 4));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 8));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 12));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 16));

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// AXIS
namespace Axis {
	GLuint AxisVao;
	GLuint AxisVbo[3];
	GLuint AxisShader[2];
	GLuint AxisProgram;

	float AxisVerts[] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0
	};
	float AxisColors[] = {
		1.0, 0.0, 0.0, 1.0,
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		0.0, 0.0, 1.0, 1.0
	};
	GLubyte AxisIdx[] = {
		0, 1,
		2, 3,
		4, 5
	};
	const char* Axis_vertShader =
		"#version 330\n\
in vec3 in_Position;\n\
in vec4 in_Color;\n\
out vec4 vert_color;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	vert_color = in_Color;\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* Axis_fragShader =
		"#version 330\n\
in vec4 vert_color;\n\
out vec4 out_Color;\n\
void main() {\n\
	out_Color = vert_color;\n\
}";

	void setupAxis() {
		glGenVertexArrays(1, &AxisVao);
		glBindVertexArray(AxisVao);
		glGenBuffers(3, AxisVbo);

		glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisColors, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AxisVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, AxisIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		AxisShader[0] = compileShader(Axis_vertShader, GL_VERTEX_SHADER, "AxisVert");
		AxisShader[1] = compileShader(Axis_fragShader, GL_FRAGMENT_SHADER, "AxisFrag");

		AxisProgram = glCreateProgram();
		glAttachShader(AxisProgram, AxisShader[0]);
		glAttachShader(AxisProgram, AxisShader[1]);
		glBindAttribLocation(AxisProgram, 0, "in_Position");
		glBindAttribLocation(AxisProgram, 1, "in_Color");
		linkProgram(AxisProgram);
	}
	void cleanupAxis() {
		glDeleteBuffers(3, AxisVbo);
		glDeleteVertexArrays(1, &AxisVao);

		glDeleteProgram(AxisProgram);
		glDeleteShader(AxisShader[0]);
		glDeleteShader(AxisShader[1]);
	}
	void drawAxis() {
		glBindVertexArray(AxisVao);
		glUseProgram(AxisProgram);
		glUniformMatrix4fv(glGetUniformLocation(AxisProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// SPHERE
namespace Sphere {

	const char* sphere_vertShader =
		"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mv_Mat;\n\
void main() {\n\
	gl_Position = mv_Mat * vec4(in_Position, 1.0);\n\
}";
	const char* sphere_geomShader =
		"#version 330\n\
layout(points) in;\n\
layout(triangle_strip, max_vertices = 4) out;\n\
out vec4 eyePos;\n\
out vec4 centerEyePos;\n\
uniform mat4 projMat;\n\
uniform float radius;\n\
vec4 nu_verts[4];\n\
void main() {\n\
	vec3 n = normalize(-gl_in[0].gl_Position.xyz);\n\
	vec3 up = vec3(0.0, 1.0, 0.0);\n\
	vec3 u = normalize(cross(up, n));\n\
	vec3 v = normalize(cross(n, u));\n\
	nu_verts[0] = vec4(-radius*u - radius*v, 0.0); \n\
	nu_verts[1] = vec4( radius*u - radius*v, 0.0); \n\
	nu_verts[2] = vec4(-radius*u + radius*v, 0.0); \n\
	nu_verts[3] = vec4( radius*u + radius*v, 0.0); \n\
	centerEyePos = gl_in[0].gl_Position;\n\
	for (int i = 0; i < 4; ++i) {\n\
		eyePos = (gl_in[0].gl_Position + nu_verts[i]);\n\
		gl_Position = projMat * eyePos;\n\
		EmitVertex();\n\
	}\n\
	EndPrimitive();\n\
}";
	const char* sphere_fragShader_flatColor =
		"#version 330\n\
in vec4 eyePos;\n\
in vec4 centerEyePos;\n\
out vec4 out_Color;\n\
uniform mat4 projMat;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
uniform float radius;\n\
void main() {\n\
	vec4 diff = eyePos - centerEyePos;\n\
	float distSq2C = dot(diff, diff);\n\
	if (distSq2C > (radius*radius)) discard;\n\
	float h = sqrt(radius*radius - distSq2C);\n\
	vec4 nuEyePos = vec4(eyePos.xy, eyePos.z + h, 1.0);\n\
	vec4 nuPos = projMat * nuEyePos;\n\
	gl_FragDepth = ((nuPos.z / nuPos.w) + 1) * 0.5;\n\
	vec3 normal = normalize(nuEyePos - centerEyePos).xyz;\n\
	out_Color = vec4(color.xyz * dot(normal, (mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)).xyz) + color.xyz * 0.3, 1.0 );\n\
}";


	object create(float radius, glm::vec3 pos, glm::vec3 initColor, float intensity) {

		object newObject;
		newObject.color = initColor;
		newObject.radius = radius;
		newObject.pos = pos;
		newObject.intensity = intensity;

		glGenVertexArrays(1, &newObject.vao);
		glBindVertexArray(newObject.vao);
		glGenBuffers(1, &newObject.vbo);

		glBindBuffer(GL_ARRAY_BUFFER, newObject.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &pos, GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		newObject.shaders[0] = compileShader(sphere_vertShader, GL_VERTEX_SHADER, "sphereVert");
		newObject.shaders[1] = compileShader(sphere_geomShader, GL_GEOMETRY_SHADER, "sphereGeom");
		newObject.shaders[2] = compileShader(sphere_fragShader_flatColor, GL_FRAGMENT_SHADER, "sphereFrag");

		newObject.program = glCreateProgram();
		glAttachShader(newObject.program, newObject.shaders[0]);
		glAttachShader(newObject.program, newObject.shaders[1]);
		glAttachShader(newObject.program, newObject.shaders[2]);
		glBindAttribLocation(newObject.program, 0, "in_Position");
		linkProgram(newObject.program);

		return newObject;

	}

	void cleanup(object obj) {
		glDeleteBuffers(1, &obj.vbo);
		glDeleteVertexArrays(1, &obj.vao);

		glDeleteProgram(obj.program);
		glDeleteShader(obj.shaders[0]);
		glDeleteShader(obj.shaders[1]);
		glDeleteShader(obj.shaders[2]);

	}
	void update(object obj) {
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		float* buff = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		buff[0] = obj.pos.x;
		buff[1] = obj.pos.y;
		buff[2] = obj.pos.z;
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void draw(object obj) {
		glBindVertexArray(obj.vao);
		glUseProgram(obj.program);

		glUniformMatrix4fv(glGetUniformLocation(obj.program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(obj.program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RV::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(obj.program, "projMat"), 1, GL_FALSE, glm::value_ptr(RV::_projection));
		glUniform4f(glGetUniformLocation(obj.program, "color"), obj.color.r, obj.color.g, obj.color.b, 1.f);
		glUniform1f(glGetUniformLocation(obj.program, "radius"), obj.radius);
		glDrawArrays(GL_POINTS, 0, 1);

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// models3D
namespace models3D {

	const char* models3D_vertShader =
	"#version 330\n\
	in vec3 in_Position;\n\
	in vec3 in_Normal;\n\
	out vec4 vert_Normal;\n\
	uniform mat4 objMat;\n\
	uniform mat4 mv_Mat;\n\
	uniform mat4 mvpMat;\n\
	uniform vec3 sunPos;\n\
	out float sunDiffuse;\n\
	uniform vec3 moonPos;\n\
	out float moonDiffuse;\n\
	uniform vec3 bulbPos;\n\
	out float bulbDiffuse;\n\
	void main() {\n\
		gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);\n\
		vert_Normal = mv_Mat * objMat * vec4(in_Normal, 0.0);\n\
		vec4 newPosition = objMat * vec4(in_Position, 1.0);\n\
		vec4 newNormal = vec4(in_Normal, 1.0);\n\
		float sunDistance = distance(sunPos, newPosition.xyz);\n\
		vec4 sunDir = vec4(normalize(sunPos - newPosition.xyz), 1.0);\n\
		sunDiffuse = dot(newNormal, sunDir) / (4 * 3.14 * sunDistance * sunDistance);\n\
		float moonDistance = distance(moonPos, newPosition.xyz);\n\
		vec4 moonDir = vec4(normalize(moonPos - newPosition.xyz), 1.0);\n\
		moonDiffuse = dot(newNormal, moonDir) / (4 * 3.14 * moonDistance * moonDistance);\n\
		float bulbDistance = distance(bulbPos, newPosition.xyz);\n\
		vec4 bulbDir = vec4(normalize(bulbPos - newPosition.xyz), 1.0);\n\
		bulbDiffuse = dot(newNormal, bulbDir) / (4 * 3.14 * bulbDistance * bulbDistance);\n\
	}";

	/*const char* models3D_fragShader =
	"#version 330\n\
	in vec4 vert_Normal;\n\
	out vec4 out_Color;\n\
	uniform mat4 mv_Mat;\n\
	uniform vec4 color;\n\
	uniform vec3 sunColor;\n\
	uniform float sunIntensity;\n\
	in float sunDiffuse;\n\
	uniform vec3 moonColor;\n\
	in float moonDiffuse;\n\
	uniform vec3 bulbColor;\n\
	in float bulbDiffuse;\n\
	uniform vec3 ambientColor;\n\
	void main() {\n\
		out_Color = vec4(color.xyz * sunColor * (sunDiffuse * sunIntensity) + color.xyz * moonColor * (moonDiffuse * sunIntensity) + color.xyz * bulbColor * (bulbDiffuse * 50) + color.xyz * ambientColor, 1.0);\n\
	}";*/

	const char* models3D_fragToonShader =
	"#version 330\n\
	in vec4 vert_Normal;\n\
	out vec4 out_Color;\n\
	uniform mat4 mv_Mat;\n\
	uniform vec4 color;\n\
	uniform vec3 sunColor;\n\
	uniform float sunIntensity;\n\
	in float sunDiffuse;\n\
	uniform vec3 moonColor;\n\
	in float moonDiffuse;\n\
	uniform vec3 bulbColor;\n\
	in float bulbDiffuse;\n\
	uniform vec3 ambientColor;\n\
	uniform int toonShaderOption;\n\
	void main() {\n\
		float sunDiff = sunDiffuse;\n\
		float moonDiff = moonDiffuse;\n\
		float bulbDiff = bulbDiffuse;\n\
		if(toonShaderOption == 1 || toonShaderOption == 2) { if(sunDiffuse * sunIntensity < 0.2) { sunDiff = 0.0; } else if(sunDiffuse * sunIntensity < 0.7) { sunDiff = 0.5; } else { sunDiff = 1.0; } }\n\
		if(toonShaderOption == 2 || toonShaderOption == 3) { if(moonDiffuse * sunIntensity < 0.2) { moonDiff = 0.0; } else if(moonDiffuse * sunIntensity < 0.7) { moonDiff = 0.5; } else { moonDiff = 1.0; } }\n\
		if(toonShaderOption == 3) { if(bulbDiffuse * 50 < 0.2) { bulbDiff = 0.0; } else if(bulbDiffuse * 50 < 0.7) { bulbDiff = 0.5; } else { bulbDiff = 1.0; } }\n\
		if(toonShaderOption == 1) { moonDiff = 0.0; bulbDiff = 0.0; }\n\
		if(toonShaderOption == 2) { bulbDiff = 0.0; }\n\
		if(toonShaderOption == 3) { sunDiff = 0.0; }\n\
		out_Color = vec4(color.xyz * sunColor * (sunDiff * sunIntensity) + color.xyz * moonColor * (moonDiff * sunIntensity) + color.xyz * bulbColor * (bulbDiff * 50) + color.xyz * ambientColor, 1.0);\n\
	}";

	model create(std::string name, float scale, glm::vec3 initColor) {

		model newModel;
		newModel.color = initColor;
		newModel.objMat = glm::scale(glm::mat4(), glm::vec3(scale));

		bool res = loadOBJ(name.c_str(), newModel.vertices, newModel.uvs, newModel.normals);

		if (res) {
			std::cout << "Modelo " << name.c_str() << "cargado correctamente\n";
		}
		else {
			std::cout << "Error al cargar el modelo " << name.c_str() << std::endl;
		}

		glGenVertexArrays(1, &newModel.vao);
		glBindVertexArray(newModel.vao);
		glGenBuffers(2, newModel.vbo);

		glBindBuffer(GL_ARRAY_BUFFER, newModel.vbo[0]);

		glBufferData(GL_ARRAY_BUFFER, newModel.vertices.size() * sizeof(glm::vec3), &newModel.vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, newModel.vbo[1]);

		glBufferData(GL_ARRAY_BUFFER, newModel.normals.size() * sizeof(glm::vec3), &newModel.normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		newModel.shaders[0] = compileShader(models3D_vertShader, GL_VERTEX_SHADER, "objectVert");
		newModel.shaders[1] = compileShader(models3D_fragToonShader, GL_FRAGMENT_SHADER, "objectFragToonShader");

		newModel.program = glCreateProgram();

		glAttachShader(newModel.program, newModel.shaders[0]);
		glAttachShader(newModel.program, newModel.shaders[1]);

		glBindAttribLocation(newModel.program, 0, "in_Position");
		glBindAttribLocation(newModel.program, 1, "in_Normal");

		linkProgram(newModel.program);

		return newModel;

	}

	void cleanUp(model aModel) {

		glDeleteBuffers(2, aModel.vbo);
		glDeleteVertexArrays(1, &aModel.vao);

		glDeleteProgram(aModel.program);

		glDeleteShader(aModel.shaders[0]);
		glDeleteShader(aModel.shaders[1]);

	}

	void draw(model aModel) {

		glBindVertexArray(aModel.vao);

		glUseProgram(aModel.program);

		glUniformMatrix4fv(glGetUniformLocation(aModel.program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(aModel.program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniform4f(glGetUniformLocation(aModel.program, "color"), aModel.color.x, aModel.color.y, aModel.color.z, 0.f);
		glUniform3f(glGetUniformLocation(aModel.program, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(aModel.program, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(aModel.program, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(aModel.program, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(aModel.program, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(aModel.program, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(aModel.program, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(aModel.program, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(aModel.program, "toonShaderOption"), toonShader);

		glUniformMatrix4fv(glGetUniformLocation(aModel.program, "objMat"), 1, GL_FALSE, glm::value_ptr(aModel.objMat));
		glDrawArrays(GL_TRIANGLES, 0, aModel.vertices.size());

		//std::cout << aModel.vertices.size() << std::endl;

		glUseProgram(0);
		glBindVertexArray(0);

	}

	void drawCabins(model cabin, model trump, model chicken, float currentTime) {

		//Cabin
		glBindVertexArray(cabin.vao);
		glUseProgram(cabin.program);

		glUniformMatrix4fv(glGetUniformLocation(cabin.program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(cabin.program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniform4f(glGetUniformLocation(cabin.program, "color"), cabin.color.x, cabin.color.y, cabin.color.z, 0.f);
		glUniform3f(glGetUniformLocation(cabin.program, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(cabin.program, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(cabin.program, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(cabin.program, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(cabin.program, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(cabin.program, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(cabin.program, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(cabin.program, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(cabin.program, "toonShaderOption"), toonShader);

		float posX, posY;
		glm::mat4 objMatModel;
		glm::mat4 translation_cabin;

		for (int i = 0; i < totalCabins; i++) {

			posX = radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*i) / totalCabins));
			posY = radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*i) / totalCabins));

			translation_cabin = glm::translate(glm::mat4(), glm::vec3(0.f, posY, posX));
			glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.f, posY + 4.0f, posX));
			objMatModel = translation * cabin.objMat;

			glUniformMatrix4fv(glGetUniformLocation(cabin.program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMatModel));
			glDrawArrays(GL_TRIANGLES, 0, cabin.vertices.size());

		}

		//Trump
		glBindVertexArray(trump.vao);
		glUseProgram(trump.program);

		glUniformMatrix4fv(glGetUniformLocation(trump.program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(trump.program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniform4f(glGetUniformLocation(trump.program, "color"), trump.color.x, trump.color.y, trump.color.z, 0.f);
		glUniform3f(glGetUniformLocation(trump.program, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(trump.program, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(trump.program, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(trump.program, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(trump.program, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(trump.program, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(trump.program, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(trump.program, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(trump.program, "toonShaderOption"), toonShader);

		glm::mat4 translation_trump = glm::translate(glm::mat4(), glm::vec3(0.f, -1.0f, 2.0f));
		glm::mat4 rotation_trump = glm::rotate(glm::mat4(), 90.f * 3.14f / 180.f, glm::vec3(0.0f, 1.0f, 0.0f));
		objMatModel = translation_trump * translation_cabin * rotation_trump * trump.objMat;

		glUniformMatrix4fv(glGetUniformLocation(trump.program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMatModel));
		glDrawArrays(GL_TRIANGLES, 0, trump.vertices.size());


		//Chicken
		glBindVertexArray(chicken.vao);
		glUseProgram(chicken.program);

		glUniformMatrix4fv(glGetUniformLocation(chicken.program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(chicken.program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniform4f(glGetUniformLocation(chicken.program, "color"), chicken.color.x, chicken.color.y, chicken.color.z, 0.f);
		glUniform3f(glGetUniformLocation(chicken.program, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(chicken.program, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(chicken.program, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(chicken.program, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(chicken.program, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(chicken.program, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(chicken.program, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(chicken.program, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(chicken.program, "toonShaderOption"), toonShader);

		glm::mat4 translation_chicken = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -1.5f));
		glm::mat4 rotation_chicken = glm::rotate(glm::mat4(), 90.f * 3.14f / 180.f, glm::vec3(0.0f, 1.0f, 0.0f));
		objMatModel = translation_chicken * translation_cabin * rotation_chicken * chicken.objMat;

		glUniformMatrix4fv(glGetUniformLocation(chicken.program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMatModel));
		glDrawArrays(GL_TRIANGLES, 0, chicken.vertices.size());

		//std::cout << aModel.vertices.size() << std::endl;

		glUseProgram(0);
		glBindVertexArray(0);

	}

	void draw2Wheels(float currentTime) {

		//Wheels
		glBindVertexArray(wheel.vao);
		glUseProgram(wheel.program);

		glUniformMatrix4fv(glGetUniformLocation(wheel.program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(wheel.program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniform4f(glGetUniformLocation(wheel.program, "color"), wheel.color.x, wheel.color.y, wheel.color.z, 0.f);
		glUniform3f(glGetUniformLocation(wheel.program, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(wheel.program, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(wheel.program, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(wheel.program, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(wheel.program, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(wheel.program, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(wheel.program, "toonShaderOption"), 0);

		//Wheel1
		glm::mat4 translation_wheel1 = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 50.f));
		glm::mat4 rotation_wheel1 = glm::rotate(glm::mat4(), -1.18f * 3.14f / 180.f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 objMatModel = translation_wheel1 * rotation_wheel1 * wheel.objMat;

		glUniformMatrix4fv(glGetUniformLocation(wheel.program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMatModel));
		glDrawArrays(GL_TRIANGLES, 0, wheel.vertices.size());

		//Wheel2
		glm::mat4 translation_wheel2 = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -50.f));
		glm::mat4 rotation_wheel2 = glm::rotate(glm::mat4(), 1.18f * 3.14f / 180.f, glm::vec3(1.0f, 0.0f, 0.0f));
		objMatModel = translation_wheel2 * rotation_wheel2 * wheel.objMat;

		glUniformMatrix4fv(glGetUniformLocation(wheel.program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMatModel));
		glDrawArrays(GL_TRIANGLES, 0, wheel.vertices.size());

		//Cabin
		glBindVertexArray(cabin.vao);
		glUseProgram(cabin.program);

		glUniformMatrix4fv(glGetUniformLocation(cabin.program, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(cabin.program, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniform4f(glGetUniformLocation(cabin.program, "color"), cabin.color.x, cabin.color.y, cabin.color.z, 0.f);
		glUniform3f(glGetUniformLocation(cabin.program, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(cabin.program, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(cabin.program, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(cabin.program, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(cabin.program, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(cabin.program, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(cabin.program, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(cabin.program, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(cabin.program, "toonShaderOption"), toonShader);

		float posX, posY;
		glm::mat4 translation_cabin;

		for (int i = 0; i < totalCabins; i++) {

			posX = radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*i) / totalCabins));
			posY = radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*i) / totalCabins));

			translation_cabin = glm::translate(glm::mat4(), glm::vec3(0.f, posY, posX + 50.f));
			glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.f, posY + 4.0f, posX + 50.f));
			objMatModel = translation * cabin.objMat;

			glUniformMatrix4fv(glGetUniformLocation(cabin.program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMatModel));
			glDrawArrays(GL_TRIANGLES, 0, cabin.vertices.size());


			translation_cabin = glm::translate(glm::mat4(), glm::vec3(0.f, posY, posX - 50.f));
			translation = glm::translate(glm::mat4(), glm::vec3(0.f, posY + 4.0f, posX - 50.f));
			objMatModel = translation * cabin.objMat;

			glUniformMatrix4fv(glGetUniformLocation(cabin.program, "objMat"), 1, GL_FALSE, glm::value_ptr(objMatModel));
			glDrawArrays(GL_TRIANGLES, 0, cabin.vertices.size());

		}

		//std::cout << aModel.vertices.size() << std::endl;

		glUseProgram(0);
		glBindVertexArray(0);

	}

}

////////////////////////////////////////////////// CUBE
namespace Cube {
	GLuint myCubeVao;
	GLuint myCubeVbo[3];
	GLuint myCubeShaders[2];
	GLuint myCubeProgram;
	glm::mat4 myObjMat = glm::mat4(1.f);

	extern const float myHalfW = 0.5f;
	int myNumVerts = 24 + 6; // 4 vertex/face * 6 faces + 6 PRIMITIVE RESTART

							 //   4---------7
							 //  /|        /|
							 // / |       / |
							 //5---------6  |
							 //|  0------|--3
							 //| /       | /
							 //|/        |/
							 //1---------2
	glm::vec3 myVerts[] = {
		glm::vec3(-myHalfW, -myHalfW, -myHalfW),
		glm::vec3(-myHalfW, -myHalfW,  myHalfW),
		glm::vec3(myHalfW, -myHalfW,  myHalfW),
		glm::vec3(myHalfW, -myHalfW, -myHalfW),
		glm::vec3(-myHalfW,  myHalfW, -myHalfW),
		glm::vec3(-myHalfW,  myHalfW,  myHalfW),
		glm::vec3(myHalfW,  myHalfW,  myHalfW),
		glm::vec3(myHalfW,  myHalfW, -myHalfW)
	};
	glm::vec3 myNorms[] = {
		glm::vec3(0.f, -1.f,  0.f),
		glm::vec3(0.f,  1.f,  0.f),
		glm::vec3(-1.f,  0.f,  0.f),
		glm::vec3(1.f,  0.f,  0.f),
		glm::vec3(0.f,  0.f, -1.f),
		glm::vec3(0.f,  0.f,  1.f)
	};

	glm::vec3 myCubeVerts[] = {
		myVerts[1], myVerts[0], myVerts[2], myVerts[3],
		myVerts[5], myVerts[6], myVerts[4], myVerts[7],
		myVerts[1], myVerts[5], myVerts[0], myVerts[4],
		myVerts[2], myVerts[3], myVerts[6], myVerts[7],
		myVerts[0], myVerts[4], myVerts[3], myVerts[7],
		myVerts[1], myVerts[2], myVerts[5], myVerts[6]
	};
	glm::vec3 myCubeNorms[] = {
		myNorms[0], myNorms[0], myNorms[0], myNorms[0],
		myNorms[1], myNorms[1], myNorms[1], myNorms[1],
		myNorms[2], myNorms[2], myNorms[2], myNorms[2],
		myNorms[3], myNorms[3], myNorms[3], myNorms[3],
		myNorms[4], myNorms[4], myNorms[4], myNorms[4],
		myNorms[5], myNorms[5], myNorms[5], myNorms[5]
	};
	GLubyte myCubeIdx[] = {
		0, 1, 2, 3, UCHAR_MAX,
		4, 5, 6, 7, UCHAR_MAX,
		8, 9, 10, 11, UCHAR_MAX,
		12, 13, 14, 15, UCHAR_MAX,
		16, 17, 18, 19, UCHAR_MAX,
		20, 21, 22, 23, UCHAR_MAX
	};

	const char* myCube_vertShader =
	"#version 330\n\
	in vec3 in_Position;\n\
	in vec3 in_Normal;\n\
	out vec4 vert_Normal;\n\
	uniform mat4 objMat;\n\
	uniform mat4 mv_Mat;\n\
	uniform mat4 mvpMat;\n\
	uniform vec3 sunPos;\n\
	out float sunDiffuse;\n\
	uniform vec3 moonPos;\n\
	out float moonDiffuse;\n\
	uniform vec3 bulbPos;\n\
	out float bulbDiffuse;\n\
	void main() {\n\
		gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);\n\
		vert_Normal = mv_Mat * objMat * vec4(in_Normal, 0.0);\n\
		vec4 newPosition = objMat * vec4(in_Position, 1.0);\n\
		vec4 newNormal = vec4(in_Normal, 1.0);\n\
		float sunDistance = distance(sunPos, newPosition.xyz);\n\
		vec4 sunDir = vec4(normalize(sunPos - newPosition.xyz), 1.0);\n\
		sunDiffuse = dot(newNormal, sunDir) / (4 * 3.14 * sunDistance * sunDistance);\n\
		float moonDistance = distance(moonPos, newPosition.xyz);\n\
		vec4 moonDir = vec4(normalize(moonPos - newPosition.xyz), 1.0);\n\
		moonDiffuse = dot(newNormal, moonDir) / (4 * 3.14 * moonDistance * moonDistance);\n\
		float bulbDistance = distance(bulbPos, newPosition.xyz);\n\
		vec4 bulbDir = vec4(normalize(bulbPos - newPosition.xyz), 1.0);\n\
		bulbDiffuse = dot(newNormal, bulbDir) / (4 * 3.14 * bulbDistance * bulbDistance);\n\
	}";


	const char* myCube_fragShader =
	"#version 330\n\
	in vec4 vert_Normal;\n\
	out vec4 out_Color;\n\
	uniform mat4 mv_Mat;\n\
	uniform vec4 color;\n\
	uniform vec3 sunColor;\n\
	uniform float sunIntensity;\n\
	in float sunDiffuse;\n\
	uniform vec3 moonColor;\n\
	in float moonDiffuse;\n\
	uniform vec3 bulbColor;\n\
	in float bulbDiffuse;\n\
	uniform vec3 ambientColor;\n\
	uniform int toonShaderOption;\n\
	void main() {\n\
		float sunDiff = sunDiffuse;\n\
		float moonDiff = moonDiffuse;\n\
		float bulbDiff = bulbDiffuse;\n\
		if(toonShaderOption == 1 || toonShaderOption == 2) { if(sunDiffuse * sunIntensity < 0.2) { sunDiff = 0.0; } else if(sunDiffuse * sunIntensity < 0.7) { sunDiff = 0.5; } else { sunDiff = 1.0; } }\n\
		if(toonShaderOption == 2 || toonShaderOption == 3) { if(moonDiffuse * sunIntensity < 0.2) { moonDiff = 0.0; } else if(moonDiffuse * sunIntensity < 0.7) { moonDiff = 0.5; } else { moonDiff = 1.0; } }\n\
		if(toonShaderOption == 3) { if(bulbDiffuse * 50 < 0.2) { bulbDiff = 0.0; } else if(bulbDiffuse * 50 < 0.7) { bulbDiff = 0.5; } else { bulbDiff = 1.0; } }\n\
		if(toonShaderOption == 1) { moonDiff = 0.0; bulbDiff = 0.0; }\n\
		if(toonShaderOption == 2) { bulbDiff = 0.0; }\n\
		if(toonShaderOption == 3) { sunDiff = 0.0; }\n\
		out_Color = vec4(color.xyz * sunColor * (sunDiff * sunIntensity) + color.xyz * moonColor * (moonDiff * sunIntensity) + color.xyz * bulbColor * (bulbDiff * 50) + color.xyz * ambientColor, 1.0);\n\
	}";

	void mySetupCube() {
		glGenVertexArrays(1, &myCubeVao);
		glBindVertexArray(myCubeVao);
		glGenBuffers(3, myCubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, myCubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myCubeVerts), myCubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, myCubeVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(myCubeNorms), myCubeNorms, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glPrimitiveRestartIndex(UCHAR_MAX);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myCubeVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(myCubeIdx), myCubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		myCubeShaders[0] = compileShader(myCube_vertShader, GL_VERTEX_SHADER, "cubeVert");
		myCubeShaders[1] = compileShader(myCube_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		myCubeProgram = glCreateProgram();
		glAttachShader(myCubeProgram, myCubeShaders[0]);
		glAttachShader(myCubeProgram, myCubeShaders[1]);
		glBindAttribLocation(myCubeProgram, 0, "in_Position");
		glBindAttribLocation(myCubeProgram, 1, "in_Normal");
		linkProgram(myCubeProgram);
	}
	void myCleanupCube() {
		glDeleteBuffers(3, myCubeVbo);
		glDeleteVertexArrays(1, &myCubeVao);

		glDeleteProgram(myCubeProgram);
		glDeleteShader(myCubeShaders[0]);
		glDeleteShader(myCubeShaders[1]);
	}
	void myUpdateCube(const glm::mat4& transform) {
		myObjMat = transform;
	}
	void myDrawCube() {
		glEnable(GL_PRIMITIVE_RESTART);
		glBindVertexArray(myCubeVao);
		glUseProgram(myCubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(myObjMat));
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform4f(glGetUniformLocation(myCubeProgram, "color"), 0.1f, 1.f, 1.f, 0.f);
		glDrawElements(GL_TRIANGLE_STRIP, myNumVerts, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_PRIMITIVE_RESTART);
	}
	void myDrawCubesWheel(double currentTime) {

		glEnable(GL_PRIMITIVE_RESTART);
		glBindVertexArray(myCubeVao);
		glUseProgram(myCubeProgram);

		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform4f(glGetUniformLocation(myCubeProgram, "color"), models3D::cabin.color.x, models3D::cabin.color.y, models3D::cabin.color.z, 0.f);
		glUniform3f(glGetUniformLocation(myCubeProgram, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(myCubeProgram, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(myCubeProgram, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(myCubeProgram, "toonShaderOption"), toonShader);

		float posX, posY;
		glm::mat4 translation_cube;
		glm::mat4 scale_cube = glm::scale(glm::mat4(), glm::vec3(5.f, 5.f, 5.f));
		//CUBS
		for (int i = 0; i < totalCabins; i++) {

			posX = radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*i) / totalCabins));
			posY = radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*i) / totalCabins));

			translation_cube = glm::translate(glm::mat4(), glm::vec3(0.f, posY, posX));
			myObjMat = translation_cube * scale_cube;

			glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(myObjMat));
			glDrawElements(GL_TRIANGLE_STRIP, myNumVerts, GL_UNSIGNED_BYTE, 0);

		}

		/*int cube = 1;
		posX = radiusWheel * cos(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*cube) / totalCabins));
		posY = radiusWheel * sin(2 * 3.14f * frecWheel * currentTime + ((2 * 3.14f*cube) / totalCabins));
		glm::mat4 translation_cube = glm::translate(glm::mat4(), glm::vec3(0.f, posY, posX));*/

		//TRUMP CUBE
		glm::mat4 translation_trump = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.7f));
		myObjMat = translation_trump * translation_cube;

		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(myObjMat));
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform4f(glGetUniformLocation(myCubeProgram, "color"), models3D::trump.color.x, models3D::trump.color.y, models3D::trump.color.z, 0.f);
		glUniform3f(glGetUniformLocation(myCubeProgram, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(myCubeProgram, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(myCubeProgram, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(myCubeProgram, "toonShaderOption"), toonShader);
		glDrawElements(GL_TRIANGLE_STRIP, myNumVerts, GL_UNSIGNED_BYTE, 0);

		//CHICKEN CUBE
		glm::mat4 translation_chicken = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, -0.7f));
		myObjMat = translation_chicken * translation_cube;

		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(myObjMat));
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(myCubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform4f(glGetUniformLocation(myCubeProgram, "color"), models3D::chicken.color.x, models3D::chicken.color.y, models3D::chicken.color.z, 0.f);
		glUniform3f(glGetUniformLocation(myCubeProgram, "sunPos"), Sphere::sun.pos.x, Sphere::sun.pos.y, Sphere::sun.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "sunColor"), Sphere::sun.color.x, Sphere::sun.color.y, Sphere::sun.color.z);
		glUniform1f(glGetUniformLocation(myCubeProgram, "sunIntensity"), Sphere::sun.intensity);
		glUniform3f(glGetUniformLocation(myCubeProgram, "moonPos"), Sphere::moon.pos.x, Sphere::moon.pos.y, Sphere::moon.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "moonColor"), Sphere::moon.color.x, Sphere::moon.color.y, Sphere::moon.color.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "bulbPos"), Sphere::bulb.pos.x, Sphere::bulb.pos.y, Sphere::bulb.pos.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "bulbColor"), Sphere::bulb.color.x, Sphere::bulb.color.y, Sphere::bulb.color.z);
		glUniform3f(glGetUniformLocation(myCubeProgram, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
		glUniform1i(glGetUniformLocation(myCubeProgram, "toonShaderOption"), toonShader);
		glDrawElements(GL_TRIANGLE_STRIP, myNumVerts, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_PRIMITIVE_RESTART);
	}

}