#include "Player.h"

const btScalar RADIANS_PER_DEGREE = M_PI / btScalar(180.0);

Player::Player(std::string nickname, glm::vec3 pos, GLFWwindow* window) : nickname(nickname), cameraPos(pos), window(window)
{
    load_font();
    bullet_shader = new Shader("bullet.vert", "bullet.frag");
    debug_drawer = new BulletDebugDrawer_OpenGL();
}


Player::~Player()
{

}

std::string Player::get_nickname() { return nickname; }

int Player::get_kills() { return kills; }

int Player::get_death() { return death; }

void Player::set_window_pointer()
{
    glfwSetWindowUserPointer(window, this);
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
    
}

void Player::draw_line()
{
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 10000.0f);
    debug_drawer->line_shader = bullet_shader;
    bullet_shader->Use();
    glm::mat4 view_ray = glm::mat4(1.f);
    glm::mat4 model_ray = glm::mat4(1.f);
    model_ray = glm::translate(model_ray, glm::vec3(0.f, -0.1f, 0.f));
    view_ray = lookAt();
    debug_drawer->SetMatrices(view_ray, projection, model_ray);
    glm::vec3 direction = get_camera_front();
    direction.x *= 10000;
    direction.y *= 10000;
    direction.z *= 10000;
    glm::vec3 r_cam = getCameraPos();
    //r_cam.z += 0.5f;
    /*if (direction.z >= 0.f)
        r_cam.z += 0.5f;
    else
        r_cam.z -= 0.5f;*/
    debug_drawer->drawLine(btVector3(r_cam.x, r_cam.y, r_cam.z), btVector3(direction.x, direction.y, direction.z), btVector3(1.0f, 1.f, 1.f));
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
    btQuaternion q = btQuaternion(-changedyaw * RADIANS_PER_DEGREE,0.0f, -pitch * RADIANS_PER_DEGREE);
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

void Player::make_hit(int damage)
{
    if (health > 0) {
        health -= damage;
        if (!SoundEngine->isCurrentlyPlaying("sounds/hitmarker.mp3"))
            SoundEngine->play2D("sounds/hitmarker.mp3");
        if (health <= 0)
        {
            freeze = true;
            btQuaternion q = btQuaternion(0.f, 0.0f, -87.9f * RADIANS_PER_DEGREE);
            btTransform trans = model->body->getWorldTransform();
            trans.setRotation(q);
            model->body->setWorldTransform(trans);
            inc_death();
            SoundEngine->play2D("sounds/lego_yoda_death.mp3");

        }
    }
}

glm::vec3 Player::get_camera_front(){ return cameraFront; }
glm::vec3 Player::get_camera_pos(){ return cameraPos; }
glm::vec3 Player::get_camera_up() { return cameraUp; }
glm::vec3 Player::get_camera_target() { return cameraTarget; }
float Player::get_cam_spd() { return cameraSpeed; }
float Player::get_boost_bar() { return boostBar; }
void Player::set_boost_bar(float n) { boostBar = n; }
void Player::set_cam_spd(float n) { cameraSpeed = n; }

void Player::load_font()
{
    font = new Font();
}

void Player::render_player_info()
{
    
    font->render_text(std::string("Health: "+std::to_string(health)), 25.0f, 25.0f, 0.4f, glm::vec3(1.0, 1.0f, 1.0f));

}

void Player::render_text(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    font->render_text(text, x, y, scale, color);
}

void Player::draw_lobby_info(const std::vector<Player*> players)
{
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        for (int j = 0; j < players.size(); j++)
        {
            render_text(players[j]->get_nickname() + "   " + std::to_string(players[j]->get_kills()) + "   " + std::to_string(players[j]->get_death()), 25.0f, 150.0f-j*20, 0.4f, glm::vec3(1.f, 1.f, 1.f));
        }
    }
}

void Player::respawn()
{
    dead_time = 10.f;
    health = 100.f;
    freeze = false;
    btQuaternion q = btQuaternion(0.f, 0.0f, 360.0f * RADIANS_PER_DEGREE);
    btTransform trans = model->body->getWorldTransform();
    trans.setRotation(q);
    model->body->setWorldTransform(trans);
}

void Player::inc_kills() { kills++; }
void Player::inc_death() { death++; }
int Player::get_health() { return health; }