﻿/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_form.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "computemin.h"
#include <QtAlgorithms>
#include <time.h>
#include <form.h>
#include <QFileDialog>
#include <QFile>
#include <QtMath>
#include <nlopt.h>
#include <circlearea.h>
#include <linearea.h>
#include <area.h>
#include "alglib3.9.0/src/optimization.h"
#include "alglib3.9.0/src/stdafx.h"
#include <direct.h>
#include <fstream>
using namespace alglib;

double getOptAspectRatio(const QVector<double>& dataX, const QVector<double>& dataY){
	double dxSum = 0;
	double dySum = 0;
	for (int i = 0; i < dataX.size(); i++){
		dxSum += dataX[i];
		dySum += dataY[i];
	}
	return dySum / dxSum;
}

/*every marker percent*/
double EMPfunc(unsigned n, const double *alpha, double *grad, void *data)
{
//    ar_data_slopes *d = (ar_data_slopes *) data;
//    int num = d->num;
//    double *slopes = d->slopes;
//    float derivative = 0;
//    float alpahSquare = alpha[0]*alpha[0];
//    float funcv = 0;
//    for(int i=0;i<num;i++)
//    {
//        float phySlope = fabs(slopes[i]*alpha[0]);
//        float funiv = atan(phySlope) - PI/4;
//        float ideriv = fabs(slopes[i])/(1 + alpahSquare*slopes[i]*slopes[i]);
//        if(funiv<0)
//            derivative += -ideriv;
//        else
//            derivative += ideriv;
//        funcv += fabs(funiv);
//    }
//    grad[0] = derivative/num;
//    return fabs(funcv);
	double funcv = 0;
    area *a=(area *)data;
    a->setR(alpha[0]);

	//funcv = a->cal_percentcircleare();
	//grad[0] = a->grad_C;

	funcv = a->cal_percentcircleare() + a->cal_percentlinearea();
	grad[0] = a->grad_C + a->grad_L;
	//printf("grad:%f funcv: %f\n", grad[0], funcv);
    //return a->cal_totalcirclearea()+a->cal_totallinearea();
	//return a->cal_percentcircleare() + a->cal_percentlinearea();
	return funcv;

}

/*Maximum Visual Entropy*/
double MVEfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	double funcv = 0;
	area *a = (area *)data;
	a->setR(alpha[0]);

	funcv = a->cal_circle_entropy() + a->cal_line_entropy();
	grad[0] = a->derivative_circle_entropy + a->derivative_line_entropy;
	printf("grad:%f funcv: %f\n", grad[0], funcv);
	//return a->cal_totalcirclearea()+a->cal_totallinearea();
	//return a->cal_percentcircleare() + a->cal_percentlinearea();
	return funcv;
}

//maximum visual entropy and display occupation
double MVEOfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	double ve_ratio = 0.9;
	double funcv = 0;
	area *a = (area *)data;
	a->setR(alpha[0]);
	double targetPixelRatio = 0.3;
	funcv = a->cal_visual_display_funcv(ve_ratio, targetPixelRatio);
	grad[0] = a->derivative_visual_display_funcv;
	
	//funcv = a->
	printf("grad:%f funcv: %f\n", grad[0], funcv);
	//return a->cal_totalcirclearea()+a->cal_totallinearea();
	//return a->cal_percentcircleare() + a->cal_percentlinearea();
	return funcv;
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ControlW=new Form();
  ControlW->show();
  //QObject::connect(ControlW->ui->doubleSpinBox, SIGNAL(valueChanged(double)),this->ui->customPlot,SLOT(setMarksize(double)));
  QObject::connect(ControlW->ui->MarkSize, SIGNAL(valueChanged(double)),this,SLOT(setMarksize(double)));
  QObject::connect(ControlW->ui->LineSize,SIGNAL(valueChanged(int)),this,SLOT(setLinesize(int)));
  QObject::connect(ControlW->ui->Aspect,SIGNAL(valueChanged(double)),this,SLOT(setAspect(double)));
  QObject::connect(ControlW->ui->background,SIGNAL(toggled(bool)),this,SLOT(setBackground(bool)));
  QObject::connect(ControlW->ui->save, SIGNAL(toggled(bool)), this, SLOT(Notsave(bool)));
  QObject::connect(ControlW->ui->btn_read, SIGNAL(clicked(bool)), this, SLOT(loadOneCSVData())); 
  QObject::connect(ControlW->ui->btn_read_multi, SIGNAL(clicked(bool)), this, SLOT(loadMultiCSVData()));
  QObject::connect(ControlW->ui->btn_opt_marker, SIGNAL(clicked(bool)), this, SLOT(optMarker()));
  QObject::connect(ControlW->ui->btn_opt_multimarker, SIGNAL(clicked(bool)), this, SLOT(optMultiMarker()));
  QObject::connect(ControlW->ui->btn_opt_ratio, SIGNAL(clicked(bool)), this, SLOT(optRatio()));
  QObject::connect(ControlW->ui->zeroliney,SIGNAL(valueChanged(double)),this,SLOT(setZerolinex(double)));
  QObject::connect(ControlW->ui->zerolinex,SIGNAL(valueChanged(double)),this,SLOT(setZeroliney(double)));
  plotwidth=500;
  plotheight=500; //excel 600*400
  init_markSize = 7;
  init_lineSize = 1;
  blank = 0;
  aspecRatio = 1;
  newwidth = plotwidth - 2 * blank;
  newheight = plotheight - 2 * blank;
  marginwidth=15*2;
  setGeometry(10, 40, plotwidth+marginwidth, plotheight+marginwidth);

  ControlW->ui->label_x->setText(QString("x=%1").arg(plotwidth));
  ControlW->ui->label_y->setText(QString("y=%1").arg(plotheight));

}

void MainWindow::clearData(){
	X.clear(); OX.clear();
	OX.clear(); OY.clear();
	PX.clear(); PY.clear();
	m_data.clear(); m_slopes.clear();
	m_multiData.clear();
	ui->customPlot->clearPlottables();
	ui->customPlot->rescaleAxes(true);
	ui->customPlot->replot();
}

void MainWindow::ratioData(const QVector<double>& OX, const QVector<double>& OY, double ratio)
{
    //m_data = loadData("data/co2-bank.csv");
    ////m_slopes = calcSlope(m_data);
    //float ratio = m_pBank->run(m_data,	MLC);
    //return ratio;
	QVector<double> XX = OX;
	QVector<double> YY = OY;
	qSort(XX);
	qSort(YY);

	double width = newwidth / ratio;
	double pwidth, pheight;
	//pwidth = ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower;
	pwidth = *(XX.end() - 1) - *XX.begin();
	double changeX = pwidth / width;
	//pheight = ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower;
	pheight = *(YY.end() - 1) - *YY.begin();
	double changeY = pheight / newheight;
	/*for (int i = 0; i != X.size(); i++)
	{
	double tmpX = (OX[i] - ui->customPlot->xAxis->range().lower) / changeX;
	double tmpY = (OY[i] - ui->customPlot->yAxis->range().lower) / changeY;
	PX.push_back(tmpX);
	PY.push_back(tmpY);
	}*/
	PX.clear();
	PY.clear();
	for (int i = 0; i != OX.size(); i++)
	{
		double tmpX = (OX[i] - ui->customPlot->xAxis->range().lower) / changeX + blank;
		double tmpY = (OY[i] - ui->customPlot->yAxis->range().lower) / changeY + blank;
		PX.push_back(tmpX);
		PY.push_back(tmpY);
	}
}

void MainWindow::normalizeData(QVector<QVector2D> &data){
	vector<double> datax, datay; 
	for (int i = 0; i < data.size(); i++){
		datax.push_back(data[i].x());
		datay.push_back(data[i].y());
	}
	qSort(datax);
	qSort(datay);

	double width = newwidth;
	double pwidth, pheight;
	//pwidth = ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower;
	pwidth = *(datax.end() - 1) - *datax.begin();
	double changeX = pwidth / width;
	//pheight = ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower;
	pheight = *(datay.end() - 1) - *datay.begin();
	double changeY = pheight / newheight;

	for (int i = 0; i != data.size(); i++)
	{
		double tmpX = (data[i].x() - ui->customPlot->xAxis->range().lower) / changeX + blank;
		double tmpY = (data[i].y() - ui->customPlot->yAxis->range().lower) / changeY + blank;
		data[i].setX(tmpX);
		data[i].setY(tmpY);
	}

}

void MainWindow::dataSelecting(int stride){
	MX.clear();
	MY.clear();
	/*if (OX.size() <= 200){
		MX = OX;
		MY = OY;
	}*/
	//else{//uniform sampling
	//	stride = floor(OX.size() / 100.0);
	//	for (int i = 0; i < OX.size(); i += stride){
	//		MX.push_back(OX[i]);
	//		MY.push_back(OY[i]);
	//	}
	//}
	//else{//local extrema
	//	for (int i = 1; i < OX.size() - 1; i++){
	//		if ((OY[i] >= OY[i - 1] && OY[i] >= OY[i + 1]) || (OY[i] <= OY[i - 1] && OY[i] <= OY[i + 1])){
	//			MX.push_back(OX[i]);
	//			MY.push_back(OY[i]);
	//		}
	//	}
	//}
	//else{//different stride
		//int stride = 6;
		for (int i = 0; i < OX.size(); i=i+stride){
			MX.push_back(OX[i]);
			MY.push_back(OY[i]);
		}
	//}
	//printf("data selection: step=%d  data points=%d.\n", stride, MX.size());
	printf("data selection: %d points.\n", MX.size());
}

void MainWindow::dataSelecting(QVector<int>& indexes){
	MX.clear();
	MY.clear();
	for (int i = 0; i < indexes.size(); i++){
		MX.push_back(OX[indexes[i]]);
		MY.push_back(OY[indexes[i]]);
	}
	printf("data selection: %d points.\n", MX.size());
}

void MainWindow::saveFigure(double ratio, double markersize, int sampleStep){
	string dir = "result/";
	QString dstfile = QString::fromStdString(dir) + fileName + "_ratio=" + QString::number(ratio) + "_originMarker=" + QString::number(markersize) + "_sampleStep=" + QString::number(sampleStep) + ".pdf";
	
	mkdir(dir.c_str());
	ui->customPlot->savePdf(dstfile);
	printf("save result into %s\n", dstfile.toStdString().c_str());
}

void MainWindow::saveMarks(const string filename, vector<Mark>& marks){
	ofstream ofs(filename);
	for (int i = 0; i < marks.size(); i++){
		ofs << "<mark>" << endl
			<< "\t" << "<shape>" << marks[i].getShape() << "</shape>" << endl
			<< "\t" << "<size>" << marks[i].getSize() << "</size>" << endl
			<< "</marker>" << endl;
	}
}

void MainWindow::setMarksize(double d)
{
	/*previous code*/
    /*ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, d));
    ui->customPlot->replot();
    cr.setR(d/2);
    lr.setCirclesize(d/2);

    m_area.setR(d/2);
    double tm=cr.cal_totalarea();
    ControlW->ui->label_overlapC->setText(QString("%1").arg(m_area.cal_totalcirclearea()));
    double lm=lr.cal_vis();
    ControlW->ui->label_overlapL->setText(QString("%1").arg(m_area.cal_totallinearea()));
    ControlW->ui->label_overlapR->setText(QString("%1").arg(m_area.cal_totalcirclearea()+m_area.cal_totallinearea()));*/
	
	ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, d));
	ui->customPlot->replot();
}

void MainWindow::setLinesize(int i)
{
    QPen pen=ui->customPlot->graph(1)->pen();
    pen.setWidth(i);
    lr.setLineWidth(i);
    ui->customPlot->graph(1)->setPen(pen);
    ui->customPlot->replot();

}

void MainWindow::setAspect(double a)
{
	newwidth = plotwidth / a;
    newheight=plotheight;
	ratioData(OX, OY, a);
	ratioData(MX, MY, a);
    setGeometry(10, 40, newwidth+marginwidth, newheight+marginwidth);
    ControlW->ui->label_x->setText(QString("x=%1").arg(newwidth));
    ControlW->ui->label_y->setText(QString("y=%1").arg(newheight));

}

void MainWindow::setBackground(bool b)
{
    ui->customPlot->layer("grid")->setVisible(b);
    ui->customPlot->replot();
}

void MainWindow::Notsave(bool b)
{
	save = false;
}

void MainWindow::setZerolinex(double y)
{
    ui->customPlot->graph(2)->drawZeroline(X,y);
    ui->customPlot->replot();
}

void MainWindow::setZeroliney(double x)
{

    ui->customPlot->graph(3)->drawZeroline(x,Y);
    ui->customPlot->replot();
}

void MainWindow::readDate()
{
     QStringList filenames=QFileDialog::getOpenFileNames(this,QString("Open File XY"),"../");
     for(int i=0;i!=filenames.size();i++)
     {
         if(filenames[i].endsWith("x"))
         {
//             qDebug()<<"x";
             QFile file(filenames[i]);
                 if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                     return;

                 QTextStream in(&file);
                 while (!in.atEnd())
                 {
                     QString line = in.readLine();
                     if(!line.isNull())
                     {
                         X.push_back(line.toDouble());
                         OX.push_back(line.toDouble());
                     }
                 }
         }
         if(filenames[i].endsWith("y"))
         {
//             qDebug()<<"y";
             QFile file(filenames[i]);
                 if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                     return;
                 QTextStream in(&file);
                 while (!in.atEnd())
                 {
                     QString line = in.readLine();
                     if(!line.isNull())
                     {
                         Y.push_back(line.toDouble());
                         OY.push_back(line.toDouble());
                     }
                 }
         }
     }

     ui->customPlot->addGraph();
     ui->customPlot->graph(0)->setData(X, Y);
//     ui->customPlot->graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);
     ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
     ui->customPlot->addGraph();
     ui->customPlot->graph(1)->setData(X,Y);
//     ui->customPlot->graph(1)->setLineStyle(QCPGraph::LineStyle::lsLine);
     qSort(X);//find maximum and minimum value of set X.
     qSort(Y);
     ui->customPlot->addGraph();
     ui->customPlot->graph(2)->drawZeroline(X,ControlW->ui->zeroliney->value());//set x
//     ui->customPlot->graph(2)->setLineStyle(QCPGraph::LineStyle::lsLine);
     ui->customPlot->addGraph();
     ui->customPlot->graph(3)->drawZeroline(ControlW->ui->zerolinex->value(),Y);//set y

     ui->customPlot->xAxis->setTicks(false);//Ticks invisiable
     ui->customPlot->yAxis->setTicks(false);
     ui->customPlot->xAxis->setBasePen(QPen(QColor(200,200,200), 0, Qt::DotLine));//Draw basic axis with dotline style
     ui->customPlot->yAxis->setBasePen(QPen(QColor(200,200,200), 0, Qt::DotLine));
     ui->customPlot->xAxis->setTickLabels(false);
     ui->customPlot->yAxis->setTickLabels(false);
     ui->customPlot->xAxis->setRange(*X.begin(),*(X.end()-1));
     ui->customPlot->yAxis->setRange(*Y.begin(),*(Y.end()-1));
     ControlW->ui->label_rangeX->setText(QString("X is from %1 to %2").arg(*X.begin()).arg(*(X.end()-1)));
     ControlW->ui->label_rangeY->setText(QString("Y is from %1 to %2").arg(*Y.begin()).arg(*(Y.end()-1)));
     ui->customPlot->replot();

     double pwidth,pheight;
     pwidth=ui->customPlot->xAxis->range().upper-ui->customPlot->xAxis->range().lower;
     double changeX=pwidth/newwidth;
     pheight=ui->customPlot->yAxis->range().upper-ui->customPlot->yAxis->range().lower;
     double changeY=pheight/newheight;
     for(int i=0;i!=X.size();i++)
     {
		 double tmpX = (OX[i] - ui->customPlot->xAxis->range().lower) / changeX + blank;
		 double tmpY = (OY[i] - ui->customPlot->yAxis->range().lower) / changeY + blank;
         PX.push_back(tmpX);
         PY.push_back(tmpY);
     }

     QVector<double> ls1,ls2,ls12;
   //计算A，这里i就是a
     double init_i=0.1;
     double step=0.1;
     double maxValue=newwidth;

//     double a1=computeA(init_i,step,maxValue,px1,py1,ls1);
//     cr=circlearea(px1,py1,3.5);
//     m_area=area(px1,py1,1,3.5);
//     grad_compute(px1,py1);
//     lr=linearea(px1,py1,1,2);
   // run();

}

void MainWindow::loadOneCSVData(){
	clearData();
	QString filename = QFileDialog::getOpenFileName(this, QString("Open File XY"), "data/");
	fileName = filename.split('/').last().split('.').first();
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	//printf("load %s\n", filename.toStdString());

	while (!file.atEnd())
	{
		QString strLine = file.readLine();
		QStringList lineStrList = strLine.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

		if (lineStrList.size()>0)
		{
			QStringList strNumbers = lineStrList.at(0).split(',');
			if (strNumbers.size() >= 2)
			{
				double xx = strNumbers.at(0).toDouble();
				double yy = strNumbers.at(1).toDouble();
				//m_data.push_back(pair<double, double>(xx, yy));
				X.push_back(xx); OX.push_back(xx);
				Y.push_back(yy); OY.push_back(yy);
			}
		}
	}
	qDebug() << "loaded" << filename;
	qDebug() << X.size() << "data points";

	entropy entro(OX, OY);
	QVector<int> indexes;
	entro.sample_by_entropy(indexes);
	//dataSelecting(indexes);//get marker points to MX, MY

	dataSelecting(1);
	setAspect(3 / 3.);

	ui->customPlot->addGraph();
	//ui->customPlot->graph(0)->setData(X, Y);
	ui->customPlot->graph(0)->setData(MX, MY);
	//ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	//     ui->customPlot->graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);
	//ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 70));
	ui->customPlot->graph(0)->setPen(QPen(QColor(0, 174, 74))); //(0, 174, 74) (152, 185, 84) (79, 129, 189) (128, 100, 162) (247,150, 70) (201, 210, 0)
	//pair <(0,172,238) (247, 160, 55) (0, 102, 54) (25, 115, 187)> google
	//pair <(255, 0, 0) (255, 165, 0)(0, 255, 0)> google
	//pair <(193, 80, 76) (155, 187, 88) (78, 129, 189) (129, 100, 163)> excel
	//ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
	ui->customPlot->addGraph();
	ui->customPlot->graph(1)->setData(MX, MY);
	//ui->customPlot->graph(1)->setData(X, Y);
	ui->customPlot->graph(1)->setPen(QPen(QColor(0, 174, 74)));
	ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, init_markSize));
	ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
	//     ui->customPlot->graph(1)->setLineStyle(QCPGraph::LineStyle::lsLine);
	qSort(X);//find maximum and minimum value of set X.
	qSort(Y);

	//ui->customPlot->addGraph();
	//ui->customPlot->graph(2)->drawZeroline(X, ControlW->ui->zeroliney->value());//set x
	////     ui->customPlot->graph(2)->setLineStyle(QCPGraph::LineStyle::lsLine);
	//ui->customPlot->addGraph();
	//ui->customPlot->graph(3)->drawZeroline(ControlW->ui->zerolinex->value(), Y);//set y

	ui->customPlot->xAxis->setTicks(false);//Ticks invisiable
	ui->customPlot->yAxis->setTicks(false);
	ui->customPlot->xAxis->setBasePen(QPen(QColor(0, 174, 74), 0, Qt::DotLine));//Draw basic axis with dotline style
	ui->customPlot->yAxis->setBasePen(QPen(QColor(0, 174, 74), 0, Qt::DotLine ));
	ui->customPlot->xAxis->setTickLabels(false);
	ui->customPlot->yAxis->setTickLabels(false);
	ui->customPlot->xAxis->setRange(*X.begin(), *(X.end() - 1));
	ui->customPlot->yAxis->setRange(*Y.begin(), *(Y.end() - 1));
	ControlW->ui->label_rangeX->setText(QString("X is from %1 to %2").arg(*X.begin()).arg(*(X.end() - 1)));
	ControlW->ui->label_rangeY->setText(QString("Y is from %1 to %2").arg(*Y.begin()).arg(*(Y.end() - 1)));
	ui->customPlot->replot();

	//ratioData(OX, OY, 1);//default ratio=1;
	//ratioData(MX, MY, 2/3.);

	//double pwidth, pheight;
	//pwidth = ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower;
	//double changeX = pwidth / newwidth;
	//pheight = ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower;
	//double changeY = pheight / newheight;
	///*for (int i = 0; i != X.size(); i++)
	//{
	//	double tmpX = (OX[i] - ui->customPlot->xAxis->range().lower) / changeX;
	//	double tmpY = (OY[i] - ui->customPlot->yAxis->range().lower) / changeY;
	//	PX.push_back(tmpX);
	//	PY.push_back(tmpY);
	//}*/
	//for (int i = 0; i != X.size(); i++)
	//{
	//	double tmpX = (OX[i] - ui->customPlot->xAxis->range().lower) / changeX + blank;
	//	double tmpY = (OY[i] - ui->customPlot->yAxis->range().lower) / changeY + blank;
	//	PX.push_back(tmpX);
	//	PY.push_back(tmpY);
	//}

	QVector<double> ls1, ls2, ls12;
	//计算A，这里i就是a
	double init_i = 0.1;
	double step = 0.1;
	double maxValue = newwidth;

	//run();
	if (save){
		saveFigure(init_lineSize, init_markSize);
		//ui->customPlot->savePdf(fileName + "_ratio=" + QString::number(1) + "_originMarker=" + QString::number(7) + ".pdf");
	}	
	return;
}

void MainWindow::loadMultiCSVData(){
	QStringList filenames = QFileDialog::getOpenFileNames(this, QString("Open File XY"), "data/");
	QVector<QVector2D> dxy;
	QVector2D xy;
	for (int i = 0; i != filenames.size(); i++)
	{
		QFile file(filenames[i]);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		while (!file.atEnd())
		{
			QString strLine = file.readLine();
			QStringList lineStrList = strLine.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

			if (lineStrList.size()>0)
			{
				QStringList strNumbers = lineStrList.at(0).split(',');
				if (strNumbers.size() >= 2)
				{
					double xx = strNumbers.at(0).toDouble();
					double yy = strNumbers.at(1).toDouble();
					//m_data.push_back(pair<double, double>(xx, yy));
					xy.setX(xx) ; xy.setY(yy);
					dxy.push_back(xy);
				}
			}
		}
		//normalizeData(dxy);
		m_multiData.push_back(dxy);
		dxy.clear();
	}
	
	

	/*visualize muti line and markers*/

}

void runLocalOptimizer(nlopt_opt opt, double &goodMS, double &goodInit, double &minMinf, double lb, double ub)
{
	int numInitialization = 20;
	double minfunc = minMinf;
	for (int ii = 0; ii<numInitialization; ii++)
	{
		double markersize = lb + fabs(rand() / double(RAND_MAX)) * (ub - lb);
		//double markersize = goodInit;
		double initaR = markersize;
		double minf = 1;
		if (nlopt_optimize(opt, &markersize, &minf) < 0) {
			printf("nlopt failed!\n");
		}
		else {
			if (minf<minMinf)
			{
				minMinf = minf;
				goodMS = markersize;
				goodInit = initaR;
			}
			//printf("found minimum at f(%g) = %0.10g from initial %g\n", markersize, minf, initaR);

		}
	}
}

double MainWindow::run(int &goodStep)
{
	/*aspecRatio = getOptAspectRatio(X, Y);
	printf("found optimal aspect ratio:%f\n", aspecRatio);
	setAspect(aspecRatio);*/
#if 0 //select optimal step
	double minf = 1; //minmum overlap percentage
	double optMS = 0;
	for (int step = 1; step < OX.size() - 1; step++){
		double lineWidth = ControlW->ui->LineSize->value();
		double markerSize = ControlW->ui->MarkSize->value();
		double lb = lineWidth / 2;//line size
		//area *ardata = new area(PX, PY, lineWidth, markerSize);
		dataSelecting(step);
		setAspect();
		area *ardata = new area(PX, PY, lineWidth, markerSize);
		double ub = ardata->getupperbound(PX, PY, lb);
		if (lb <= ub)
			printf("marker radius should between [%f, %f]\n", lb, ub);
		else{
			printf("cannot optimize marker radius: lb=%f and ub=%f, step++\n", lb, ub);
			//return -1;
			continue;
		}
		if (markerSize < 2 * lb || markerSize>2 * ub){
			/*printf("marker point size overflow!!!\n");
			return markerSize;*/
			markerSize = ub;
		}
		nlopt_opt opt;
		opt = nlopt_create(NLOPT_LD_MMA, 1);
		nlopt_set_lower_bounds(opt, &lb);
		nlopt_set_upper_bounds(opt, &ub);

		//nlopt_set_min_objective(opt, AMPfunc, (void *)ardata);
		nlopt_set_min_objective(opt, EMPfunc, (void *)ardata);
		nlopt_set_xtol_rel(opt, 1e-2);
		double aspectRatio = 1.0005;
		double markersize = markerSize;

		/*aspect ratio optimization*/
		/*if (nlopt_optimize(opt, &aspectRatio, &minf) < 0) {
		printf("nlopt failed!\n");
		}
		else {
		printf("found minimum at f(%g) = %0.10g\n", aspectRatio, minf);
		}*/

		/*marker size optimization*/
		/*if (nlopt_optimize(opt, &markersize, &minf) < 0) {
		printf("nlopt failed!\n");
		}
		else {
		printf("found minimum at f(%g) = %0.10g\n", markersize, minf);
		}*/

		double minMinf = 10e+6;
		double minMindx = 1000;
		double goodMS = 0;
		double goodII = ub;
		runLocalOptimizer(opt, goodMS, goodII, minMinf, lb, ub);
		printf("found optimal circle radius=%f at step=%d minimum function value=%f\n", goodMS, step, minMinf);
		nlopt_destroy(opt);
		delete ardata;
		if (minMinf > minf)
			break;
		else{
			minf = minMinf;
			optMS = goodMS;
			goodStep = step;
		}
	}
	printf("\nfound final circle radius:%f at step=%d minimum function value:%f\n", optMS, goodStep, minf);
	return 2 * optMS;
#else //optimize using step 1
	double optMS = 0;
	goodStep = 1;
	/*dataSelecting(goodStep);
	setAspect();*/
	double lineWidth = ControlW->ui->LineSize->value();
	double markerSize = ControlW->ui->MarkSize->value();
	double lb = lineWidth / 2;//line size
	//area *ardata = new area(PX, PY, lineWidth, markerSize);
	area *ardata = new area(PX, PY, lineWidth, markerSize, newwidth, newheight);
	double ub = ardata->getupperbound(PX, PY, lb);
	if (lb <= ub)
		printf("marker radius should between [%f, %f]\n", lb, ub);
	else{
		printf("cannot optimize marker radius: lb=%f and ub=%f\n", lb, ub);
		//return -1;
	}
	if (markerSize < 2 * lb || markerSize>2 * ub){
		/*printf("marker point size overflow!!!\n");
		return markerSize;*/
		markerSize = ub;
	}
	nlopt_opt opt;
	opt = nlopt_create(NLOPT_LD_MMA, 1);
	nlopt_set_lower_bounds(opt, &lb);
	nlopt_set_upper_bounds(opt, &ub);

	//nlopt_set_min_objective(opt, AMPfunc, (void *)ardata);
	//nlopt_set_min_objective(opt, EMPfunc, (void *)ardata);
	//nlopt_set_min_objective(opt, MVEfunc, (void *)ardata); 
	nlopt_set_min_objective(opt, MVEOfunc, (void *)ardata);
	nlopt_set_xtol_rel(opt, 1e-2);
	double aspectRatio = 1.0005;
	double markersize = markerSize;

	double minMinf = 10e+6;
	double minMindx = 1000;
	double goodMS = 0;
	double goodII = ub;
	runLocalOptimizer(opt, goodMS, goodII, minMinf, lb, ub);
	printf("found optimal circle radius=%f minimum function value=%f\n", goodMS, minMinf);
	nlopt_destroy(opt);
	delete ardata;
	return 2 * goodMS;

#endif
	
}

void MainWindow::optRatio(){
	aspecRatio = getOptAspectRatio(X, Y);
	printf("found optimal aspect ratio:%f\n", aspecRatio);
	setAspect(aspecRatio);
}

void MainWindow::optMarker(){
	int goodStep;
	time_t start = clock();
	double markersize = run(goodStep);
	time_t end = clock();
	printf("ellapsed time %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	if (markersize != -1){
		//replot qcustomPlot
		dataSelecting(goodStep);
		ui->customPlot->graph(1)->setData(MX, MY);
		ui->customPlot->graph(1)->setPen(QPen(QColor(0, 174, 74)));
		ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
		ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
		setMarksize(markersize);
		if (save){
			saveFigure(aspecRatio, markersize, goodStep);
			//ui->customPlot->savePdf(fileName + "_ratio=" + QString::number(aspecRatio) + "_optMarker=" + QString::number(markersize) + ".pdf");
		}
		printf("optimal mark size: %f\n", markersize);
	}	
	return;
}

void MainWindow::optMultiMarker(){
	QVector<double> datax, datay;
	int goodStep;

	/*calculate intersections and distance for every 2 lines*/
	int **overlapPoints;
	double **distance;
	overlapPoints = new int*[m_multiData.size()];
	distance = new double*[m_multiData.size()];

	for (int i = 0; i < m_multiData.size(); i++){
		overlapPoints[i] = new int[m_multiData.size()];
		distance[i] = new double[m_multiData.size()];
	}

	for (int i = 0; i < m_multiData.size(); i++){
		overlapPoints[i][i] = 0;
		distance[i][i] = 0;
		for (int j = i + 1; j < m_multiData.size(); j++){
			linearea l1(m_multiData[i]);
			linearea l2(m_multiData[j]);

			double dist = linearea::HausdorffDist(l1, l2);
			int num = linearea::intesection(l1, l2);

			overlapPoints[i][j] = num;
			overlapPoints[j][i] = num;
			distance[i][j] = dist;
			distance[j][i] = dist;
			printf("distance[%d][%d]:%f\n", i, j, distance[i][j]);
			printf("overlapPoints[%d][%d]:%d\n", i, j, overlapPoints[i][j]);
		}
	}

	vector<Mark> marks;
	Mark mark;
	marks.resize(m_multiData.size());
	/*select mark shape and line color according to the intersection points and line distance*/
	/*1. for line color: set different colors according excel or colorbrewer*/
	/*2. for mark shape: set shape combines that have the minimum overlap at intersection points.
	% '-o' circle
	% '-s' square
	% '-d' diamond
	% '-^' upward-pointing triangle
	% '-v' downward-pointing triangle
	% '->' rightward-pointing triangle
	% '-<' leftward-pointing triangle
	% '-p' pentagram (five-pointed star)
	% '-h' hexagram (six-pointed star) 
	Proposed combinations:
	1. '-o' <=> '-^' 
	2. '-s' <=> '-^'
	3. '-d' <=> '-^'
	4. '-^' <=> '-o' '-s' '-d'*/
	for (int shape1 = 0; shape1 < m_multiData.size(); shape1++){
		for (int shape2 = 0; shape2 < m_multiData.size(); shape2++){
			for (int shape3 = 0; shape3 < m_multiData.size(); shape3++){
				for (int shape4 = 0; shape4 < m_multiData.size(); shape4++){
					
				}
			}
		}
	}
	/*optimize mark size for each line*/
	for (int i = 0; i < m_multiData.size(); i++){
		datax.clear();
		datay.clear();
		for (int j = 0; j < m_multiData[i].size(); j++){
			datax.push_back(m_multiData[i][j].x());
			datay.push_back(m_multiData[i][j].y());
		}
		ratioData(datax, datay);
 		double markersize = run(goodStep);
		marks[i].setSize(markersize);
	}

	delete overlapPoints;
	delete distance;
	saveMarks("a.xml", marks);
}










