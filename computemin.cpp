#include "computemin.h"
#include <algorithm>
#include <time.h>
#include <qmath.h>
#include "alglib3.9.0/src/optimization.h"
#include "alglib3.9.0/src/stdafx.h"
#include "area.h"
#include <QDebug>
using namespace alglib;
double computeMin(double a,QVector<double> x,QVector<double> y)
{
    double l=0;
    QVector<double> xx,yy;
    for(int i=x.size();i>1;i--)
    {
        double tmp=x[i-1]-x[i-2];
        xx.push_back(tmp);
    }
    for(int i=y.size();i>1;i--)
    {
        double tmp=y[i-1]-y[i-2];
        yy.push_back(tmp);
    }

    for(int i=0;i!=xx.size();i++)
    {
        l+=qSqrt((xx[i]*xx[i])/a+a*yy[i]*yy[i]);
    }
    return l;
}
double computeMin2(double a,QVector<double> &x1,QVector<double> &y1,QVector<double> &x2,QVector<double> &y2)
{
    double l=0;
    QVector<double> xx1,yy1,xx2,yy2;
    for(int i=x1.size();i>1;i--)
    {
        double tmp=x1[i-1]-x1[i-2];
        xx1.push_back(tmp);
    }
    for(int i=y1.size();i>1;i--)
    {
        double tmp=y1[i-1]-y1[i-2];
        yy1.push_back(tmp);
    }

    for(int i=x2.size();i>1;i--)
    {
        double tmp=x2[i-1]-x2[i-2];
        xx2.push_back(tmp);
    }
    for(int i=y2.size();i>1;i--)
    {
        double tmp=y2[i-1]-y2[i-2];
        yy2.push_back(tmp);
    }
    for(int i=0;i!=xx1.size();i++)
    {
        l+=sqrt((xx1[i]*xx1[i])/a+a*yy1[i]*yy1[i]);
    }
    for(int i=0;i!=xx2.size();i++)
    {
        l+=sqrt((xx2[i]*xx2[i])/a+a*yy2[i]*yy2[i]);
    }
    return l;
}

double computeA(double init_i,double step,double maxValue,
                QVector<double> &x,QVector<double> &y,
                QVector<double> &ls)
{
    for(double i=init_i;i<maxValue;i=i+step)
    {
        double tmp=computeMin(i,x,y);
        ls.push_back(tmp);
    }
    std::vector<double> tmp=ls.toStdVector();
    std::vector<double>::iterator pos;
    pos=min_element(tmp.begin(),tmp.end());
    double minl=*pos;
    int numpos;
    numpos=ls.indexOf(minl);
    double a2=init_i+step*numpos;
    return sqrt(a2);
}

double computeA2(double init_i,double step,double maxValue,
                 QVector<double> &x1,QVector<double> &y1,
                 QVector<double> &x2,QVector<double> &y2,
                 QVector<double> &ls)
{
    for(double i=init_i;i<maxValue;i=i+step)
    {
        double tmp=computeMin2(i,x1,y1,x2,y2);
        ls.push_back(tmp);
    }
    std::vector<double> tmp=ls.toStdVector();
    std::vector<double>::iterator pos;
    pos=min_element(tmp.begin(),tmp.end());
    double minl=*pos;
    int numpos;
    numpos=ls.indexOf(minl);
    double a2=init_i+step*numpos;
    return sqrt(a2);
}




double computeSlope(QVector<double> &x, QVector<double> &y)
{
    QVector<double> slopes;
    for(int i=1;i!=x.size();i++)
    {
        double slp=(y[i]-y[i-1])/(x[i]-x[i-1]);
        slopes.push_back(slp);
    }
    qSort(slopes);
    int mid=x.size()/2;
    double a=1/qAbs(slopes[mid]);
    return a;
}


double computeAWO(QVector<double> &x, QVector<double> &y, double &a)
{
    double sumlength=0;
    double weight=0;
    double besta;
    for(int i=1;i!=x.size();i++)
    {
        double slope=qAbs((y[i]-y[i-1])/(x[i]-x[i-1]));
        double length=(y[i]-y[i-1])*(y[i]-y[i-1])+(x[i]-x[i-1])*(x[i]-x[i-1]);
        weight+=slope*length;
        sumlength+=length;

    }
    return besta;
}


double computeMark(QVector<double> &x, QVector<double> &y, int linesize)
{
    QVector<double> linemarks;
    for(double mark=1.05;mark<5;mark+=0.05)
    {
        double sumarealine=0;
        double sumareamark=(x.size()-1)*(qAsin(linesize/(2*mark))/90*3.1415*mark*mark+linesize/2*qSqrt(mark*mark-(linesize/2)*(linesize/2)));
        for(int i=1;i!=x.size();i++)
        {
            double area=(y[i]-y[i-1])*(y[i]-y[i-1])+(x[i]-x[i-1])*(x[i]-x[i-1])*linesize;
            sumarealine+=area;
        }
        sumarealine=sumarealine-sumareamark;
        linemarks.push_back(sumarealine);
    }
    std::vector<double> stdlinemarks=linemarks.toStdVector();
    double max=*max_element(stdlinemarks.begin(),stdlinemarks.end());
    return max;
}




QVector<int> nearestMark(QVector<double> px,QVector<double> py,int index)//输入数据点和需要检测的点的索引值，输出距离该点最近的两个点的索引
{
    QVector<int> nearTwo;
    QVector<double> lengths;
    double length=0;
    QVector2D startp(px[index],py[index]);
    for(int i=0;i!=px.size();i++)
    {
        if(i==index)
            continue;
        QVector2D endp(px[i],py[i]);
        length=(startp-endp).length();
        lengths.push_back(length);
    }
    std::vector<double> stdlengths=lengths.toStdVector();
    std::vector<double>::iterator pos=min_element(stdlengths.begin(),stdlengths.end());
    int numpos;
    numpos=lengths.indexOf(*pos);
    nearTwo.push_back(numpos);
    stdlengths.at(numpos)=9999;
    pos=min_element(stdlengths.begin(),stdlengths.end());
    numpos=lengths.indexOf(*pos);
    stdlengths.at(numpos)=9999;
    nearTwo.push_back(numpos);

    return nearTwo;
}

QVector<int> nearestMark(QVector<double> px, QVector<double> py, int index,double r)//输入数据点和需要检测的点的索引值，半径。输出在该半径内所有点的索引
{
    QVector<int> inRadious;
    QVector2D startp(px[index],py[index]);
    for(int i=0;i!=px.size();i++)
    {
        if(i==index)
            continue;
        QVector2D endp(px[i],py[i]);
        double length=(startp-endp).length();
        if(length<(2*r))
            inRadious.push_back(i);
    }
    return inRadious;
}

static QVector<double> xxx,yyy;
void grad_compute(const QVector<double> &px,const QVector<double> &py)
{
    real_1d_array x = "[20]";
    double epsg = 1.0e-6;
    double epsf = 0;
    double epsx = 0;
    ae_int_t maxits = 0;
    mincgstate state;
    mincgreport rep;
    xxx=px;
    yyy=py;
    mincgcreate(x, state);
    mincgsetcond(state, epsg, epsf, epsx, maxits);
    alglib::mincgoptimize(state,fun_grad);
    mincgresults(state, x, rep);

    qDebug()<<(x.tostring(5).c_str());
}

void fun_grad(const real_1d_array &x, double &func, real_1d_array &grad, void *ptr)
{
    area *test=new area(xxx,yyy,1,x[0]);
    qDebug()<<x[0];
    //setR(x[0]);
    //setLinesize(x[1]);
    func = test->cal_totalcirclearea()+test->cal_totallinearea();
    grad[0] = 2*M_PI*test->m_Points.size()*test->m_r-test->grad_circle();
    //grad[1] = 4*pow(x[1]-3,3);
    qDebug()<<func<<" "<<grad[0];
    delete test;
}
