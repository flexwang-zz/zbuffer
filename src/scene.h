#pragma once
#ifndef ZBUFFER_SCENE_H
#define ZBUFFER_SCENE_H

#include "zbuffer.h"
#include "camera.h"
#include "triangleMesh.h"

class Scene {
public:
	Scene(char *filepath);
	Scene();


	TriangleMesh *tri;
    Camera *camera;
    Point *light;
};

#endif
