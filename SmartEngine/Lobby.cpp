#include "Lobby.h"

// Added player to lobby
bool Lobby::add_player(Player* player)
{
	for (auto p : players)
	{
		if (p->model->id == player->model->id)
			return false;
	}
	players.push_back(player);
	return true;
}

// Find player id by bullet object id
// if false then returned -1
// if true returned player id in lobby array
int Lobby::find_playerid_by_obj_id(int id)
{
	for (int i=0;i<players.size();i++)
	{
		if (players[i]->model->id == id)
			return i;
	}
	return -1;
}

void Lobby::make_damage(unsigned int i, unsigned int damage)
{
	players[i]->make_hit(damage);
}

void Lobby::update_key(int i)
{
    if (!players[i]->in_jump) {
        if (glfwGetKey(players[i]->window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if (glfwGetKey(players[i]->window, GLFW_KEY_A) == GLFW_PRESS)
            {
                glm::vec3 left = glm::normalize(glm::cross(players[i]->get_camera_front(), players[i]->get_camera_up()));
                glm::vec3 front = glm::normalize(glm::vec3(players[i]->get_camera_front().x, 0.f, players[i]->get_camera_front().z));
                front = glm::vec3(front.x - left.x, 0.f, front.z - left.z);
                players[i]->model->body->setLinearVelocity(btVector3(front.x * players[i]->get_cam_spd(), 0.f, front.z * players[i]->get_cam_spd()));
            }
            else
                if (glfwGetKey(players[i]->window, GLFW_KEY_D) == GLFW_PRESS)
                {
                    glm::vec3 right = glm::normalize(glm::cross(players[i]->get_camera_front(), players[i]->get_camera_up()));
                    glm::vec3 front = glm::normalize(glm::vec3(players[i]->get_camera_front().x, 0.f, players[i]->get_camera_front().z));
                    front = glm::vec3(front.x + right.x, 0.f, front.z + right.z);
                    players[i]->model->body->setLinearVelocity(btVector3(front.x * players[i]->get_cam_spd(), 0.f, front.z * players[i]->get_cam_spd()));
                }
                else
                    players[i]->model->body->setLinearVelocity(btVector3(players[i]->get_camera_front().x * players[i]->get_cam_spd(), 0.f, players[i]->get_camera_front().z * players[i]->get_cam_spd()));

            //players[i]->model->body->applyCentralForce(btVector3(players[i]->get_camera_front().x * players[i]->get_cam_spd(), 0.f, players[i]->get_camera_front().z * players[i]->get_cam_spd()));
        }
        if (glfwGetKey(players[i]->window, GLFW_KEY_A) == GLFW_PRESS)
        {
            if (glfwGetKey(players[i]->window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(players[i]->window, GLFW_KEY_S) == GLFW_RELEASE)
            {
                glm::vec3 left = glm::normalize(glm::cross(players[i]->get_camera_front(), players[i]->get_camera_up()));
                players[i]->model->body->setLinearVelocity(btVector3(left.x * -players[i]->get_cam_spd(), 0.f, left.z * -players[i]->get_cam_spd()));
            }

        }
        if (glfwGetKey(players[i]->window, GLFW_KEY_S) == GLFW_PRESS)
        {
            if (glfwGetKey(players[i]->window, GLFW_KEY_A) == GLFW_PRESS)
            {
                glm::vec3 left = glm::normalize(glm::cross(players[i]->get_camera_front(), players[i]->get_camera_up()));
                glm::vec3 front = glm::normalize(glm::vec3(players[i]->get_camera_front().x, 0.f, players[i]->get_camera_front().z));
                front = glm::vec3(front.x + left.x, 0.f, front.z + left.z);
                players[i]->model->body->setLinearVelocity(btVector3(front.x * -players[i]->get_cam_spd(), 0.f, front.z * -players[i]->get_cam_spd()));
            }
            else
                if (glfwGetKey(players[i]->window, GLFW_KEY_D) == GLFW_PRESS)
                {
                    glm::vec3 right = glm::normalize(glm::cross(players[i]->get_camera_front(), players[i]->get_camera_up()));
                    glm::vec3 front = glm::normalize(glm::vec3(players[i]->get_camera_front().x, 0.f, players[i]->get_camera_front().z));
                    front = glm::vec3(front.x - right.x, 0.f, front.z - right.z);
                    players[i]->model->body->setLinearVelocity(btVector3(front.x * -players[i]->get_cam_spd(), 0.f, front.z * -players[i]->get_cam_spd()));
                }
                else
                    players[i]->model->body->setLinearVelocity(btVector3(players[i]->get_camera_front().x * -players[i]->get_cam_spd(), 0.f, players[i]->get_camera_front().z * -players[i]->get_cam_spd()));
        }
        if (glfwGetKey(players[i]->window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if (glfwGetKey(players[i]->window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(players[i]->window, GLFW_KEY_S) == GLFW_RELEASE)
            {
                glm::vec3 right = glm::normalize(glm::cross(players[i]->get_camera_front(), players[i]->get_camera_up()));
                players[i]->model->body->setLinearVelocity(btVector3(right.x * players[i]->get_cam_spd(), 0.f, right.z * players[i]->get_cam_spd()));
            }
        }
        if (glfwGetKey(players[i]->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            if (players[i]->get_boost_bar() != 0.0f)
                players[i]->set_cam_spd(players[i]->get_cam_spd() + 1.0f);
        }
        if ((glfwGetKey(players[i]->window, GLFW_KEY_Z) == GLFW_PRESS) && !players[i]->in_jump)
        {
            if (glfwGetKey(players[i]->window, GLFW_KEY_W) == GLFW_PRESS)
            {
                glm::vec3 front = glm::normalize(glm::vec3(players[i]->get_camera_front().x, 0.f, players[i]->get_camera_front().z));
                players[i]->model->body->setLinearVelocity(btVector3(front.x*players[i]->get_cam_spd(), 30.f, front.z * players[i]->get_cam_spd()));
                players[i]->in_jump = true;
            }
            else
                if (glfwGetKey(players[i]->window, GLFW_KEY_S) == GLFW_PRESS)
                {
                    glm::vec3 front = glm::normalize(glm::vec3(players[i]->get_camera_front().x, 0.f, players[i]->get_camera_front().z));
                    players[i]->model->body->setLinearVelocity(btVector3(front.x * -players[i]->get_cam_spd(), 30.f, front.x * -players[i]->get_cam_spd()));
                    players[i]->in_jump = true;
                }
                else
                    if (glfwGetKey(players[i]->window, GLFW_KEY_A) == GLFW_PRESS)
                    {
                        players[i]->model->body->setLinearVelocity(btVector3(30.f, 30.f, 0.f));
                        players[i]->in_jump = true;
                    }
                    else {
                        players[i]->model->body->setLinearVelocity(btVector3(0.f, 30.f, 0.f));
                        players[i]->in_jump = true;
                    }
        }
    }
    if (glfwGetKey(players[i]->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(players[i]->window, true);
    if (glfwGetMouseButton(players[i]->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(!players[0]->SoundEngine->isCurrentlyPlaying("sounds/rifle_burstcut.mp3"))
            players[0]->SoundEngine->play2D("sounds/rifle_burstcut.mp3");
        // ---------------RAYCASTING
        btCollisionWorld::ClosestRayResultCallback raycallback(btVector3(players[i]->getCameraPos().x, players[i]->getCameraPos().y, players[i]->getCameraPos().z), btVector3(players[i]->getCameraLook().x * 10000, players[i]->getCameraLook().y * 10000, players[i]->getCameraLook().z * 10000));
        players[i]->dynamicsWorld->rayTest(btVector3(players[i]->getCameraPos().x, players[i]->getCameraPos().y, players[i]->getCameraPos().z), btVector3(players[i]->getCameraLook().x * 10000, players[i]->getCameraLook().y * 10000, players[i]->getCameraLook().z * 10000), raycallback);
        if (raycallback.hasHit())
        {
            // Find if we hit the player
            bulletObject* ob1 = (bulletObject*)(raycallback.m_collisionObject->getUserPointer());
            if (ob1 != NULL)
            {
                std::cout<<"HIT!\n";
                int id = find_playerid_by_obj_id(ob1->id);
                if (id != -1)
                {
                    make_damage(id, 10);
                }
                else
                {
                    glm::vec3 look = players[i]->getCameraLook();
                    ob1->body->applyCentralForce(btVector3(look.x*100, look.y* 100, look.z* 100));
                }
            }
        }
        //----------------
    }
}
// Set to update mouse for only 1 player
void Lobby::set_update_mouse(int i)
{
    players[i]->setupdatemouse();
}

