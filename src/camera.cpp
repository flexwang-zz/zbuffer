#include "camera.h"

Camera::Camera(int w, int h, float f, Point e, Point l, Vector u)
	:width(w), height(h), fov(f), eye(e), look(l), up(u)
{
	Vector dir = eye - look;

	xdir = Normalize(Cross(dir, up));
	ydir = Normalize(up);

	float xrange = (float) (dir.Length()*tan(fov/2.f));
	float yrange = xrange/width*height;
		
	dx = xrange/width*2.f;
	dy = yrange/height*2.f;
		
	o = look + xdir*(-xrange) + ydir*(-yrange);
}

Camera::Camera(map<string, vector<string>> &infos) {
	string imageinfo = getvalue(infos, "image", "640 480");
	istringstream is(imageinfo);
	is>>width>>height;

	float x, y, z;

	string eyeinfo = getvalue(infos, "eye", "0 0 -1");
	is = istringstream(eyeinfo);

	is >>x>>y>>z;
	eye = Point(x, y, z);

	string lookinfo = getvalue(infos, "look", "0 0 0");
	is = istringstream(lookinfo);

	is >>x>>y>>z;
	look = Point(x, y, z);

	string upinfo = getvalue(infos, "up", "0 1 0");
	is = istringstream(upinfo);
	is >>x>>y>>z;
	up = Vector(x, y, z);

	string fovinfo = getvalue(infos, "fov", "45");
	fov = atof(fovinfo.c_str());

	world2camera = LookAt(eye, look, up);
	camera2view = Perspective(fov, 0.001f, 300.f);
}

void Camera::RotateX(float x) {
	Vector dir = eye-look;

	xdir = Normalize(Cross(dir, up));
	ydir = Normalize(up);

	dir = Rotate(x, ydir)(dir);

	eye = look+dir;

	world2camera = LookAt(eye, look, up);
	camera2view = Perspective(fov, 0.001f, 300.f);
	
}

void Camera::RotateY(float y) {
	Vector dir = eye-look;

	xdir = Normalize(Cross(dir, up));
	ydir = Normalize(up);

	dir = Rotate(y, xdir)(dir);
	up = Rotate(y, xdir)(up);
	eye = look+dir;
	
	world2camera = LookAt(eye, look, up);
	camera2view = Perspective(fov, 0.001f, 300.f);
}

void Camera::Scale(float s) {
	Vector dir = eye-look;
	dir *= s;

	eye = look+dir;
	world2camera = LookAt(eye, look, up);
	camera2view = Perspective(fov, 0.001f, 300.f);
}

void Camera::Project(float xin, float yin, float zin, float& xout, float& yout, float& zout) {
	Vector v(xin, yin, zin);
	v = world2camera(v);
	v = camera2view(v);

	if (width < height) {
		v.y *= (float)width/height;
	}
	else {
		v.x *= (float)height/width;
	}

	xout = (v.x+1.f)/2.f*width-1.f;
	yout = (-v.y+1.f)/2.f*height-1.f;
	zout = v.z;
}