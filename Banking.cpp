#include "Banking.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <math.h>
#include <nlopt.h>
#include <stdio.h>


Banking::Banking(void)
{
}


Banking::~Banking(void)
{
}

vector<float> Banking::calcSlope(vector<pair<float,float>> dataxy)
{
	vector<float> slopes;
	for(int i=0;i<dataxy.size()-1;i++)
	{
		float deltax = dataxy[i+1].first - dataxy[i].first;
		float deltay = dataxy[i+1].second - dataxy[i].second;
		float slope = deltay / deltax;
		slopes.push_back(slope);
	}
	return slopes;
}

double EOfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *slopes = d->slopes;
	float derivative = 0;
	float alpahSquare = alpha[0]*alpha[0];
	float funcv = 0;
	for(int i=0;i<num;i++)
	{
		float phySlope = fabs(slopes[i]*alpha[0]);
		float funiv = atan(phySlope) - PI/4;
		float ideriv = fabs(slopes[i])/(1 + alpahSquare*slopes[i]*slopes[i]);
		if(funiv<0)
			derivative += -ideriv;
		else
			derivative += ideriv;
		funcv += fabs(funiv);
	}
	grad[0] = derivative/num;
	return fabs(funcv);
}
double AOfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *slopes = d->slopes;

	float funcv = 0;
	for(int i=0;i<num;i++)
	{
		float phySlope = fabs(slopes[i]*alpha[0]);
		funcv += atan(phySlope);
	}
	funcv = (funcv /num - PI/4);

	if (grad) {
		float derivative = 0;
		float alpahSquare = alpha[0]*alpha[0];
		for(int i=0;i<num;i++)
		{
			float ideriv = fabs(slopes[i])/(1 + alpahSquare*slopes[i]*slopes[i]);
			derivative += ideriv;
		}
		if(funcv>=0)
			grad[0] = derivative/num;
		else
			grad[0] = -derivative/num;
	}
	return fabs(funcv);
}

double AWOfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *datax = d->datax;
	double *slopes = d->slopes;


	float funcv = 0;
	float oriSum = 0;
	float lenSum = 0;
	for(int i=0;i<num;i++)
	{
		float phySlope = fabs(slopes[i]*alpha[0]);
		float lenInS = sqrt(1+phySlope*phySlope)*datax[i];
		oriSum +=  atan(phySlope) * lenInS;
		lenSum += lenInS;
	}
	funcv = oriSum / lenSum - PI/4;


	if (grad) {


		float derivativeLen = 0;
		float alpahSquare = alpha[0]*alpha[0];
		float weightSum = 0;
		float lengthSum = 0;
		for(int i=0;i<num;i++)
		{
			float xlen = datax[i];
			float slopeSquare = slopes[i]*slopes[i];
			float iderivLen = xlen * (slopeSquare*alpha[0])*(1/sqrt(1 + slopeSquare*alpahSquare));
			derivativeLen += iderivLen;

		}

		float derivativeWsum = 0;
		for(int i=0;i<num;i++)
		{
			float phySlope = fabs(slopes[i]*alpha[0]);
			float xlen = datax[i];

			float slopeSquare = slopes[i]*slopes[i];
			float iderivLen = xlen * (slopeSquare*alpha[0])*(1/sqrt(1 + slopeSquare*alpahSquare));

			float iderivOri = fabs(slopes[i])/(1 + alpahSquare*slopeSquare);
			float lenInS = sqrt(1+phySlope*phySlope)*xlen;
			derivativeWsum +=  atan(phySlope) * iderivLen + lenInS*iderivOri;
			weightSum +=  atan(phySlope) * lenInS;
			lengthSum += lenInS;
		}

		float denominator =  lengthSum * lengthSum;
		float derivative = (weightSum*derivativeLen + lengthSum*derivativeWsum)/denominator;

		if(funcv>=0)
			grad[0] = derivative/num;
		else
			grad[0] = -derivative/num;
	}


	return fabs(funcv);
}
double EWOfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *datax = d->datax;
	double *slopes = d->slopes;


	float funcv = 0;


	if (grad) {


		float derivativeLen = 0;
		float alpahSquare = alpha[0]*alpha[0];
		float weightSum = 0;
		float lengthSum = 0;
		for(int i=0;i<num;i++)
		{
			float xlen = datax[i];
			float slopeSquare = slopes[i]*slopes[i];
			float iderivLen = xlen * (slopeSquare*alpha[0])*(1/sqrt(1 + slopeSquare*alpahSquare));
			derivativeLen += iderivLen;

		}

		float derivativeWsum = 0;
		for(int i=0;i<num;i++)
		{
			float phySlope = fabs(slopes[i]*alpha[0]);
			float xlen = datax[i];
			float ifunc = atan(phySlope)-PI/4;
			float slopeSquare = slopes[i]*slopes[i];
			float iderivLen = xlen * (slopeSquare*alpha[0])*(1/sqrt(1 + slopeSquare*alpahSquare));

			float iderivOri = fabs(slopes[i])/(1 + alpahSquare*slopeSquare);
			if(ifunc<0)
				iderivOri = -iderivOri;
			float lenInS = sqrt(1+phySlope*phySlope)*xlen;
			derivativeWsum +=  fabs(ifunc) * iderivLen + lenInS*iderivOri;
			weightSum +=  fabs(ifunc) * lenInS;
			lengthSum += lenInS;
		}

		float denominator =  lengthSum * lengthSum;
		float derivative = (weightSum*derivativeLen + lengthSum*derivativeWsum)/denominator;
		funcv = weightSum / lengthSum;
	
		grad[0] = derivative/num;
		
	}


	return funcv;
}

double GORfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *slopes = d->slopes;


	float funcGOR = 0;
	if (grad) {
		float derivativeWsum = 0;
		float alpahSquare = alpha[0] * alpha[0];
		for(int i=0;i<num;i++)
		{
			float thetai = atan(slopes[i]*alpha[0]);
			float derivativeI =  slopes[i]/(1 + alpahSquare*slopes[i]*slopes[i]);

			for(int j=i+1;j<num;j++)
			{
				float thetaj = atan(slopes[j]*alpha[0]);
				float derivativeJ =  slopes[j]/(1 + alpahSquare*slopes[j]*slopes[j]);

				float thetadiff = fabs(thetai - thetaj);
				if(thetadiff>PI/2)
				{
					thetadiff = PI - thetadiff;
					derivativeWsum +=  2 * thetadiff * (derivativeJ - derivativeI);
				}
				else
					derivativeWsum +=  2 * thetadiff * (derivativeI - derivativeJ);
				funcGOR += thetadiff*thetadiff;
			}

		}
		grad[0]  = derivativeWsum;
	}


	return -funcGOR;
}

double LORfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *slopes = d->slopes;


	float funcLOR = 0;
	if (grad) {
		float derivativeWsum = 0;
		float alpahSquare = alpha[0] * alpha[0];
		for(int i=0;i<num-1;i++)
		{
			float thetai = atan(slopes[i]*alpha[0]);
			float derivativeI =  slopes[i]/(1 + alpahSquare*slopes[i]*slopes[i]);

			//for(int j=0;j<num;j++)
			int j = i +1;
			{
				float thetaj = atan(slopes[j]*alpha[0]);
				float derivativeJ =  slopes[j]/(1 + alpahSquare*slopes[j]*slopes[j]);

				float thetadiff = fabs(thetai - thetaj);
				if(thetadiff>PI/2)
				{
					thetadiff = PI - thetadiff;
					derivativeWsum +=  2 * thetadiff * (derivativeJ - derivativeI);
				}
				else
					derivativeWsum +=  2 * thetadiff * (derivativeI - derivativeJ);
				funcLOR += thetadiff*thetadiff;
			}

		}
		grad[0]  = derivativeWsum;
	}


	return -funcLOR;
}
double MLCfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *slopes = d->slopes;


	float funcLOR = 0;
	if (grad) {
		float derivativeWsum = 0;
		float alpahSquare = alpha[0] * alpha[0];
		for(int i=0;i<num-1;i++)
		{
			float thetai = atan(slopes[i]*alpha[0]);
			float derivativeI =  slopes[i]/(1 + alpahSquare*slopes[i]*slopes[i]);

			//for(int j=0;j<num;j++)
			int j = i +1;
			{
				float thetaj = atan(slopes[j]*alpha[0]);
				float derivativeJ =  slopes[j]/(1 + alpahSquare*slopes[j]*slopes[j]);

				float thetadiff = fabs(thetai - thetaj);
				if(thetadiff>PI/2)
				{
					thetadiff = PI - thetadiff;
					derivativeWsum +=    (derivativeJ - derivativeI);
				}
				else
					derivativeWsum +=   (derivativeI - derivativeJ);
				funcLOR += thetadiff;
			}

		}
		grad[0]  = derivativeWsum;
	}


	return -funcLOR;
}
double ALfunc(unsigned n, const double *alpha, double *grad, void *data)
{
	ar_data_slopes *d = (ar_data_slopes *) data;
	int num = d->num;
	double *datax = d->datax;
	double *slopes = d->slopes;



	float lenSum = 0;

	for(int i=0;i<num;i++)
	{
		float lenInS = sqrt(1/alpha[0]+alpha[0]*slopes[i]*slopes[i])*datax[i];
		lenSum += lenInS;
	}



	if (grad) {

		float derivative = 0;

		float alpahSquare = pow(alpha[0],2.0);

		for(int i=0;i<num;i++)
		{
			float xlen = datax[i];
			float slopeSquare = slopes[i]*slopes[i];
			float iderivLen = xlen * (slopeSquare-1/alpahSquare)*(1/sqrt(1/alpha[0] + alpha[0]*slopeSquare));
			derivative += iderivLen;
		}
		grad[0] = derivative*0.5;
	}


	return lenSum;
}
float Banking::run(vector<pair<float,float>> data,int method)
{
	vector<float> slopes = calcSlope(data);
	if(method==MS||method==AS)
	{
		if(method==MS)
			return medianSlope(slopes);
		else
			return averageSlope(slopes);
	}
	ar_data_slopes *ardata = new  ar_data_slopes;
	ardata->num = slopes.size();
	ardata->slopes = new double[slopes.size()];
	ardata->datax = new double[slopes.size()];
	for(int i=0;i<slopes.size();i++)
	{
		ardata->slopes[i] = slopes[i];
		ardata->datax[i] = fabs(data[i+1].first - data[i].first);
	}

	double lb =  0 ;
	nlopt_opt opt;
	opt = nlopt_create(NLOPT_LD_MMA, 1); 
	nlopt_set_lower_bounds(opt, &lb);
	switch (method)
	{
	case AO:
		nlopt_set_min_objective(opt, AOfunc, (void *)ardata);
		break;
	case AWO:
		nlopt_set_min_objective(opt, AWOfunc, (void *)ardata);
		break;
	case GOR:
		nlopt_set_min_objective(opt, GORfunc, (void *)ardata);
		break;
	case LOR:
		nlopt_set_min_objective(opt, LORfunc, (void *)ardata);
		break;
	case AL:
		nlopt_set_min_objective(opt, ALfunc, (void *)ardata);
		break;
	case EO:
		nlopt_set_min_objective(opt, EOfunc, (void *)ardata);
		break;
	case EWO:
		nlopt_set_min_objective(opt, EWOfunc, (void *)ardata);
		break;
	case MLC:
		nlopt_set_min_objective(opt, MLCfunc, (void *)ardata);
		break;
	default:
		break;
	}
	nlopt_set_xtol_rel(opt, 1e-8);
	double aspectRatio = 150.90005;  
	double minf; 

	if (nlopt_optimize(opt, &aspectRatio, &minf) < 0) {
		printf("nlopt failed!\n");
	}
	else {
		printf("found minimum at f(%g) = %0.10g\n", aspectRatio, minf);
	}

	
	nlopt_destroy(opt);
	delete ardata;
	return aspectRatio;
}

float Banking::medianSlope(vector<float> slopes)
{
	float aspect = 0;
	vector<float> absslopes;
	for(int i=0;i<slopes.size();i++)
		absslopes.push_back(fabs(slopes[i]));
	std::sort(absslopes.begin(), absslopes.end());
	float fMedian = 0.0;
	int iSize = slopes.size();
    if ((iSize % 2) == 0) {
        fMedian = (absslopes[iSize/2] + absslopes[(iSize/2) - 1])/2.0;
    } else {
        fMedian = absslopes[iSize/2];
    }
	aspect = fMedian;
	return aspect;
}


float Banking::averageSlope(vector<float> slopes)
{
	float aspect = 0;
	vector<float> absslopes;
	for(int i=0;i<slopes.size();i++)
		absslopes.push_back(fabs(slopes[i]));
	float sumslope = std::accumulate(absslopes.begin(),absslopes.end(),0);
	aspect = sumslope / (float)slopes.size();
	return aspect;
}








