
//Check and clear warnings 

//This file generates rate based spike train from the potential map.
#ifndef _SPIKEH_
#define _SPIKEH_
#include<stdio.h>
#include "parameters.h"
#include<math.h>
#include<string.h>
#include<stdlib.h>
float train[784][351];


float min(float** x)
{ float min=0;
  for(int i=0; i<pixel_x; i++)
   { for(int j=0; j<pixel_x; j++)
     {if(x[i][j]<min)
       min=x[i][j];}
    }
  return min;
}
float max(float** x)
{ float max=0;
  for(int i=0; i<pixel_x; i++)
   { for(int j=0; j<pixel_x; j++)
     {if(x[i][j]>max)
       max=x[i][j];}
    }
  return max;
}
float** encode(float** pot){
        int i=0;
        int count=0;
	//initializing spike train
	float freq;
        
               float** train = (float **)malloc(784 * sizeof(float *)); 
               for (int i=0; i<784; i++) 
                   train[i] = (float *)malloc(351 * sizeof(float)); 
        extern int pixel_x,T;
	for(int l=0; l<pixel_x;l++){
		for(int m=0;m<pixel_x;m++){
		
			float temp[351]; // = np.zeros([(T+1),])
                        for(i=0; i<351;i++)
                           temp[i]=0;
                        

                        float pote[]={min(pot),max(pot)};
                        float yval[]={1.0,50.0};
			//calculating firing rate proportional to the membrane potential
			freq = interp(pot[l][m], pote, yval);

				
			int time_period = ceil(350/freq);
                          //int time_period=10; //for debugging
			//generating spikes according to the firing rate
			int time_of_spike = time_period;
			if(pot[l][m]>0){                                //Check!!
				while(time_of_spike<(351)){
					temp[(int)time_of_spike] = 1;
					time_of_spike+= time_period; }}
                        for(i=0; i<351; i++)
                          train[count][i]=temp[i];
                        count++;
			
        }}

	return train;

}


/*void main()
{}*/
#endif
