/***************************************************************************
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
using namespace alglib;

double NLfunc(unsigned n, const double *alpha, double *grad, void *data)
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

    area *a=(area *)data;
    a->setR(alpha[0]);
    //grad[0]=2*M_PI*a->m_Points.size()*alpha[0]-a->grad_circle();
	grad[0] = a->grad_percent_line() - a->grad_percent_circle();
	printf("grad:%f funcv: %f\n", grad[0], a->cal_percentcircleare() + a->cal_percentlinearea());
    //return a->cal_totalcirclearea()+a->cal_totallinearea();
	return a->cal_percentcircleare() + a->cal_percentlinearea();

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
  QObject::connect(ControlW->ui->btn_read,SIGNAL(clicked(bool)),this,SLOT(readDate()));
  QObject::connect(ControlW->ui->zeroliney,SIGNAL(valueChanged(double)),this,SLOT(setZerolinex(double)));
  QObject::connect(ControlW->ui->zerolinex,SIGNAL(valueChanged(double)),this,SLOT(setZeroliney(double)));
  plotwidth=300;
  plotheight=300;
  newwidth=plotwidth;
  newheight=plotheight;
  marginwidth=15*2;
  setGeometry(10, 40, plotwidth+marginwidth, plotheight+marginwidth);

  ControlW->ui->label_x->setText(QString("x=%1").arg(plotwidth));
  ControlW->ui->label_y->setText(QString("y=%1").arg(plotheight));


}

float MainWindow::dataProcessing()
{
    m_data = loadData("data/co2-bank.csv");
    //m_slopes = calcSlope(m_data);
    float ratio = m_pBank->run(m_data,	MLC);
    return ratio;
}

void MainWindow::setMarksize(double d)
{
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, d));
    ui->customPlot->replot();
    cr.setR(d/2);
    lr.setCirclesize(d/2);

    m_area.setR(d/2);
    double tm=cr.cal_totalarea();
    ControlW->ui->label_overlapC->setText(QString("%1").arg(m_area.cal_totalcirclearea()));
    double lm=lr.cal_vis();
    ControlW->ui->label_overlapL->setText(QString("%1").arg(m_area.cal_totallinearea()));
    ControlW->ui->label_overlapR->setText(QString("%1").arg(m_area.cal_totalcirclearea()+m_area.cal_totallinearea()));
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
    newwidth=plotwidth/a;
    newheight=plotheight;
    setGeometry(10, 40, newwidth+marginwidth, newheight+marginwidth);
    ControlW->ui->label_x->setText(QString("x=%1").arg(newwidth));
    ControlW->ui->label_y->setText(QString("y=%1").arg(newheight));

}

void MainWindow::setBackground(bool b)
{
    ui->customPlot->layer("grid")->setVisible(b);
    ui->customPlot->replot();
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
         double tmpX=(OX[i]-ui->customPlot->xAxis->range().lower)/changeX;
         double tmpY=(OY[i]-ui->customPlot->yAxis->range().lower)/changeY;
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
    run();

}

double MainWindow::run()
{

    area *ardata = new area(PX,PY,1,7);

    double lb =  1 ;//line size
    nlopt_opt opt;
    opt = nlopt_create(NLOPT_LD_MMA, 1);
    nlopt_set_lower_bounds(opt, &lb);

    nlopt_set_min_objective(opt, NLfunc, (void *)ardata);
    nlopt_set_xtol_rel(opt, 1e-8);
    double aspectRatio = 1.0005;
    double minf;

    if (nlopt_optimize(opt, &aspectRatio, &minf) < 0) {
        printf("nlopt failed!\n");
    }
    else {
        printf("found minimum at f(%g) = %0.10g\n", aspectRatio, minf);
    }


    nlopt_destroy(opt);
    delete ardata;
    return aspectRatio;
}










