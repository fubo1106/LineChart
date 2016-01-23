#ifndef LINEAREA_H
#define LINEAREA_H
#include <QVector2D>
#include <QPointF>
#include <QVector>
#include "line.h"
#include "circle.h"
#include "circlearea.h"
class linearea
{
public:
    linearea();
    linearea(const QVector<double> &px,const QVector<double> &py,const double &linesize,const double &circlesize);//input 2 sets of points.
	linearea(const QVector<QVector2D>& points, double linesize = 1, double circlesize = 7);//input 2 sets of points.

    double cal_Lol(const line &l1,const line &l2);
    double cal_Lol(const QVector2D &s,const QVector2D &o,const QVector2D &e);
    double cal_Angle(const QVector2D &s,const QVector2D &o,const QVector2D &e);
    double cal_Col(const double &r,const double &l);
    double cal_Tri(const QVector2D &s,const QVector2D &o,const QVector2D &e);
    double cal_vis();

    double lineWidth() const;
    void setLineWidth(double lineWidth);

    double circlesize() const;
    void setCirclesize(double circlesize);

	QVector<QVector2D> getPoints();
	void setPoints(const QVector<double> &px, const QVector<double> &py, double linesize = 1, const double circlesize = 7);

	static double HausdorffDist(linearea &l1, linearea &l2);
	static int intesection(linearea &l1, linearea &l2);//get number of intersection points for l1 and l2
	

private:
    QVector<QVector2D> m_Points;
    circlearea cr;

    double m_lineWidth;
    double m_circlesize;

    double m_Lol;
    double m_Col;
    double m_Tri;

    double m_AllLineLength;
    double m_sumLineArea;
    double m_realLineArea;


};

#endif // LINEAREA_H
