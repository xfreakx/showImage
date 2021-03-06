#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageprocess.h"
#include "CL\cl.h"
#include "OpenCL.h"
#include "OpenClDeviceInfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    showMaximized();
    _scene = new QGraphicsScene(this);
    _ui->imageArea->setScene(_scene);
    _ui->imageArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _ui->imageArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	loadOpenCLDevices();
	
	_ui->comboBox->adjustSize();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::loadOpenCLDevices()
{
	OpenCL* opencl = new OpenCL();

	opencl->getOpenCLDeviceList(_devices);

	int devicesCount = _devices.size();

	for (int i = 0; i < devicesCount; i++)
	{
		_ui->comboBox->addItem(_devices[i]._deviceName);
	}
}

void MainWindow::on_openImage_clicked()
{

}

void MainWindow::on_actionOpen_triggered()
{
    _imagePath = QFileDialog::getOpenFileName(this,
                                             tr("Open File"),
                                             "",
                                             tr("Files (*.jpg)")
                                             );

    QPixmap *tmpImage = new QPixmap(_imagePath);

    _imagePreview = new QGraphicsPixmapItem(
                tmpImage->scaled(QSize(800,600))
    );

    _ui->infoBox->setHtml("<strong>Image loaded</strong>");

    _scene->addItem(_imagePreview);

    _image = tmpImage->toImage();


    //ImageProcess *imageProcess = new ImageProcess(&image);

}

void MainWindow::processImage()
{
	int deviceNumber = _ui->comboBox->currentIndex();
	ImageProcess *imageProcessor = new ImageProcess(&_image, *_devices[deviceNumber]._deviceId);
}
