#include "scene.h"

Scene::Scene() {
}

Scene::Scene(char *filepath) {
	ifstream fin(filepath);  

	if (!fin) {
		fprintf(stderr, "Error: File not exist!");
		exit(-1);
	}

	map<string, vector<string>> infos;

	string line;

	while (getline(fin, line)) {
		istringstream is(line);

		string type;

		is>>type;

		infos[type].push_back(line.substr(type.size()));
	}

	camera  =  new Camera(infos);
	tri = new TriangleMesh(infos);

	string lightinfo = getvalue(infos, "light", "1. 1. 1.");

	istringstream is(lightinfo);

	float x, y, z;
	is >>x >>y >>z;
	light = new Point(x, y, z);

}

