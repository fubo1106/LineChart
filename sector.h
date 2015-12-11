#ifndef SECTOR_H
#define SECTOR_H


class sector
{
public:
    sector();
    sector(const double &angle,const double &r);

    double area() const;
    void setArea(double area);

private:
    double m_angle;
    double m_r;
    double m_area;
};

#endif // SECTOR_H
