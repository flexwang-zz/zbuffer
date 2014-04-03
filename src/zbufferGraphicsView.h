#ifndef ZBUFFERGRAPHICSVIEW_H
#define ZBUFFERGRAPHICSVIEW_H

#include <QtWidgets/QGraphicsView>
#include <qevent.h>
#include "zbuffer.h"
#include "scene.h"

class ZbufferGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    ZbufferGraphicsView(QWidget *parent = 0);
	void ZbufferRender(QString str);
	void ZbufferRender(float rx, float ry, float scale = 1.f);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	bool ispressed;
	QPointF pos;
	Scene *scene;
};
	
#endif