#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zbuffer.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Obj Files (*.obj)"));

	ui->graphicsView->ZbufferRender(fileName);
}
