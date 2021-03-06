#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include "Model.h"
// Main player class, has func for update camera and more
class Player
{
public:
	Player(glm::vec3 pos, GLFWwindow* window);
	~Player();
	void setupdayekey();
	void setupdatemouse();
	void updateCamPos();
	void updatekey();
	void updatemouse(double xpos, double ypos);
	glm::vec3 getCameraPos();
	glm::vec3 getCameraLook();
	glm::mat4 lookAt();
	GLFWwindow* window;
	bulletObject* model;
	bool cursor = false;
	float elapsedtime = 0.0f;
private:
	float getModelAngle();
	btVector3 getAxis();
	float calculateHorizontalDistance();
	float calculateVerticalDostance();
	float distanceFromPlayer = 50.f;
	float horizontalDistance = 0.f;
	float verticalDistance = 0.f;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, 0.f);
	bool freeze = false;
	float cameraSpeed = 10.0f;
	float boostBar = 100.0f;
	
	GLfloat lastX = 640;
	GLfloat lastY = 480;
	GLfloat yaw = 180.0f;
	GLfloat pitch = 0.0f;

	static void updatekey(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		//Player* player = (Player*)glfwGetWindowUserPointer(window);
		//player->updatekey(key, scancode, action, mode);
	}
	static void updatemouse(GLFWwindow* window, double xpos, double ypos)\
	{
		Player* player = (Player*)glfwGetWindowUserPointer(window);
		player->updatemouse(xpos, ypos);
	}
};
#endif // !PLAYER_H
