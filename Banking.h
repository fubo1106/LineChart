#pragma once
#include<vector>
using namespace std;
#define MS 1
#define AO 2
#define AWO 3
#define GOR 4
#define LOR 5
#define AS 6
#define AL 7
#define EO 8
#define EWO 9
#define MLC 10
#define PI 3.1415926
#define EPS 0.00001


typedef struct {
	int num;
	double *slopes;
}ar_slopes;

typedef struct {
	int num;
	double *datax;
	double *slopes;
}ar_data_slopes;

class Banking
{
public:
	Banking(void);
	~Banking(void);
public:
	float run(vector<pair<float,float>> data,int method);
private:
	vector<float> calcSlope(vector<pair<float,float>> data);



private:
	float medianSlope(vector<float> slope);//medianSlope

	float averageSlope(vector<float> slope);//average slope
};

