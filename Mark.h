#pragma once
#include <string>
#include <vector>
using namespace std;
#define CIRCLE 0
#define SQUARE 1
#define DIAMOND 2
#define TRIANGLE 3
class Mark
{
public:
	Mark();
	~Mark();

	string getShape();
	void setShape(string shape);
	double getSize();
	void setSize(double size);

	void saveMark(const string filename, const Mark &mark);
	void saveMarks(const string filename, vector<Mark> &marks);

private:
	string _shape;
	double _size;
};

