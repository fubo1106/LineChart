#include "sector.h"

sector::sector()
{

}

sector::sector(const double &angle, const double &r)
{
    m_area=r*r*angle/2;
}
double sector::area() const
{
    return m_area;
}

void sector::setArea(double area)
{
    m_area = area;
}


