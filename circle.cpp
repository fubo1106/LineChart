#include "circle.h"

circle::circle()
{

}

circle::circle(const double &r, const QVector2D &mid)
{
    m_r=r;
    m_mid=mid;
}
double circle::r() const
{
    return m_r;
}

void circle::setR(double r)
{
    m_r = r;
}
QVector2D circle::mid() const
{
    return m_mid;
}

void circle::setMid(const QVector2D &mid)
{
    m_mid = mid;
}



