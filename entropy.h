#ifndef ENTROPY_H
#define ENTROPY_H
#include <QVector>
#include <QVector2D>
class entropy
{
public:
	entropy(const QVector<double> &px, const QVector<double> &py, int radius = 5);//10 20
	~entropy();

	void sample_by_entropy(QVector<int>& indexes, int num = 100);//50 100
	QVector<double> calc_Entropy();

private:
	double calc_Entropy_For_one(int index, const QVector<int>& data);
	template <typename T>
	QVector<int> sort_indexes(const QVector<T> &v);

	QVector<double> m_DataX;
	QVector<double> m_DataY;
	QVector<int> m_normalizedY; //normalize data into [0 1 2 3 4 5 6 7 8 9];

	int _radius;
	int _num;
	double _maxVal;
	double _minVal;
	int _labelNum;
	//double _resolution;
};
#endif
