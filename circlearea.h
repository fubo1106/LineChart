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
    double cal_overlapthree(const alglib::real_2d_array &threepoints);

    double cal_totalarea();

    double r() const;
    void setR(double r);

    double sumtotal() const;
    void setSumtotal(double sumtotal);

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
