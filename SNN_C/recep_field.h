// This file consists of function that convolves an image with a receptive field so that input to the network is 
// close to the form perceived by our eyes. 
#ifndef _RECEPH_
#define _RECEPH_
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "parameters.h"
float pot[28][28];
float** rf(int inp[28][28])
{	float sca1 =  0.625;
	float sca2 =  0.125;
	float sca3 = -0.125; //Hyperpolarization (biology) - Wikipedia
	float sca4 = -0.5;
        
	//Receptive field kernel
	float w[][5] = {{	sca4 ,sca3 , sca2 ,sca3 ,sca4},
	 	{	sca3 ,sca2 , sca1 ,sca2 ,sca3},
	 	{	sca2 ,sca1 , 	1 ,sca1 ,sca2},
	 	{	sca3 ,sca2 , sca1 ,sca2 ,sca3},
	 	{	sca4 ,sca3 , sca2 ,sca3 ,sca4}};
        //allocating memory for pot[][] array.
        float **pot = (float **)malloc(28 * sizeof(float *)); 
        for (int i=0; i<28; i++) 
         pot[i] = (float *)malloc(28 * sizeof(float)); 
	
        for(int i=0; i<28; i++)
         for(int j=0;j<28;j++)
         pot[i][j]=0;

	int ran[] = {-2,-1,0,1,2};
	int ox = 2;
	int oy = 2;
        extern int pixel_x;
	//Convolution
	for(int i=0; i<28; i++)
	{	for(int j=0; j<28; j++)
		{	float summ = 0;
			for(int m=0; m<5; m++)
				for(int n=0; n<5; n++)
				if((i+ran[m])>=0&&(i+ran[m])<=pixel_x-1&&(j+ran[n])>=0&&(j+ran[n])<=pixel_x-1)
					summ = summ + w[ox+ran[m]][oy+ran[n]]*inp[i+ran[m]][j+ran[n]]/255;
			pot[i][j] = summ;
	}}return pot;
}
/*void main()
{}*/	
#endif	
