#include "zbufferGraphicsView.h"


ZbufferGraphicsView::ZbufferGraphicsView(QWidget *parent): QGraphicsView(parent), ispressed(false){
}

void ZbufferGraphicsView::mousePressEvent(QMouseEvent* mouseevent) {
	
	pos = mouseevent->pos();
	ispressed = true;
}

void ZbufferGraphicsView::mouseMoveEvent(QMouseEvent* mouseevent) {
	if (!ispressed) return;

	float dx = mouseevent->pos().x() - pos.x();
	float dy = mouseevent->pos().y() - pos.y();

	dx /= 10.f;
	dy /= 10.f;
	pos = mouseevent->pos();
	if (abs(dx) > abs(dy))
	{
		ZbufferRender(dx, 0);
	}
	else {
		ZbufferRender(0, dy);
	}
	
	
	
}

void ZbufferGraphicsView::mouseReleaseEvent(QMouseEvent* mouseevent) {
	ispressed = false;
}



void ZbufferGraphicsView::ZbufferRender(QString str) {
	scene = new Scene(str.toLatin1().data());
	QGraphicsScene *qscene = new QGraphicsScene;
	qscene->addPixmap(QPixmap::fromImage(zbuffer(scene)));
	resize(qscene->width()+10, qscene->height()+10);
	setScene(qscene);
}

void ZbufferGraphicsView::ZbufferRender(float rx, float ry, float scale) {
	QGraphicsScene *qscene = new QGraphicsScene;
	qscene->addPixmap(QPixmap::fromImage(zbuffer(scene, rx, ry, scale)));
	resize(qscene->width()+10, qscene->height()+10);
	setScene(qscene);
}

void ZbufferGraphicsView::keyPressEvent(QKeyEvent *event) {

	int key = event->key();

	if (key == 81) {//q
		ZbufferRender(0.f, 0.f, 1.1f);
	}
	else if (key == 87) { //w
		ZbufferRender(0.f, 0.f, 0.9f);
	}
}