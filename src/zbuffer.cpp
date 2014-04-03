#include<stdio.h>
#include "zbuffer.h"
#include "scene.h"
#include <time.h>

inline int clamp(int value, int low, int up) {
	if (value > up)	return up;
	if (value <low) return low;
	return value;
}

//Zbuffer struct
struct _edge{
	float xi;
	float dx;

	int ymax;

	float zi;
	float dz;

	Point pi;
	Vector dp;

	Vector ni;
	Vector dn;

	bool operator < (const struct _edge & e) const {
		return xi < e.xi;
	}
};



QImage zbuffer(QString str, float rx, float ry) {

	Scene *scene = new Scene(str.toLatin1().data());
	if (rx != 0.f) {
		scene->camera->RotateX(rx);
	}
	
	if (ry != 0.f) {
		scene->camera->RotateY(ry);
	}
	int width = scene->camera->width;
	int height = scene->camera->width;

	vector<float> buffer(width*height, 1.1f);

	QImage image(width, height, QImage::Format_RGB888);
	image.fill(Qt::black);
	for (int i=0; i<scene->tri->f.size(); i++) {
		ScanlineFill(scene, i, buffer, image);
	}
	return image;
}

QImage zbuffer(Scene* scene, float rx, float ry, float scale) {

	if (rx != 0.f) {
		scene->camera->RotateX(rx);
	}
	
	if (ry != 0.f) {
		scene->camera->RotateY(ry);
	}

	if (scale != 1.f) {
		scene->camera->Scale(scale);
	}
	int width = scene->camera->width;
	int height = scene->camera->width;

	vector<float> buffer(width*height, 1.1f);

	QImage image(width, height, QImage::Format_RGB888);
	image.fill(Qt::black);
	for (int i=0; i<scene->tri->f.size(); i++) {
		ScanlineFill(scene, i, buffer, image);
	}
	return image;
}

void ScanlineFill( Scene *scene, int index, vector<float>& buffer, QImage &image) {
	Point p[3];
	Triangle tri = scene->tri->f[index];
	Camera *camera = scene->camera;

	int ymin = image.height() + 1;
	int ymax = -1;

	for (int i=0; i<3; i++) {
		Point pp = tri.mesh->v[tri.v[i]];
		camera->Project(pp.x, pp.y, pp.z, p[i].x, p[i].y, p[i].z);

		if (p[i].y > ymax) {
			ymax = p[i].y;
		}

		if (p[i].y < ymin) {
			ymin = p[i].y;
		}
	}

	vector< vector<Edge> > slNet(ymax - ymin + 1);
	InitScanLineNewEdgeTable(slNet, p, ymin, ymax, tri);
	ProcessScanLineFill(slNet, ymin, ymax, image, buffer, tri, scene->light);


}

void InitScanLineNewEdgeTable(vector< vector<Edge> >& slNet,
	Point* p, int ymin, int ymax, Triangle tri)
{
	Edge e;
	for(int i = 0; i < 3; i++)
	{
		const Point& Ps = tri.mesh->v[tri.v[i]];
		const Point& Pe = tri.mesh->v[tri.v[(i + 1) % 3]];
		const Vector& Ns = tri.mesh->n[tri.n[i]];
		const Vector& Ne = tri.mesh->n[tri.n[(i + 1) % 3]];
		const Point& ps = p[i];         
		const Point& pe = p[(i + 1) % 3];         
		const Point& pss = p[(i - 1 + 3) % 3];
		const Point& pee = p[(i + 2) % 3];

		if((int)pe.y != (int)ps.y) //skip horizonal line
		{
			e.dx = float((int)pe.x - (int)ps.x) / (float)((int)pe.y - (int)ps.y);
			e.dz = float(pe.z - ps.z) / float(pe.y - ps.y);
			e.dp = Vector(Pe-Ps)/ float(pe.y - ps.y);
			e.dn = Vector(Ne-Ns)/ float(pe.y - ps.y);
			if(pe.y > ps.y)
			{
				e.xi = ps.x;
				e.zi = ps.z;
				e.pi = Ps;
				e.ni = Ns;
				if(pee.y >= pe.y)
					e.ymax = pe.y - 1;
				else
					e.ymax = pe.y;

				slNet[ps.y - ymin].push_back(e);
			}
			else
			{
				e.xi = pe.x;
				e.zi = pe.z;
				e.pi = Pe;
				e.ni = Ne;
				if(pss.y >= ps.y)
					e.ymax = ps.y - 1;
				else
					e.ymax = ps.y;
				slNet[pe.y - ymin].push_back(e);
			}
		}
		else {
		}
	}
}


void ProcessScanLineFill(vector<vector<Edge>>& slNet, int ymin, int ymax, QImage &image, vector<float>& buffer, Triangle tri, Point *light)
{
	vector<Edge> aet;

	int width = image.width();
	int height = image.height();

	for(int y = ymin; y <= ymax; y++)
	{
		aet.insert(aet.end(), slNet[y-ymin].begin(), slNet[y-ymin].end());
		sort (aet.begin(), aet.end());

		std::vector<Edge>::iterator iter = aet.begin();

		if ( y > 0 && y < height) {
			while (iter != aet.end()) {

				int xstart = clamp(iter->xi, 0, width-1);

				float zi = iter->zi;
				Point pi = iter->pi;
				Vector ni = iter->ni;
				if ((iter+1) == aet.end())	break;
				float dz = (iter+1)->zi-iter->zi;
				Vector dp = (iter+1)->pi-iter->pi;
				Vector dn = (iter+1)->ni-iter->ni;
				int xend = clamp((++iter)->xi, 0, width-1);
				dz /= xend-xstart;
				dp /= xend-xstart;
				dn /= xend-xstart;

				for (int x=xstart; x<=xend; x++) {
					if (zi < buffer[y*width+x]) {
						buffer[y*width+x] = zi;
						float brdf = AbsCos((pi-*light), ni);
						image.setPixel(x, y, qRgb(250*brdf, 50*brdf, 255*brdf));
					}
					zi += dz;
					pi += dp;
					ni += dn;
				}
				iter++;
			}
		}
		vector<Edge> copyaet = aet;
		aet.clear();
		for (iter=copyaet.begin(); iter!=copyaet.end(); iter++) {
			if (iter->ymax != y) {
				iter->xi += iter->dx;
				iter->zi += iter->dz;
				aet.push_back(*iter);
			}
		}

	}
}
