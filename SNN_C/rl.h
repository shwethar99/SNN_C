#ifndef _RLH_
#define _RLH_

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include "parameters.h"

double rl(int t)
{	
	extern int tau_plus,tau_minus,STDP_offset;
        extern float A_minus,A_plus;
	if(t>0)
		return -A_plus*(exp(-(float)t/tau_plus)-STDP_offset);
	if(t<=0)
		return A_minus*(exp((float)t/tau_minus)-STDP_offset);

}

//STDP weight update rule
double update(double w,double del_w)
{      extern float sigma,w_min,mu;
       extern int w_max;
       define_parameters();
       if(del_w<0)
		return w + sigma*del_w*pow((w-abs(w_min)),mu);
	else if (del_w>0)
		return w + sigma*del_w*pow((w_max-w),mu);
}
#endif
