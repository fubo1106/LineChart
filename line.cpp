#include "line.h"
#include <QDebug>
line::line()
{

}

line::line(const QVector2D &ps, const QVector2D &pe)
{
    if(((pe-ps).y()!=0) && ((pe-ps).x()!=0))
    {
        a=(pe-ps).y()/(pe-ps).x();
        b=ps.y()-a*ps.x();
        double c=pe.y()-a*pe.x();
        if(b!=c)
            qDebug()<<"error";
    }
    else
        qDebug()<<"Special";
}

line::line(const QVector2D &p, const double &k)
{
    a=k;
    b=p.y()-a*p.x();
}
double line::getA() const
{
    return a;
}

void line::setA(double value)
{
    a = value;
}
double line::getB() const
{
    return b;
}

void line::setB(double value)
{
    b = value;
}



