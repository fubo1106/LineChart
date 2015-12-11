﻿#ifndef AREA_H
#define AREA_H
#include "alglib3.9.0/src/alglibmisc.h"
#include <QVector>
#include <QVector2D>
#include <QObject>
using namespace alglib;
class area
{
public:
    area();
    area(const QVector<double> &px,const QVector<double> &py,const double &linesize,const double &circlesize);
    void buildkdtree();
    void overlap();
    bool sametwo(const alglib::real_2d_array &p,const alglib::real_2d_array &q);
    double cal_overlaptwo(const alglib::real_2d_array &twopoints);
    double cal_totalcirclearea();
	double cal_percentcircleare();

    double cal_Angle(const QVector2D &s,const QVector2D &o,const QVector2D &e);
    double cal_Col(const double &r,const double &l);
    double cal_Tri(const QVector2D &s, const QVector2D &o, const QVector2D &e);
    double cal_coverline();
    double cal_totallinearea();
	double cal_percentlinearea();


    void fun_grad(const real_1d_array &x, double &func, real_1d_array &grad, void *ptr);
    void cal_grad();
    double grad_circle();
	double grad_percent_circle();
    double grad_line();
	double grad_percent_line();
    double r() const;
    void setR(double r);



    double linesize() const;
    void setLinesize(double linesize);

public:
    QVector<QVector2D> m_Points;
    real_2d_array alg_Points;
    kdtree m_kdtree;
    QVector<real_2d_array> m_overlaptwopoint;

    double m_r;
    double m_linesize;
    double m_length;
    double m_Col;
    double m_CoC;
    double sum_C;
    double sum_L;
    double vis_C;
	double vis_per_C;
    double vis_L;
	double vis_per_L;


};

#endif // AREA_H
