#pragma once
#ifndef COLLISION_H
#define COLLISION_H

class Collision
{
	public:
	bool raysphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r, float* dist = NULL, coordinate* point = NULL);
		bool rayplane(float nx, float ny, float nz, float xs, float ys, float zs, float xd, float yd, float zd, coordinate p1, coordinat p2, coordinate p3, coordinate p4, float* dist = NULL, coordinate* point = NULL);
};

#endif