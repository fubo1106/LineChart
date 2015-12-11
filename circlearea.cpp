#include "circlearea.h"
#include <QVector>
#include "alglib3.9.0/src/alglibmisc.h"
#include "alglib3.9.0/src/ap.h"
#include <QDebug>
#include <QtCore/qmath.h>
using namespace alglib;
circlearea::circlearea()
{

}

circlearea::circlearea(const QVector<double> &px, const QVector<double> &py,const double &r)
{
    for(int i=0;i!=px.size();i++)
    {
        m_Points.push_back(QVector2D(px[i],py[i]));
        alg_Points.setlength(px.size(),2);
        alg_Points[i][0]=px[i];
        alg_Points[i][1]=py[i];
    }
    m_r=r;
    m_sumtotal=M_PI*qPow(r,2)*m_Points.size();
    buildkdtree();
    overlap();
}

void circlearea::buildkdtree()
{
    ae_int_t nx=2;
    ae_int_t ny = 0;
    ae_int_t normtype = 2;
    kdtreebuild(alg_Points,nx,ny,normtype,m_kdtree);

}

void circlearea::overlap()
{
    m_overlaptwo.clear();
    m_overlapthree.clear();
    for(int i=0;i!=m_Points.size();i++)
    {
        real_1d_array p;
        p.setlength(2);
        p[0]=m_Points[i].x();
        p[1]=m_Points[i].y();
        ae_int_t num=kdtreequeryrnn(m_kdtree,p,(m_r)*2);
        if(num==2)
        {
            qDebug()<<num;
            qDebug()<<i;
            real_2d_array result;
            kdtreequeryresultsx(m_kdtree,result);
            //qDebug()<<QString(result.tostring(0).c_str());
            m_overlaptwo.push_back(result);
        }
        if(num==3)
        {
            real_2d_array result;
            kdtreequeryresultsx(m_kdtree,result);
            qDebug()<<QString(result.tostring(0).c_str());
            m_overlapthree.push_back(result);
        }
        if(num>3)
        {
            qDebug()<<num<<" "<<i<<" Mark size is too large";
        }
    }

}

bool circlearea::sametwo(const real_2d_array &p, const real_2d_array &q)
{
    QVector2D a(p[0][0],p[0][1]);
    QVector2D b(p[1][0],p[1][1]);
    QVector2D c(q[0][0],q[0][1]);
    QVector2D d(q[1][0],q[1][1]);

    if((b-a)==(c-d))
        return true;
    else
        return false;
}

double circlearea::cal_overlaptwo(const alglib::real_2d_array &twopoints)
{
    QVector2D a(twopoints[0][0],twopoints[0][1]);
    QVector2D b(twopoints[1][0],twopoints[1][1]);

    double d=(b-a).length();
    double angel=2*qAcos(d/(2*m_r));
    double areaSector=qPow(m_r,2)*angel/2;
    double areaTri=(d/4)*qSqrt(4*qPow(m_r,2)-qPow(d,2));
//    double areaTri=qPow(r,2)*qSin(angel)/2;


//    return areaSector-areaTri;
    return qPow(m_r,2)*(angel-qSin(angel));//another formular
}

double circlearea::cal_overlapthree(const real_2d_array &threepoints)
{
    QVector2D a(threepoints[0][0],threepoints[0][1]);
    QVector2D b(threepoints[1][0],threepoints[1][1]);
    QVector2D c(threepoints[2][0],threepoints[2][1]);
    return 0;

}

double circlearea::cal_totalarea()//probably having bugs for other situation
{
    m_sumtwo=0;
    for(int i=0;i<m_overlaptwo.size()-1;i=i+2)
    {
        if(sametwo(m_overlaptwo[i],m_overlaptwo[i+1]))
            m_sumtwo+=cal_overlaptwo(m_overlaptwo[i]);
        else
            m_sumtwo+=cal_overlaptwo(m_overlaptwo[i])+cal_overlaptwo(m_overlaptwo[i+1]);
    }

    m_realtotal=m_sumtotal-m_sumtwo;
    return m_realtotal;
}
double circlearea::r() const
{
    return m_r;
}

void circlearea::setR(double r)
{
    m_r = r;
    m_sumtotal=M_PI*qPow(r,2)*m_Points.size();
    overlap();
}
double circlearea::sumtotal() const
{
    return m_sumtotal;
}

void circlearea::setSumtotal(double sumtotal)
{
    m_sumtotal = sumtotal;
}



