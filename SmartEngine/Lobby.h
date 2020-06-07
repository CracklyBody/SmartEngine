#pragma once
#ifndef LOBBY_H
#define LOBBY_H

#include <iostream>
#include <vector>

#include "Player.h"
#include "BulletDebugDrawer.h"

class Lobby {
public:
	Lobby();
	~Lobby() {};
	bool add_player(Player* player);
	int find_playerid_by_obj_id(int id);
	void make_damage(unsigned int i, unsigned int damage);
	void update_key(int id);
	void update_respawn(float elapsed_time);
	void set_update_mouse(int id);
	BulletDebugDrawer_OpenGL* debug_drawer;
	Shader* bullet_shader;
	const std::vector<Player*> get_players_info();
private:
	// Here we store all players in array
	std::vector<Player*> players;
};
#endif // !LOBBY_H
