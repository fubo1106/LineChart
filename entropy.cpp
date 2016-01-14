#include "entropy.h"
using namespace std;
entropy::entropy(const QVector<double> &px, const QVector<double> &py, int radius)
{
	m_DataX = px;
	m_DataY = py;
	_radius = radius;
	_num = px.size();
	_labelNum = _num / 10;

	double temp_min = py[0];
	double temp_max = py[0];
	for (int i = 1; i < py.size(); i++){
		if (py[i] > temp_max){
			temp_max = py[i];
		}
		if (py[i] < temp_min){
			temp_min = py[i];
		}
	}
	_maxVal = temp_max; 
	_minVal = temp_min;
	//_resolution = (_maxVal - _minVal) / _num;
	for (int i = 0; i < py.size(); i++){
		int val = floor((py[i] - _minVal) / (_maxVal - _minVal) * (_labelNum - 1));
		m_normalizedY.push_back(val);
	}
}

entropy::~entropy()
{
}

void entropy::sample_by_entropy(QVector<int>& indexes, int num){
	indexes.clear();
	if (_num <= num){
		for (int i = 0; i < _num; i++){
			indexes.push_back(i);
		}
	}
	else{
		QVector<double> entro = calc_Entropy();
		QVector<int> sequence = sort_indexes(entro);
		indexes.clear();
		for (int i = 0; i < num; i++){
			indexes.push_back(sequence[i]);
		}
		qSort(indexes);
	}
	
}

QVector<double> entropy::calc_Entropy(){
	QVector<double> entropy;
	entropy.resize(m_normalizedY.size());

	for (int i = 0; i < m_normalizedY.size(); i++){
		double ent = calc_Entropy_For_one(i, m_normalizedY);
		entropy[i] = ent;
		//printf("%d's entropy\n", i);
	}
	return entropy;
}

double entropy::calc_Entropy_For_one(int index, const QVector<int>& data){
	int* hashTable = new int[_labelNum];
	for (int i = 0; i < _labelNum; i++){
		hashTable[i] = 0;
	}

	int count = 0;
	double entropy = 0;
	for (int i = index - _radius; i <= index + _radius; i++){
		if (i < 0 || i >= data.size()){ //out of index range
			continue;
		}
		else{
			hashTable[data[i]]++;
			count++;
		}
	}
	for (int i = 0; i < _labelNum; i++){
		double prob = hashTable[i] / (double)count;
		if (prob != 0)
			entropy -= prob * log2(prob);
	}
	delete[]hashTable;
	return entropy;
}

template <typename T>//size_t => int
QVector<int> entropy::sort_indexes(const QVector<T> &v) {

	// initialize original index locations
	QVector<int> idx(v.size());
	for (int i = 0; i != idx.size(); ++i) idx[i] = i;

	// descent sort indexes based on comparing values in v
	sort(idx.begin(), idx.end(),
		[&v](int i1, int i2) {return v[i1] > v[i2]; });

	return idx;
}

