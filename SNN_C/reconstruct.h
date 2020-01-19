/*###################################################### README #####################################################

# This file is used to leverage the generative property of a Spiking Neural Network. reconst_weights function is used
# for that purpose. Looking at the reconstructed images helps to analyse training process.

####################################################################################################################

*/
//Complete the code conversion!!!
#ifndef _RECONSTRUCTH_
#define _RECONSTRUCTH_
#include<stdio.h>
#include<stdlib.h>
#include "parameters.h"

int img[28][28];
int** reconst_weights(float** weights,int ind, long int num)
{	
       /*weights = np.array(weights)
	weights = np.reshape(weights, (pixel_x,pixel_x))*/ 
       float mod_weight[28][28];  
       int** img=(int**)malloc(sizeof(int)*28);
       for(int i=0;i<28; i++)
       { for(int j=0; j<28; j++)
          { mod_weight[i][j]=weights[ind][i*28+j];}
       }
        float x[]={w_min,w_max};
        float y[]={0.0,255.0};
        for(int i=0; i<28; i++)
        { for(int j=0; j<28; j++)
           { img[i][j] = (int)interp(mod_weight[i][j], x, y);}
         }
	
    
	printf("neuron_ %ld .png", num);
	return img;
}
//void main()
//{}
#endif
