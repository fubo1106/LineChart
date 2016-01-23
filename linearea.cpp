#include "linearea.h"
#include <QVector>
#include <qmath.h>
#include <QDebug>
#include "circlearea.h"
linearea::linearea()
{

}

linearea::linearea(const QVector<double> &px,const QVector<double> &py,const double &linesize,const double &circlesize)
{
    m_lineWidth=linesize;
    m_circlesize=circlesize;
    m_Lol=0;
    m_Col=0;
    m_Tri=0;
    for(int i=0;i!=px.size();i++)
    {
        m_Points.push_back(QVector2D(px[i],py[i]));
    }

    for(int i=1;i!=m_Points.size();i++)
    {
        m_AllLineLength+=(m_Points[i]-m_Points[i-1]).length();

    }

    cr=circlearea(px,py,circlesize);
//    for(int i=1;i!=m_Points.size()-1;i++)//Lol
//    {

//        m_Lol+=cal_Lol(m_Points[i-1],m_Points[i],m_Points[i+1]);
//        m_Tri+=cal_Tri(m_Points[i-1],m_Points[i],m_Points[i+1]);
//    }

//    m_Col=(m_Points.size()-1)*cal_Col(m_circlesize,m_lineWidth);

//    m_VisLineArea=m_AllLineLength*m_lineWidth-m_Col+m_Tri;


}

linearea::linearea(const QVector<QVector2D>& points, double linesize, double circlesize){
	m_lineWidth = linesize;
	m_circlesize = circlesize;
	m_Points.clear();
	m_Points = points;
}

double linearea::cal_Lol(const QVector2D &s, const QVector2D &o, const QVector2D &e)
{
    double angle=cal_Angle(s,o,e);

    double h=m_lineWidth/2;
    double l=h/qTan(angle/2);
    return h*l;

}

double linearea::cal_Angle(const QVector2D &s, const QVector2D &o, const QVector2D &e)
{
    double c=(o-s).length();
    double b=(e-o).length();
    double a=(e-s).length();
    double cosA=(qPow(b,2)+qPow(c,2)-qPow(a,2))/(2*b*c);
    return qAcos(cosA);
}

double linearea::cal_Col(const double &r,const double &l)
{
   double ll=l/2;
   double area_tri=qSqrt(qPow(r,2)-qPow(ll,2))*ll;
   double area_sector=qPow(r,2)*qAsin(ll/r);
   return area_tri+area_sector;


}

double linearea::cal_Tri(const QVector2D &s, const QVector2D &o, const QVector2D &e)
{
    double angle=cal_Angle(s,o,e);
    double h=m_lineWidth/2;
    double l=h/qTan(angle/2);

    return l*qSin(angle/2)*l*qCos(angle/2);
}

double linearea::cal_vis()
{
    m_Col=0;
    m_Tri=0;
    m_Col=(m_Points.size()-1)*cal_Col(m_circlesize,m_lineWidth);
    for(int i=1;i!=m_Points.size()-1;i++)//Lol
    {
        m_Tri+=cal_Tri(m_Points[i-1],m_Points[i],m_Points[i+1]);
    }
    double m_VisLineArea=m_AllLineLength*m_lineWidth-m_Col+m_Tri;
    double m_sumLineArea=m_AllLineLength*m_lineWidth;
    return m_VisLineArea;
}
double linearea::lineWidth() const
{
    return m_lineWidth;
}

void linearea::setLineWidth(double lineWidth)
{
    m_lineWidth = lineWidth;
}
double linearea::circlesize() const
{
    return m_circlesize;
}

void linearea::setCirclesize(double circlesize)
{
    m_circlesize = circlesize;
}

QVector<QVector2D> linearea::getPoints(){
	return m_Points;
}

void linearea::setPoints(const QVector<double> &px, const QVector<double> &py, double linesize, double circlesize){
	m_lineWidth = linesize;
	m_circlesize = circlesize;
	
	m_Points.clear();
	for (int i = 0; i != px.size(); i++)
	{
		m_Points.push_back(QVector2D(px[i], py[i]));
	}
}

double linearea::HausdorffDist(linearea &l1, linearea &l2){
	QVector<QVector2D> points1 = l1.getPoints();
	QVector<QVector2D> points2 = l2.getPoints();

	double shortest;
	double h = 0;
	double d;
	for (int i = 0; i < points1.size(); i++){
		shortest = INFINITY;
		for (int j = 0; j < points2.size(); j++){
			d = (points1[i] - points2[j]).length();
			if (d < shortest){
				shortest = d;
			}
		}
		if (shortest > h){
			h = shortest;
		}
	}
	return h;
}

int linearea::intesection(linearea &l1, linearea &l2){
	QVector<QVector2D> points1 = l1.getPoints();
	QVector<QVector2D> points2 = l2.getPoints();

	int num = qMin(points1.size(), points2.size());
	bool largerThan;
	int intersectionNum = 0;

	if (points1[0].y() > points2[0].y())
		largerThan = true;
	else
		largerThan = false;
	for (int i = 1; i < num; i++){
		double y1 = points1[i].y();
		double y2 = points2[i].y();

		if (largerThan && y1 <= y2){
			intersectionNum++;
			largerThan = !largerThan;
		}
		if (!largerThan && y1 >= y2){
			intersectionNum++;
			largerThan = !largerThan;
		}
		
	}
	return intersectionNum;
}









