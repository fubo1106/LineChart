#include "Mark.h"
#include <fstream>

Mark::Mark()
{
}


Mark::~Mark()
{
}

string Mark::getShape(){
	return _shape;
}

void Mark::setShape(string shape){
	_shape = shape;
}

double Mark::getSize(){
	return _size;
}

void Mark::setSize(double size){
	_size = size;
}

void saveMark(const string filename, const Mark &mark){

}

void saveMarks(const string filename, vector<Mark>& marks){
	ofstream ofs(filename);
	for (int i = 0; i < marks.size(); i++){
		ofs << marks[i].getShape() << " " << marks[i].getSize() << endl;
	}
}

