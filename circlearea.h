#ifndef CIRCLEAREA_H
#define CIRCLEAREA_H
#include "circle.h"
#include "alglib3.9.0/src/alglibmisc.h"
#include <QVector>
#include <QVector2D>

class circlearea
{
public:
    circlearea();
    circlearea(const QVector<double> &px,const QVector<double> &py,const double &r);
    void buildkdtree();
    void overlap();
    bool sametwo(const alglib::real_2d_array &p,const alglib::real_2d_array &q);
    double cal_overlaptwo(const alglib::real_2d_array &twopoints);
	double cal_overlaptwo(circle& c1, circle& c2);
    double cal_overlapthree(const alglib::real_2d_array &threepoints);

    double cal_totalarea();

    double r() const;
    void setR(double r);

    double sumtotal() const;
    void setSumtotal(double sumtotal);

public:
	double grad_overlaptwo(circle& c1, circle& c2);
	double grad_common(circle& c1, circle& c2, circle& c3, int root1, int root2, int root3);

public:
	bool inside(QVector2D& p, circle& c);
	bool isOverlap(circle& c1, circle& c2);
	QVector<QVector2D> intersection(circle& c1, circle& c2);
	QVector<QVector2D> intersection(circle& c1, circle& c2, circle& c3, int root1 = 0, int root2 = 0, int root3 = 0);
	QVector<std::pair<QVector2D, QVector2D>> intersectPairs(circle& c1, circle& c2, circle& c3);

private:
    QVector<QVector2D> m_Points;
    alglib::real_2d_array alg_Points;
    alglib::kdtree m_kdtree;
    QVector<alglib::real_2d_array> m_overlaptwo;
    QVector<alglib::real_2d_array> m_overlapthree;
    double m_sumtwo;
    double m_sumthree;
    double m_sumtotal;
    double m_realtotal;
    double m_r;
};

#endif // CIRCLEAREA_H
