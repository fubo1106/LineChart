#include "area.h"
#include <QVector2D>
#include "alglib3.9.0/src/alglibmisc.h"
#include "alglib3.9.0/src/ap.h"
#include "alglib3.9.0/src/stdafx.h"
#include "alglib3.9.0/src/optimization.h"
#include <QDebug>
#include <QtCore/qmath.h>
using namespace alglib;
area::area()
{

}

area::area(const QVector<double> &px, const QVector<double> &py, const double &linesize, const double &circlesize)
{
    m_linesize=linesize;
    m_r=circlesize/2;
    m_CoC=0;
    m_Col=0;
    m_length=0;

    for(int i=0;i!=px.size();i++)
    {
        m_Points.push_back(QVector2D(px[i],py[i]));
        alg_Points.setlength(px.size(),2);
        alg_Points[i][0]=px[i];
        alg_Points[i][1]=py[i];
    }

    for(int i=1;i!=m_Points.size();i++)
    {
        m_length+=(m_Points[i]-m_Points[i-1]).length();
    }
    sum_C=M_PI*qPow(m_r,2)*m_Points.size();
	sum_L = m_linesize*m_length;
    buildkdtree();
    overlap();
}

void area::buildkdtree()
{
    ae_int_t nx=2;
    ae_int_t ny = 0;
    ae_int_t normtype = 2;
    kdtreebuild(alg_Points,nx,ny,normtype,m_kdtree);
}

void area::overlap()
{
    m_overlaptwopoint.clear();

    for(int i=0;i!=m_Points.size();i++)
    {
        real_1d_array p;
        p.setlength(2);
        p[0]=m_Points[i].x();
        p[1]=m_Points[i].y();
        ae_int_t num=kdtreequeryrnn(m_kdtree,p,(m_r)*2);
        if(num==2)
        {
            //qDebug()<<num;
            //qDebug()<<i;
            real_2d_array result;
            kdtreequeryresultsx(m_kdtree,result);
            printf("overlap 2: pair:%[%f %f],[%f %f]\n", result[0][0], result[0][1], result[1][0], result[1][1]);
            m_overlaptwopoint.push_back(result);
        }
        if(num==3)
        {
            real_2d_array result;
            kdtreequeryresultsx(m_kdtree,result);
			//printf("overlap 3: marker:[%f %f] pair:[%f %f] [%f %f]\n", p[0], p[1], result[0][0], result[0][1], result[1][0], result[1][1]);
			printf("overlap 3: pair:[%f %f] [%f %f] [%f %f]\n", result(0, 0), result(0, 1), result(1, 0), result(1, 1), result(2, 0), result(2, 1));
			QVector2D a(result(1,0), result(1,1));
			QVector2D b(result(2,0), result(2,1));
			double len = (b - a).length();

			if (len < (m_r)* 2)
				m_overlapallthreepoint.push_back(result); //两两相交
			else
				m_overlapthreepoint.push_back(result);

			//qDebug()<<QString(result.tostring(0).c_str());
            //m_overlapthree.push_back(result);
        }
        if(num>3)
        {
            qDebug()<<num<<" "<<i<<" Mark size is too large";
			//exit(0);
        }
    }
}

bool area::sametwo(const real_2d_array &p, const real_2d_array &q)
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

double area::cal_overlaptwo(const real_2d_array &twopoints)
{//two circle's overlap area
    QVector2D a(twopoints[0][0],twopoints[0][1]);
    QVector2D b(twopoints[1][0],twopoints[1][1]);

    double d=(b-a).length();
    double angel=2*qAcos(d/(2*m_r));
//    double areaSector=qPow(m_r,2)*angel/2;
//    double areaTri=(d/4)*qSqrt(4*qPow(m_r,2)-qPow(d,2));
//    return areaSector-areaTri;
    return qPow(m_r,2)*(angel-qSin(angel));//another formular
}

double area::cal_overlapthree(const real_2d_array &twopoints){
	QVector2D a(twopoints(0, 0), twopoints(0, 1));
	QVector2D b(twopoints(1, 0), twopoints(1, 1));
	QVector2D c(twopoints(2, 0), twopoints(2, 1));
	
	double d1 = (b - a).length();
	double angel1 = 2 * qAcos(d1 / (2 * m_r));
	double overlaop1 = qPow(m_r, 2)*(angel1 - qSin(angel1));//another formular

	double d2 = (c - a).length();
	double angel2 = 2 * qAcos(d2 / (2 * m_r));
	double overlaop2 = qPow(m_r, 2)*(angel2 - qSin(angel2));//another formular

	return overlaop1 + overlaop2;
}

double area::cal_overlapallthree(const real_2d_array &twopoints){
	
	double overlap = cal_overlapthree(twopoints);

	//common area
	double common_area = 0;
	QVector2D a(twopoints(0, 0), twopoints(0, 1));
	QVector2D b(twopoints(1, 0), twopoints(1, 1));
	QVector2D c(twopoints(2, 0), twopoints(2, 1));

	QVector2D ab = b - a;
	QVector2D ac = c - a;
	//case1: 3 circles in a line
	if (ab[0] / ac[0] == ab[1] / ac[1]){
		real_2d_array common;
		common.setlength(2, 2);
		common(0, 0) = twopoints(1, 0);
		common(0, 1) = twopoints(1, 1);
		common(1, 0) = twopoints(2, 0);
		common(1, 1) = twopoints(2, 1);
		common_area = cal_overlaptwo(common);
	}
	//case2: 3 circle not in a line
	else{

	}

	return overlap - common_area;
}

double area::cal_totalcirclearea()
{
    double m_sumtwo=0;
    overlap();
    for(int i=0;i<m_overlaptwopoint.size()-1;i=i+2)
    {
        if(sametwo(m_overlaptwopoint[i],m_overlaptwopoint[i+1]))
            m_sumtwo+=cal_overlaptwo(m_overlaptwopoint[i]);
        else
            m_sumtwo+=cal_overlaptwo(m_overlaptwopoint[i])+cal_overlaptwo(m_overlaptwopoint[i+1]);
    }
	vis_C = sum_C - m_sumtwo;
    return vis_C;
}

double area::cal_percentcircleare(){
	double m_sumtwo = 0;
	overlap();
	for (int i = 0; i<m_overlaptwopoint.size() - 1; i = i + 2)
	{
		if (sametwo(m_overlaptwopoint[i], m_overlaptwopoint[i + 1]))
			m_sumtwo += cal_overlaptwo(m_overlaptwopoint[i]);
		else
			m_sumtwo += cal_overlaptwo(m_overlaptwopoint[i]) + cal_overlaptwo(m_overlaptwopoint[i + 1]);
	}
	for (int i = 0; i < m_overlapthreepoint.size(); i++){
		m_sumtwo += cal_overlapthree(m_overlaptwopoint[i]);
	}
	for (int i = 0; i < m_overlapallthreepoint.size(); i++){
		m_sumtwo += cal_overlapallthree(m_overlapallthreepoint[i]);;
	}
	vis_per_C = (sum_C - m_sumtwo)/sum_C;
	return vis_per_C;
}

double area::cal_Angle(const QVector2D &s, const QVector2D &o, const QVector2D &e)
{
    double c=(o-s).length();
    double b=(e-o).length();
    double a=(e-s).length();
    double cosA=(qPow(b,2)+qPow(c,2)-qPow(a,2))/(2*b*c);
    return qAcos(cosA);
}

double area::cal_Col(const double &r, const double &l)
{
    double ll=l/2;
    double area_tri=qSqrt(qPow(r,2)-qPow(ll,2))*ll;
    double area_sector=qPow(r,2)*qAsin(ll/r);
    return area_tri+area_sector;
}

double area::cal_Tri(const QVector2D &s, const QVector2D &o, const QVector2D &e)
{
    double angle=cal_Angle(s,o,e);
    double h=m_linesize/2;
    double l=h/qTan(angle/2);

    return l*qSin(angle/2)*l*qCos(angle/2);
}

double area::cal_coverline()
{
    double len=0;
    for(int i=0;i!=m_overlaptwopoint.size();i++)
    {
        QVector2D a(m_overlaptwopoint[i](0,0),m_overlaptwopoint[i](0,1));
        QVector2D b(m_overlaptwopoint[i](1,0),m_overlaptwopoint[i](1,1));
        double d=(b-a).length();
        len+=d;
    }
    return len*m_linesize/2;
}

double area::cal_totallinearea()
{
    m_Col=0;
    int numoverlaptwo=m_overlaptwopoint.size()/2;
    double m_Tri=0;
    m_Col=2*(m_Points.size()-numoverlaptwo)*cal_Col(m_r,m_linesize);
    for(int i=1;i!=m_Points.size()-1;i++)//Lol
    {
        m_Tri+=cal_Tri(m_Points[i-1],m_Points[i],m_Points[i+1]);
    }
    double vis_L=m_length*m_linesize-m_Col-cal_coverline()+m_Tri;

    return vis_L;
}

double area::cal_percentlinearea(){
	m_Col = 0;
	int numoverlaptwo = m_overlaptwopoint.size() / 2;
	double m_Tri = 0;
	m_Col = 2 * (m_Points.size() - numoverlaptwo)*cal_Col(m_r, m_linesize);
	for (int i = 1; i != m_Points.size() - 1; i++)//Lol
	{
		m_Tri += cal_Tri(m_Points[i - 1], m_Points[i], m_Points[i + 1]);
	}
	vis_per_L = (m_length*m_linesize - m_Col - cal_coverline() + m_Tri) / sum_L;
	
	return vis_per_L;
}

double area::cal_area_from_three_vetex(const QVector2D &v1, const QVector2D &v2, const QVector2D &v3){
	double Ax = v1.x();
	double Ay = v1.y();

	double Bx = v2.x();
	double By = v2.y();

	double Cx = v3.x();
	double Cy = v3.y();

	return fabs(Ax*(By - Cy) + Bx*(Cy - Ay) + Cx*(Ay - By)) / 2;
}

void area::fun_grad(const real_1d_array &x, double &func, real_1d_array &grad, void *ptr)
{
    setR(x[0]);
    //setLinesize(x[1]);
    func = cal_totalcirclearea()+cal_totallinearea();
    grad[0] = 2*M_PI*m_Points.size()*m_r-grad_circle()+grad_line();
    //grad[1] = 4*pow(x[1]-3,3);
}

void area::cal_grad()
{
    real_1d_array x = "[0]";
    double epsg = 1.0e-6;
    double epsf = 0;
    double epsx = 0;
    ae_int_t maxits = 0;
    mincgstate state;
    mincgreport rep;

    mincgcreate(x, state);
    mincgsetcond(state, epsg, epsf, epsx, maxits);
    //alglib::mincgoptimize(state,fun_grad);
    mincgresults(state, x, rep);

    qDebug()<<(x.tostring(5).c_str());
}

double area::grad_circle()
{
    double grad=0;
    for(int i=0;i<m_overlaptwopoint.size();i++)
    {
        QVector2D a(m_overlaptwopoint[i](0,0),m_overlaptwopoint[i](0,1));
        QVector2D b(m_overlaptwopoint[i](1,0),m_overlaptwopoint[i](1,1));
        double d=(b-a).length();

        double temp=pow(m_r,2)*((d/(pow(m_r,2)*sqrt(1-pow(d/(2*m_r),2))))-(d*cos(2*acos(d/(2*m_r)))/(pow(m_r,2)*sqrt(1-pow(d/(2*m_r),2)))))
                +2*m_r*(2*acos(d/(2*m_r))-sin(2*acos(d/(2*m_r))));

        grad+=temp;
    }
    return grad/2;
}

double area::grad_percent_circle(){
	double grad = 0;
	for (int i = 0; i<m_overlaptwopoint.size(); i++)
	{
		QVector2D a(m_overlaptwopoint[i](0, 0), m_overlaptwopoint[i](0, 1));
		QVector2D b(m_overlaptwopoint[i](1, 0), m_overlaptwopoint[i](1, 1));
		double d = (b - a).length();

		double temp = ((d / (pow(m_r, 2)*sqrt(1 - (pow(d, 2) / (4 * pow(m_r, 2))))))*(1 - cos(2 * acos(d / (2 * m_r)))));

		grad += temp;
	}
	//return grad / (2 * M_PI * m_overlaptwopoint.size());
	return 0;
}

double area::grad_line()
{
    double temp=2*(m_Points.size()-m_overlaptwopoint.size()/2)*
            (-1*m_linesize/(2*sqrt(m_linesize-pow(m_linesize,2)/(4*pow(m_r,2))))
             +m_linesize*m_r/(2*sqrt(pow(m_r,2)-pow(m_linesize/2,2)))
             +2*m_r*asin(m_linesize/(2*m_r)));
    return (-1*temp);
}

double area::grad_percent_line(){
	return 0;
}

double area::r() const
{
    return m_r;
}

void area::setR(double r)
{
    m_r = r;
    sum_C=M_PI*qPow(r,2)*m_Points.size();
    overlap();
}

double area::linesize() const
{
    return m_linesize;
}

void area::setLinesize(double linesize)
{
    m_linesize = linesize;
}



