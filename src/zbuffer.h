#pragma once

#ifndef ZBUFFER_ZBUFFER_H
#define ZBUFFER_ZBUFFER_H

// Global Constants
#ifdef M_PI
#undef M_PI
#endif
#define M_PI       3.14159265358979323846f

// Global Forward Include
#include <atlimage.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;
#include <math.h>
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;
#include <algorithm>
using std::sort;

//Qt Include
#include <QString>
#include <QPixmap>

// Global Forward Declarations
class Point;
class Vector;
class Triangle;
class TriangleMesh;
class Camera;
class Scene;

inline float Radians(float deg) {
    return ((float)M_PI/180.f) * deg;
}

inline string getvalue(map<string, vector<string>> &infos, string tag, string _default) {
	if (infos[tag].size()) {
		return infos[tag][0];
	}
	return _default;
}

//Zbuffer struct
typedef struct _edge Edge;
// Zbuffer api
QImage zbuffer(QString str, float rx = 0.f, float ry = 0.f);
QImage zbuffer(Scene* scene, float rx = 0.f, float ry = 0.f, float scale = 1.f);
void InitScanLineNewEdgeTable(vector< vector<Edge> >& slNet, Point* p, int ymin, int ymax, Triangle tri);
void ScanlineFill( Scene *scene, int index, vector<float>& buffer, QImage &image);
void ProcessScanLineFill(vector< vector<Edge> >& slNet, int ymin, int ymax, QImage &image, vector<float>& buffer, Triangle tri, Point *light);
#endif
