#include "Player.h"

const btScalar RADIANS_PER_DEGREE = M_PI / btScalar(180.0);

Player::Player(glm::vec3 pos,GLFWwindow * window) : cameraPos(pos), window(window)
{
    glfwSetWindowUserPointer(window, this);
}

Player::~Player()
{

}

void Player::setupdayekey()
{
    glfwSetKeyCallback(window, updatekey);
}

void Player::setupdatemouse()
{
    glfwSetCursorPosCallback(window, updatemouse);
}

glm::vec3 Player::getCameraPos()
{
    return cameraPos;
}

glm::vec3 Player::getCameraLook()
{
    return cameraFront;
}

void Player::updatekey()
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        model->body->applyCentralForce(btVector3(cameraFront.x * cameraSpeed,0.f,cameraFront.z* cameraSpeed));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 left = glm::normalize(glm::cross(cameraFront, cameraUp));
        model->body->applyCentralForce(btVector3(left.x * -cameraSpeed, 0.f, left.z * -cameraSpeed));

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        model->body->applyCentralForce(btVector3(cameraFront.x * -cameraSpeed, 0.f, cameraFront.z * -cameraSpeed));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
        model->body->applyCentralForce(btVector3(right.x * cameraSpeed, 0.f, right.z * cameraSpeed));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        if (boostBar != 0.0f)
            cameraSpeed += 1.0f;
    }   
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        model->body->applyCentralForce(btVector3(0.f, 10.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Player::updateCamPos()
{
    btTransform trans = model->body->getWorldTransform();
    btVector3 pos = trans.getOrigin();
    pos = btVector3(pos.x(), pos.y(), pos.z());
    //horizontalDistance = calculateHorizontalDistance();
    //verticalDistance = calculateVerticalDostance();
    cameraPos.y = pos.y() + 5.f;
    float theta = yaw;
    //float offsetX = horizontalDistance * sin(glm::radians(theta));
    //float offsetZ = horizontalDistance * cos(glm::radians(theta));
    cameraPos.x = pos.x() + 0.f;
    cameraPos.z = pos.z() + 0.f;

    //horizontalDistance = 10.f * cos(glm::radians(pitch));
    //verticalDistance = 10.f * sin(glm::radians(pitch));
    //cameraFront.y = pos.y();
    theta = yaw;
    //offsetX = horizontalDistance * sin(glm::radians(theta));
    //offsetZ = horizontalDistance * cos(glm::radians(theta));
    /*cameraFront.x = pos.x() + offsetX;
    cameraFront.z = pos.z() + offsetZ;
    cameraFront = glm::vec3(cameraFront.x - pos.x(), cameraFront.y - pos.y(), cameraFront.z - pos.z());
    cameraFront = glm::normalize(cameraFront);*/
    //cameraPos = glm::vec3(pos.x(), pos.y() + 30.f, pos.z() - 50.f);
    //cameraFront = glm::vec3(pos.x(), pos.y()+15.f, pos.z());
    cameraTarget = glm::vec3(pos.x(), pos.y(), pos.z());
}


glm::mat4 Player::lookAt()
{
    return glm::lookAt(cameraPos,  cameraPos+cameraFront, cameraUp);
}
void Player::updatemouse(double xpos, double ypos)
{
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);

    float changedyaw = yaw - 180;
    btQuaternion q = btQuaternion(-changedyaw * RADIANS_PER_DEGREE, 0.f, 0.0f);
    btTransform trans = model->body->getWorldTransform();
    trans.setRotation(q);
    model->body->setWorldTransform(trans);
}

btVector3 Player::getAxis()
{
    btQuaternion rotQuat = model->body->getWorldTransform().getRotation();
    return rotQuat.getAxis();
}

float Player::getModelAngle()
{
    btQuaternion rotQuat = model->body->getWorldTransform().getRotation();
    return rotQuat.getAngle();
}

float Player::calculateHorizontalDistance()
{
    return distanceFromPlayer * cos(glm::radians(-pitch));
}

float Player::calculateVerticalDostance()
{
    return distanceFromPlayer * sin(glm::radians(-pitch));
}
