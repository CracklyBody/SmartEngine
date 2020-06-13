#include "Lobby.h"
std::vector<bulletObject*> bodies;
extern volatile int id2;
extern volatile int kils2;
Lobby::Lobby()
{
    bullet_shader = new Shader("bullet.vert", "bullet.frag");
    debug_drawer = new BulletDebugDrawer_OpenGL();
}

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
// Update dead time dead players and respawn if 0
void Lobby::update_respawn(float elapsed_time)
{
    for (auto p : players)
    {
        if (p->freeze)
        {
            p->dead_time -= elapsed_time;
            if (p->dead_time <= 0.f)
            {
                p->respawn();
            }
        }
    }
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
    
    if (glfwGetMouseButton(players[i]->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(!players[0]->SoundEngine->isCurrentlyPlaying("sounds/rifle_burstcut.mp3"))
            players[0]->SoundEngine->play2D("sounds/rifle_burstcut.mp3");
        // ---------------RAYCASTING
        btCollisionWorld::ClosestRayResultCallback raycallback(btVector3(players[i]->getCameraPos().x, players[i]->getCameraPos().y, players[i]->getCameraPos().z), btVector3(players[i]->getCameraLook().x * 10000, players[i]->getCameraLook().y * 10000, players[i]->getCameraLook().z * 10000));
        players[i]->dynamicsWorld->rayTest(btVector3(players[i]->getCameraPos().x, players[i]->getCameraPos().y, players[i]->getCameraPos().z), btVector3(players[i]->getCameraLook().x * 10000, players[i]->getCameraLook().y * 10000, players[i]->getCameraLook().z * 10000), raycallback);
        players[i]->is_shoot = true;

        if (raycallback.hasHit())
        {
            // Find if we hit the player
            bulletObject* ob1 = (bulletObject*)(raycallback.m_collisionObject->getUserPointer());
            if (ob1 != NULL)
            {
                std::cout<<"HIT!\n";
                int id = find_playerid_by_obj_id(ob1->id);
                if (players[id]->get_health() > 0) {
                    if (id != -1)
                        make_damage(id, 1);
                    if (players[id]->freeze) {
                        players[i]->inc_kills();
                        players[i]->client->kils1 = players[i]->get_kills();
                    }
                }
                glm::vec3 look = players[i]->getCameraLook();
                ob1->body->applyCentralForce(btVector3(look.x*100, look.y* 100, look.z* 100));
            }
        }
        //----------------
        /*players[i]->send_to_serv(const_cast<char*>((std::to_string(players[i]->get_camera_pos().x)+" "+
                                                    std::to_string(players[i]->get_camera_pos().y)+" "+
                                                    std::to_string(players[i]->get_camera_pos().z)).c_str()));*/
    }
    if (glfwGetKey(players[i]->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(players[i]->window, true);
    if (players[i]->get_kills() >= 4 && players[i]->winner != true)         
    {
        players[i]->winner = true;
        if (!players[i]->SoundEngine->isCurrentlyPlaying("sounds/youwin.mp3"))
            players[i]->SoundEngine->play2D("sounds/youwin.mp3");
    }
    if(players[i]->winner == true)
    if (!players[i]->SoundEngine->isCurrentlyPlaying("sounds/youwin.mp3"))
        exit(0);
    // SEND SERVER PLAYER INFO
    /*players[i]->send_to_serv((std::to_string(players[i]->id).c_str()));
    players[i]->send_to_serv((std::to_string((int)players[i]->get_camera_pos().x).c_str()));
    players[i]->send_to_serv((std::to_string((int)players[i]->get_camera_pos().y).c_str()));
    players[i]->send_to_serv((std::to_string((int)players[i]->get_camera_pos().z).c_str()));*/
    /*std::string mess1 = std::to_string((int)players[i]->get_camera_pos().x);
    std::string mess2 = std::to_string((int)players[i]->get_camera_pos().y);
    std::string mess3 = std::to_string((int)players[i]->get_camera_pos().z);*/
    /*players[i]->client->plrs[players[i]->id].x = players[i]->get_camera_pos().x;
    players[i]->client->plrs[players[i]->id].y = players[i]->get_camera_pos().y;
    players[i]->client->plrs[players[i]->id].z = players[i]->get_camera_pos().z;*/
    players[i]->id = players[i]->client->id1;
    players[i]->set_kills(players[i]->client->kils1);
    std::string messg = std::to_string(players[i]->client->id1) + std::to_string(players[i]->client->kils1);
    players[i]->send_to_serv(const_cast<char*>(messg.c_str()));
    /*players[i]->send_to_serv(const_cast<char*>(mess1.c_str()));
    players[i]->send_to_serv(const_cast<char*>(mess2.c_str()));
    players[i]->send_to_serv(const_cast<char*>(mess3.c_str()));*/
    // Every tick WE UPDATE ALL PLAYERS INFO
    bool find = false;
    for (int j = 0; j < players.size(); j++)
    {
        if (players[j]->id == id2)
        {
            find = true;
            players[j]->set_kills(kils2);
        }
    }
    if (!find)
    {
        Player* nep = new Player("NewPlayer#" + std::to_string(id2), glm::vec3(0, 150, 0));
        nep->id = id2;
        nep->dynamicsWorld = players[i]->dynamicsWorld;
        nep->create_rigid_body();
        nep->model->type = 1;
        irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
        SoundEngine->setSoundVolume(irrklang::ik_f32(0.2f));

        nep->SoundEngine = SoundEngine;

        bodies.push_back(nep->model);

        //nep->model = cubee;
        players.push_back(nep);
        nep->id_in_lobby_arr = find_playerid_by_obj_id(nep->model->id);

        nep->model->body->setUserPointer(bodies[bodies.size() - 1]);
    }
    //for (int j = 1; j < players[i]->client->plrs.size(); j++)
    //{
    //    bool findt = false;
    //    for (int k = 1; k < players.size(); k++)
    //    {
    //        if (players[i]->client->plrs[j].id == players[k]->id && players[i]->id != players[k]->id)
    //        {
    //            findt = true;
    //            players[k]->reposition_body(btVector3(players[i]->client->plrs[j].x, players[i]->client->plrs[j].y, players[i]->client->plrs[j].z));
    //            //players[k]->set_health(players[i]->client->plrs[j].health);
    //            players[k]->set_kills(players[i]->client->plrs[j].kills);
    //            players[k]->set_deaths(players[i]->client->plrs[j].deaths);
    //            continue;
    //        }
    //    }
    //    if (!findt && players[i]->id != players[i]->client->plrs[j].id)
    //    {
    //        /*btRigidBody* cube2 = addBox(17.196674f, 60.196674f, 17.196674f, players[i]->client->plrs[j].x, players[i]->client->plrs[j].y, players[i]->client->plrs[j].z, 10.f, players[i]->dynamicsWorld);
    //        cube2->forceActivationState(DISABLE_DEACTIVATION);
    //        cube2->setCollisionFlags(cube2->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    //        bulletObject* cubee = new bulletObject(cube2, players[i]->client->plrs[j].id + 3, 1.0, 0.0, 0.0);*/
    //        Player* nep = new Player("NewPlayer#" + std::to_string(players[i]->client->plrs[j].id), glm::vec3(players[i]->client->plrs[j].x, players[i]->client->plrs[j].y, players[i]->client->plrs[j].z));
    //        nep->id = players[i]->client->plrs[j].id;
    //        nep->dynamicsWorld = players[i]->dynamicsWorld;
    //        nep->create_rigid_body();
    //        nep->model->type = 1;
    //        irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
    //        nep->SoundEngine = SoundEngine;

    //        bodies.push_back(nep->model);

    //        //nep->model = cubee;
    //        players.push_back(nep);
    //        nep->id_in_lobby_arr = find_playerid_by_obj_id(nep->model->id);

    //        nep->model->body->setUserPointer(bodies[bodies.size() - 1]);

    //    }
    //}
}

void Lobby::update_info()
{
    //for(int i =0;i<players.size();i++)
}

// Set to update mouse for only 1 player
void Lobby::set_update_mouse(int i)
{
    //players[i]->setupdatemouse();
}



const std::vector<Player*> Lobby::get_players_info() { return players; }
