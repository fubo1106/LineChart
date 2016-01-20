#ifndef LINE_H
#define LINE_H
#include <QVector2D>

class line
{
public:
    line();//y=ax+b
    line(const QVector2D &ps,const QVector2D &pe);//two points build line
    line(const QVector2D &p,const double &k);//point and slope build line

    double getA() const;
    void setA(double value);

    double getB() const;
    void setB(double value);

private:
    double a;
    double b;
};

#endif // LINE_H
