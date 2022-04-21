#pragma once

#include "GLCommon.h"

#include <glm/gtc/type_ptr.hpp> 

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <sstream>

#include "ShaderManager.h"
#include "Camera.h"
#include "Light.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "LightManager.h"

#include "EntityFactory.h"
#include "World.h"
#include "Entity.h"

#include "Utilities.h"

const float windowWidth = 1200;
const float windowHeight = 640;
bool editMode = true;

ShaderManager gShaderManager;

Camera camera(windowHeight, windowWidth);
float moveSpeed = 1.1f;
float lightMoveSpeed = 0.01f;

int currentEditIndex = 0;

static bool copyFile(std::string src, std::string dest)
{
	std::ifstream ifSrc(src, std::ios::binary);
	std::ofstream ofDest(dest, std::ios::binary);
	ofDest << ifSrc.rdbuf();
	return ifSrc && ofDest;
}

static bool doesFileExist(std::string path)
{
	std::ifstream ifs(path);
	return ifs.good();
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Movement controls
	if (!editMode)
	{
		if (key == GLFW_KEY_W)
		{
			camera.position += camera.direction * moveSpeed;
		}
		if (key == GLFW_KEY_A)
		{
			// Rotate our camera's direction 90 degrees to the left
			glm::vec3 left;
			constexpr float theta = glm::radians(90.0f);
			left.x = camera.direction.x * cos(theta) + camera.direction.z * sin(theta);
			left.y = 0.0f;
			left.z = -camera.direction.x * sin(theta) + camera.direction.z * cos(theta);
			camera.position += left * moveSpeed;
		}
		if (key == GLFW_KEY_S)
		{
			camera.position -= camera.direction * moveSpeed;
		}
		if (key == GLFW_KEY_D)
		{
			// Rotate our camera's direction 90 degrees to the right
			glm::vec3 right;
			constexpr float theta = glm::radians(-90.0f);
			right.x = camera.direction.x * cos(theta) + camera.direction.z * sin(theta);
			right.y = 0.0f;
			right.z = -camera.direction.x * sin(theta) + camera.direction.z * cos(theta);
			camera.position += right * moveSpeed;
		}
		if (key == GLFW_KEY_SPACE)
		{
			camera.position.y += moveSpeed;
		}
	}

	// Toggle cursor view
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		editMode = !editMode;
		int cursorOption = editMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, cursorOption);
	}

	if (key == GLFW_KEY_LEFT)
	{
		glm::vec4 position = LightManager::GetInstance()->GetLight(0)->GetDirection();
		LightManager::GetInstance()->GetLight(0)->EditDirection(position.x - lightMoveSpeed, position.y, position.z, position.w);
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		glm::vec4 position = LightManager::GetInstance()->GetLight(0)->GetDirection();
		LightManager::GetInstance()->GetLight(0)->EditDirection(position.x + lightMoveSpeed, position.y, position.z, position.w);
	}
	else if (key == GLFW_KEY_UP)
	{
		glm::vec4 position = LightManager::GetInstance()->GetLight(0)->GetDirection();
		LightManager::GetInstance()->GetLight(0)->EditDirection(position.x, position.y, position.z + lightMoveSpeed, position.w);
	}
	else if (key == GLFW_KEY_DOWN)
	{
		glm::vec4 position = LightManager::GetInstance()->GetLight(0)->GetDirection();
		LightManager::GetInstance()->GetLight(0)->EditDirection(position.x, position.y, position.z - lightMoveSpeed, position.w);
	}
	else if (key == GLFW_KEY_PAGE_UP)
	{
		glm::vec4 position = LightManager::GetInstance()->GetLight(0)->GetDirection();
		LightManager::GetInstance()->GetLight(0)->EditDirection(position.x, position.y + lightMoveSpeed, position.z, position.w);
	}
	else if (key == GLFW_KEY_PAGE_DOWN)
	{
		glm::vec4 position = LightManager::GetInstance()->GetLight(0)->GetDirection();
		LightManager::GetInstance()->GetLight(0)->EditDirection(position.x, position.y - lightMoveSpeed, position.z, position.w);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!editMode)
	{
		camera.MoveCamera(xpos, ypos);
	}
}

bool InitializerShaders();

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Initialize our window
	window = glfwCreateWindow(windowWidth, windowHeight, "Midterm", NULL, NULL);

	// Make sure the window initialized properly
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback); // Tell GLFW where our key callbacks are
	glfwSetCursorPosCallback(window, mouse_callback); // Tell GLFW where our mouse callback is

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // Give glad this process ID
	glfwSwapInterval(1);

	if (!InitializerShaders())
	{
		return -1;
	}

	// Load textures
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\white_marble.tga";
		TextureManager::GetInstance()->LoadTexture(ss.str(), TextureManager::TextureType::Diffuse, "marble");
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\zombie_texture.jpg";
		TextureManager::GetInstance()->LoadTexture(ss.str(), TextureManager::TextureType::Diffuse, "zombie");
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\skeleton_texture.jpg";
		TextureManager::GetInstance()->LoadTexture(ss.str(), TextureManager::TextureType::Diffuse, "skeleton");
	}



	// Load models
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\ISO_Sphere.ply";
		Model* model = ModelManager::GetInstance()->LoadModel(ss.str(), "lightFrame");
		model->SetWireframe(true);
		model->SetIgnoreLighting(true);
		model->SetIsOverrideColor(true);
		model->SetColorOverride(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\box.obj";
		ModelManager::GetInstance()->LoadModel(ss.str(), "box");
		ModelManager::GetInstance()->GetModel("box")->AddTexture(TextureManager::GetInstance()->GetTextureFromFriendlyName("marble"));
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\SoccerBall.obj";
		Model* model = ModelManager::GetInstance()->LoadModel(ss.str(), "skeleton");
		model->AddTexture(TextureManager::GetInstance()->GetTextureFromFriendlyName("skeleton"));
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\SoccerBall.obj";
		Model* model = ModelManager::GetInstance()->LoadModel(ss.str(), "zombie");
		model->AddTexture(TextureManager::GetInstance()->GetTextureFromFriendlyName("zombie"));
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\SoccerBall.obj";
		Model* model = ModelManager::GetInstance()->LoadModel(ss.str(), "survivor");
		model->AddTexture(TextureManager::GetInstance()->GetTextureFromFriendlyName("marble"));
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\SoccerBall.obj";
		Model* model = ModelManager::GetInstance()->LoadModel(ss.str(), "bullet");
		model->AddTexture(TextureManager::GetInstance()->GetTextureFromFriendlyName("marble"));
	}

	CompiledShader shader = *gShaderManager.pGetShaderProgramFromFriendlyName("Shader#1");

	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;
	float previousTime = static_cast<float>(glfwGetTime());

	// Give us some basic lighting
	LightManager::GetInstance()->AddLight(shader, "sun", glm::vec3(2.0f, 10.0f, 0.0f));
	Light* sun = LightManager::GetInstance()->GetLight("sun");
	sun->EditLightType(Light::LightType::DIRECTIONAL, 10.0f, 30.0f);
	sun->EditDirection(0.5f, -1.0f, 0.0f, 1.0f);

	World world;

	IEntity* survivor = world.MakeEntity("survivor");
	survivor->SetPosition(glm::vec3(-50, 0, 50));

	float lastSpawnTime = 0.0f;

	// Our actual render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// FPS TITLE
		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "FPS: " + fps + "   MS: " + ms;
				glfwSetWindowTitle(window, newTitle.c_str());

	
				fpsTimeElapsed = 0.f;
				fpsFrameCount = 0.f;
			}
		}

		float ratio;
		int width, height;
		glm::mat4 projection;
		glm::mat4 view;

		glfwGetFramebufferSize(window, &width, &height); // Assign width and height to our window width and height
		ratio = width / (float)height;

		glEnable(GL_DEPTH);
		glEnable(GL_DEPTH_TEST); // Enables the Depth Buffer, which decides which pixels will be drawn based on their depth (AKA don't draw pixels that are behind other pixels)

		glViewport(0, 0, width, height); // Specifies the transformation of device coords to window coords 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the buffers

		view = camera.GetViewMatrix();
		projection = glm::perspective(0.6f, ratio, 0.1f, 1000.0f);

		shader.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matView"), 1, GL_FALSE, glm::value_ptr(view)); // Assign new view matrix
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matProjection"), 1, GL_FALSE, glm::value_ptr(projection)); // Assign projection
		glUniform4f(glGetUniformLocation(shader.ID, "cameraPosition"), camera.position.x, camera.position.y, camera.position.z, 1.0f);

		// Safety, mostly for first frame
		if (deltaTime == 0.0f)
		{
			deltaTime = 0.03f;
		}

		if (lastSpawnTime == 0.0f || (currentTime - lastSpawnTime) >= getRandom(2.0f, 6.0f))
		{
			lastSpawnTime = currentTime;

			IEntity* skeleton = world.MakeEntity("skeleton");
			skeleton->SetPosition(glm::vec3(getRandom(-20.0f, 20.0f), 0, getRandom(-20.0f, 20.0f)));

			IEntity* zombie = world.MakeEntity("zombie");
			zombie->SetPosition(glm::vec3(getRandom(-20.0f, 20.0f), 0, getRandom(-20.0f, 20.0f)));
		}

		world.Tick(deltaTime);

		// Render the models
		ModelManager::GetInstance()->Draw("box", shader, glm::vec3(-100.0f, -3.0f, -100.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(100.0f, 0.2f, 100.0f));

		// Draw lights
		for (Light* light : LightManager::GetInstance()->GetLights())
		{
			ModelManager::GetInstance()->Draw("lightFrame", shader, light->GetPosition(), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}

		world.DrawEntities(shader);

		// Show what we've drawn
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	if (EntityFactory::GetInstance() != NULL)
	{
		delete EntityFactory::GetInstance();
	}
		
	TextureManager::GetInstance()->CleanUp();
	delete TextureManager::GetInstance();

	ModelManager::GetInstance()->CleanUp();
	delete ModelManager::GetInstance();

	glfwDestroyWindow(window); // Clean up the window

	glfwTerminate(); 
	exit(EXIT_SUCCESS);
}


bool InitializerShaders()
{
	std::stringstream ss;

	// "Normal" Shader
	Shader vertexShader;
	ss << SOLUTION_DIR << "Extern\\assets\\shaders\\vertexShader.glsl";
	vertexShader.fileName = ss.str();
	ss.str("");

	Shader fragmentShader;
	ss << SOLUTION_DIR << "Extern\\assets\\shaders\\fragmentShader.glsl";
	fragmentShader.fileName = ss.str();
	ss.str("");

	bool success = gShaderManager.createProgramFromFile("Shader#1", vertexShader, fragmentShader);
	if (success)
	{
		std::cout << "Shaders compiled OK" << std::endl;
	}
	else
	{
		std::cout << "Error making shaders: " << std::endl;
		std::cout << gShaderManager.getLastError() << std::endl;
		return 1;
	}

	return success;
}