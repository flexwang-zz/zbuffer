#pragma once

#ifndef ZBUFFER_CAMERA_H
#define ZBUFFER_CAMERA_H

#include "zbuffer.h"
#include "geometry.h"
#include "matrix4x4.h"

class Camera {
public:
	Camera(){}
	Camera(int w, int h, float f, Point e, Point l, Vector u);
	Camera(map<string, vector<string>> &infos);

	void Project(float xin, float yin, float zin, float& xout, float& yout, float& zout);
	void RotateX(float x);
	void RotateY(float y);
	void Scale(float s);

	int width, height;
	Point eye, look;
	Vector up;
	float fov;

	Vector xdir, ydir;
	float dx, dy;
	Point o;	//left-top of the view plane

	Matrix4x4 world2camera, camera2view, world2screen;
};

#endif