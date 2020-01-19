#ifndef _PARAMH_
#define _PARAMH_
#include<stdio.h>
int T=350;
int t_back=-5;
int epoch = 1;
char training_set_path[]= "./mnist_png/training/";
int pixel_x = 28;
float Prest = -70.0;
int m = 784;
int n =  6 ; // number of inner neurons- 2 images per neuron and 3 neurons.

float Pinhibit = -100.0 ;
float Pth = -55.0;
float Phyperpolarisation = -90.0;
float Pdrop = 0.8;
float Pthdrop = 0.4;
float synapse_init[6][784];

float w_min=0.00001;
int w_max;

int STDP_offset = 0;
float sigma = 0.01; 
float A_plus = 0.8 ; 
float A_minus = 0.8 ;
int tau_plus = 5;
int tau_minus = 5;
float mu=0.9;
float interp(float xp,float* x, float* y)
{
 
 float yp;
 
 /* Calculation */
 yp = y[0] + ((y[1]-y[0])/(x[1]-x[0])) * (xp - x[0]);
 
 return yp;
 
}
void define_parameters()
{ for(int i=0; i<n; i++){
	for(int j=0; j<m; j++){
		synapse_init[i][j] = 1;}}
  int max=0;
  for(int i=0; i<n; i++){
	for(int j=0; j<m; j++){
		if(synapse_init[i][j]>max)
                max=synapse_init[i][j];}}
  w_max=max;
}
#endif
