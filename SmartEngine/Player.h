#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Player
{
public:
	Player(glm::vec3 pos, GLFWwindow* window);
	~Player();
	void setupdayekey();
	void setupdatemouse();
	void updatekey();
	void updatemouse(double xpos, double ypos);
	glm::mat4 lookAt();
	GLFWwindow* window;
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool freeze = false;
	float cameraSpeed = 0.0005f;
	float boostBar = 100.0f;
	
	GLfloat lastX = 640;
	GLfloat lastY = 480;
	GLfloat yaw = -90.0f;
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