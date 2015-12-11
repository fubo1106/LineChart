#ifndef CIRCLE_H
#define CIRCLE_H
#include <QVector2D>

class circle
{
public:
    circle();
    circle(const double &r,const QVector2D &mid);

    double r() const;
    void setR(double r);

    QVector2D mid() const;
    void setMid(const QVector2D &mid);

private:
    double m_r;
    QVector2D m_mid;

};

#endif // CIRCLE_H
