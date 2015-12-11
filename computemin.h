#ifndef COMPUTEMIN
#define COMPUTEMIN
#include <QVector>
#include <QtAlgorithms>
#include <QVector2D>
#include "alglib3.9.0/src/optimization.h"
#include "alglib3.9.0/src/stdafx.h"
using namespace alglib;
double computeMin(double a,QVector<double> x,QVector<double> y);
double computeA(double init_i,double step,double maxValue,QVector<double> &x,QVector<double> &y,QVector<double> &ls);

double computeA2(double init_i,double step,double maxValue,
                 QVector<double> &x1,QVector<double> &y1,
                 QVector<double> &x2,QVector<double> &y2,
                 QVector<double> &ls);
double computeSlope(QVector<double> &x,QVector<double> &y);

double computeAWO(QVector<double> &x,QVector<double> &y);

double computeMark(QVector<double> &x,QVector<double> &y,int linesize);//compute marksize and linesize

QVector<int> nearestMark(QVector<double> px,QVector<double> py,int index);
QVector<int> nearestMark(QVector<double> px, QVector<double> py, int index,double r);

void fun_grad(const real_1d_array &x,double &func,real_1d_array &grad,void *ptr);
void grad_compute(const QVector<double> &px,const QVector<double> &py);
#endif // COMPUTEMIN

