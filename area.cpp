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
	grad_C = 0;
	grad_L = 0;
	numMarker = px.size();
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
	m_overlapallthreepoint.clear();
	m_overlapthreepoint.clear();
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
            //printf("overlap 2: pair:%[%f %f],[%f %f]\n", result[0][0], result[0][1], result[1][0], result[1][1]);
            m_overlaptwopoint.push_back(result);
        }
        if(num==3)
        {
            real_2d_array result;
            kdtreequeryresultsx(m_kdtree,result);
			//printf("overlap 3: marker:[%f %f] pair:[%f %f] [%f %f]\n", p[0], p[1], result[0][0], result[0][1], result[1][0], result[1][1]);
			//printf("overlap 3: pair:[%f %f] [%f %f] [%f %f]\n", result(0, 0), result(0, 1), result(1, 0), result(1, 1), result(2, 0), result(2, 1));
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
            //qDebug()<<num<<" "<<i<<" Mark size is too large";
			//exit(0);
        }
    }
}

double area::getupperbound(const QVector<double> &px, const QVector<double> &py, double init){
	double ub = init;
	bool flag = false;
	if (m_Points.size() != px.size()){
		m_Points.clear();
		for (int i = 0; i != px.size(); i++)
		{
			m_Points.push_back(QVector2D(px[i], py[i]));
		}
	}

	while (!flag){
		for (int i = 0; i < m_Points.size(); i++){
			real_1d_array p;
			p.setlength(2);
			p[0] = m_Points[i].x();
			p[1] = m_Points[i].y();
			ae_int_t num = kdtreequeryrnn(m_kdtree, p, ub * 2);
			if (num > 2){
				flag = true;
				break;
			}	
		}	
		ub += 0.1;
	}
	return ub - 0.2;
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

double area::grad_Col(const double r, const double l){
	return 2 * r*asin(l / (2 * r));
}

double grad_percent_Col(const double r, const double l){
	double rSquare = pow(r, 2);
	double lSquare_over_4rSquare = pow(l, 2) / (4 * r*r);
	return -l*sqrt(1 - lSquare_over_4rSquare) / (M_PI*r*r);
}

double area::grad_overlaptwo(const real_2d_array &twopoints){
	QVector2D a(twopoints[0][0], twopoints[0][1]);
	QVector2D b(twopoints[1][0], twopoints[1][1]);

	double d = (b - a).length();
	double d_over_r = d / (2 * m_r);
	double sqrt_one_minus_square_d_over_r = sqrt(1 - pow(d_over_r, 2));
	double acosTheta = acos(d_over_r);
	double rSquare = pow(m_r, 2);
	double SquareMulSqrt = rSquare*sqrt(1 - pow(d, 2) / (4 * rSquare));

	//return rSquare*d*((1 - cos(2 * acosTheta)) / SquareMulSqrt) + 2 * m_r*(2 * acosTheta - sin(2 * acosTheta)); //total area
	return d*(1 - cos(2 * acosTheta)) / (M_PI*rSquare*sqrt_one_minus_square_d_over_r); //percent
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

double area::grad_overlapthree(const real_2d_array &twopoints){
	real_2d_array overlap12, overlap13;
	overlap12(0, 0) = twopoints(0, 0); overlap12(0, 1) = twopoints(0, 1);
	overlap12(1, 0) = twopoints(1, 0); overlap12(1, 1) = twopoints(1, 1);
	
	overlap13(0, 0) = twopoints(0, 0); overlap13(0, 1) = twopoints(0, 1);
	overlap13(1, 0) = twopoints(2, 0); overlap13(1, 1) = twopoints(2, 1);

	return grad_overlaptwo(overlap12) + grad_overlaptwo(overlap13);
}

double area::cal_overlapallthree(const real_2d_array &twopoints){
	
	double overlap = cal_overlapthree(twopoints);
	//common area
	double common_area = 0;
	QVector2D a(twopoints(0, 0), twopoints(0, 1));
	QVector2D b(twopoints(1, 0), twopoints(1, 1));
	QVector2D c(twopoints(2, 0), twopoints(2, 1));

	circle aa(m_r, a);
	circle bb(m_r, b);
	circle cc(m_r, c);
	circlearea area;

	QVector<QVector2D> insec_ab = area.intersection(aa, bb);
	QVector<QVector2D> insec_ac = area.intersection(aa, cc);
	QVector<QVector2D> insec_bc = area.intersection(bb, cc);

	if (area.inside(insec_ab[0], cc) && area.inside(insec_ab[1], cc)){
		common_area = area.cal_overlaptwo(aa, bb);
	}
	else if (area.inside(insec_ac[0], bb) && area.inside(insec_ac[1], bb)){
		common_area = area.cal_overlaptwo(aa, cc);
	}
	else if (area.inside(insec_bc[0], aa) && area.inside(insec_bc[1], aa)){
		common_area = area.cal_overlaptwo(bb, cc);
	}
	else{
		QVector<QVector2D> points = area.intersection(aa, bb, cc); //see the return point format
		double angle1 = cal_Angle(points[1], aa.mid(), points[2]); //angle in aa
		double angle2 = cal_Angle(points[0], bb.mid(), points[2]); //in bb
		double angle3 = cal_Angle(points[0], cc.mid(), points[1]); //in cc

		double triangle_area = cal_area_from_three_vetex(points[0], points[1], points[2]);
		double sector1 = angle1 / 2 * m_r*m_r - cal_area_from_three_vetex(aa.mid(), points[1], points[2]);
		double sector2 = angle2 / 2 * m_r*m_r - cal_area_from_three_vetex(bb.mid(), points[0], points[2]);
		double sector3 = angle3 / 2 * m_r*m_r - cal_area_from_three_vetex(cc.mid(), points[0], points[1]);

		common_area = triangle_area + sector1 + sector2 + sector3;
	}
	return overlap - common_area;
}

double area::grad_overlapallthree(const real_2d_array &points){
	double grad_all = grad_overlapthree(points);
	double grad_overlap;

	QVector2D a(points(0, 0), points(0, 1));
	QVector2D b(points(1, 0), points(1, 1));
	QVector2D c(points(2, 0), points(2, 1));

	circle aa(m_r, a);
	circle bb(m_r, b);
	circle cc(m_r, c);
	circlearea area;

	QVector<QVector2D> insec_ab = area.intersection(aa, bb);
	QVector<QVector2D> insec_ac = area.intersection(aa, cc);
	QVector<QVector2D> insec_bc = area.intersection(bb, cc);

	if (area.inside(insec_ab[0], cc) && area.inside(insec_ab[1], cc)){
		grad_overlap = area.grad_overlaptwo(aa, bb);
	}
	else if (area.inside(insec_ac[0], bb) && area.inside(insec_ac[1], bb)){
		grad_overlap = area.grad_overlaptwo(aa, cc);
	}
	else if (area.inside(insec_bc[0], aa) && area.inside(insec_bc[1], aa)){
		grad_overlap = area.grad_overlaptwo(bb, cc);
	}
	else{//look for the fomulation in word file
		int root1, root2, root3;
		QVector<QVector2D> points = area.intersection(aa, bb, cc, root1, root2, root3); //see the return point format
		
		//get intersection points pa,pb,pc;
		QVector2D pa, pb, pc; 

		QVector2D p1, p2, p3;
		double D, del, a, b, c, d;
		p1 = aa.mid();
		p2 = bb.mid();
		p3 = cc.mid();

		double c1x = p1.x(); double c1y = p1.y();
		double c2x = p2.x(); double c2y = p2.y();
		double c3x = p3.x(); double c3y = p3.y();

		D = (p1 - p2).length();
		del = 0.25*D*sqrt(4 * m_r*m_r - D*D);

		grad_overlap = area.grad_common(aa, bb, cc, root1, root2, root3);
	}
	return grad_all - grad_overlap;
}

double area::cal_totalcirclearea()
{
    double m_sumtwo=0;
    overlap();
    for(int i=0;i<m_overlaptwopoint.size()-1;i=i+2)
    {
		if (sametwo(m_overlaptwopoint[i], m_overlaptwopoint[i + 1])){
			m_sumtwo += cal_overlaptwo(m_overlaptwopoint[i]);
		}   
		else{
			m_sumtwo += cal_overlaptwo(m_overlaptwopoint[i]) + cal_overlaptwo(m_overlaptwopoint[i + 1]);
		}        
    }
	vis_C = sum_C - m_sumtwo;
    return vis_C;
}

double area::cal_percentcircleare(){
	double m_sumtwo = 0;
	double m_sumline = 0;
	double denominator = m_Points.size()*M_PI*m_r*m_r;
	grad_C = 0;
	overlap();
	for (int i = 0; i<m_overlaptwopoint.size(); i=i+2)
	{	/*when 2 circles overlap, accurate overlap area should not be the sum of overlap(circle, circle)+overlap(line, circle)
	it should also minus the overlap of 2 circles and the line, here we just use the sum of 2 areas for a bigger penalty when 2 circle has overlap*/
		//printf("%d: %s\n", i, m_overlaptwopoint[i].tostring(1).c_str());
		m_sumtwo += cal_overlaptwo(m_overlaptwopoint[i]);
		grad_C += grad_overlaptwo(m_overlaptwopoint[i]);
	}
	for (int i = 0; i < m_overlapthreepoint.size(); i++){
		m_sumtwo += cal_overlapthree(m_overlapthreepoint[i]);
		grad_C += grad_overlapthree(m_overlapthreepoint[i]);
	}
	for (int i = 0; i < m_overlapallthreepoint.size(); i++){
		m_sumtwo += cal_overlapallthree(m_overlapallthreepoint[i]);
		grad_C += grad_overlapallthree(m_overlapallthreepoint[i]);
	}
	
#if 0 //plus the line's overlap: previous code, calculating circle area = circlearea-overlap(cicle,circle)-overlap(line,circle);
	double circle_ratio = 0.1;
	double line_ratio = 5;
	m_sumline = m_Points.size() * cal_Col(m_r, m_linesize);
	m_sumtwo = circle_ratio*m_sumtwo + line_ratio*m_sumline;
	grad_C = 2 * (circle_ratio*grad_C + line_ratio*m_Points.size()*grad_percent_Col(m_r, m_linesize));
	return (m_sumtwo * 2) / denominator;
#endif
	/*only considering the overlap area of circles,(line does not have a overlap for circles),
	this is for the new optimization framework: maxmize the visibility of marker+line*/
#if 1
	double thresh = sum_L;//line=1437 circle=79.4
	/*grad_C = (grad_C * 2 - 2 * numMarker*M_PI) / thresh;
	return (m_sumtwo * 2 - numMarker*M_PI*m_r*m_r) / thresh;*/
	
	//use n*pi*r, avoid the r^2's big influence
	grad_C = (grad_C * 2 - numMarker*M_PI) / thresh;
	return (m_sumtwo * 2 - numMarker*M_PI*m_r) / thresh;
#endif
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
	overlap_per_L = 0;
	grad_L = 0;
	circlearea area_c;

	for (int i = 0; i < m_Points.size()-1; i++){
		circle c1(m_r, m_Points[i]);
		circle c2(m_r, m_Points[i + 1]);
		if (area_c.isOverlap(c1, c2)){
			if (m_r >= m_linesize / 2){//whole line is overlapped: Overlap=line_width*line_length
				double x1 = m_Points[i].x();
				double x2 = m_Points[i + 1].x();
				double y1 = m_Points[i].y();
				double y2 = m_Points[i + 1].y();
				overlap_per_L += m_linesize*sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
				//grad_C += 0;
			}
			else{//Overlap = 2*overlap(circle,line)-overlap(circle1,circle2)
				overlap_per_L = 2 * cal_Col(m_r, m_linesize) - area_c.cal_overlaptwo(c1, c2);
				grad_L += 2 * grad_Col(m_r, m_linesize) - area_c.grad_overlaptwo(c1, c2);
			}
		}
		else{//2 circles have no ovelap: Overlap = 2*overlap(circle,line)
			overlap_per_L += 2 * cal_Col(m_r, m_linesize);
			grad_L += 2 * grad_Col(m_r, m_linesize);
		}
	}

	//overlap_per_L = (m_length*m_linesize - m_Col - cal_coverline() + m_Tri) / sum_L;
	grad_L /= sum_L;
	//return vis_per_L;
	return overlap_per_L / sum_L;
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



