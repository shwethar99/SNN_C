/*##################### README ###################################################
# This file executes the classification algorithm over input testing images.
# Winner neurons inhibit other neurons by a phenomenon called Lateral inhibition
# Spike for each output neuron at each time stamp is monitored.
################################################################################*/


//header file inclusion
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h>
#include "neuron.h"
#include "recep_field.h"
#include <time.h>
#include "spike_train.h"
#include "rl.h"
#include "parameters.h"
#include "values.h"
#include "image.h" 
#define MAXBUFLEN 1000000

//time series 
int time_of_learning[350];

// creating the hidden layer of neurons
struct neuron output_layer[6];

//synapse matrix
float synapse[6][784];

//importing value of weight_matrix from values.h file
extern float weight_matrix[6][784];

//importing image intensity values from image.h file
extern int image_intensity[6][784];

//importing filenames from values.h file
extern char filenames[6][50];

//a 28x28 matrix for pixel values of image
int img[28][28];

//method to return the index of max element in a float array 
int argmax(float* arr, int n)
{int max=0,i;
 for(i=1;i<n;i++)
  if(arr[i]>arr[max])
    max=i;
  return max;}

//performing dot operation on 2 arrays
float dot(float synapse[6][784],int n,float** train,int m)
{float sum=0;
for(int i=0; i<784;i++)
 sum+=synapse[n][i]*train[i][m];
return sum;
 }

//main() function definition starts here.
void main()
{
int imgno;
int i,j;
int c_index=0;
//initializing synapse matrix.
for(i=0; i<6; i++){
for(j=0; j<784;j++){
synapse[i][j]=0;}}


for(i=1; i<=350; i++)
time_of_learning[i]=i;


//copy contents of csv into matrices
int label_matrix[6]={2,2,1,1,0,0};

int m = 784;//Number of neurons in first layer //Number of cols in weight_matrix
int n = 6;//Number of neurons in second layer //number of rows in weight_matrix
for(i=0;i<6;i++)
{ 
       
	initial(&output_layer[i]);        //initializing potentials of output neurons.
        for(j=0;j<784;j++)
	synapse[i][j] = weight_matrix[i][j];
}

//used at the end
int predicted_class[6];
int actual_class[6];

//count of prediction in each class
int prediction_count[3][3];

//initializing prediction count to be 0:
for(i=0; i<3;i++)
{for(j=0;j<3;j++)
  prediction_count[i][j]=0;}


for(imgno=0; imgno<6; imgno++)
{            
             printf("%s\n",filenames[imgno]);
             clock_t start=clock();    //Record starting time
             float count_spikes[]={0,0,0,0,0,0}; //initializing spike count to zero.
             
            //int k=0;
            //converting the image intensity to a 28x28 matrix
            
            for(int i=0; i<6; i++)
             initial(&output_layer[i]); 
            for(int i=0; i<28; i++)
             { for(int j=0; j<28; j++)
                {img[i][j]=image_intensity[imgno][i*28+j];
                 }
              }
             
             
             
               float** train1 = (float **)malloc(784 * sizeof(float *)); 
               for (int i=0; i<784; i++) 
                   train1[i] = (float *)malloc(351 * sizeof(float));
 
                
                float** pot=rf(img);
                
              train1=encode(pot);
             
             

             int f_spike=0;
             float active_pot[]={0,0,0,0,0,0}; //active potential
             int winner=0;

             for(int i=0; i<351; i++)
            {  for(int j=0; j<6; j++)
               {  
                  if(output_layer[j].t_rest<time_of_learning[i])
                  {  
                     output_layer[j].P+=dot(synapse,j,train1,time_of_learning[i]);
                     
                     if(output_layer[j].P>Prest)
                      {output_layer[j].P-=Pdrop;  
                       
                       }
                   }
                   active_pot[j]=output_layer[j].P; 
                   
                }


                for(int j=0; j<6;j++) 
                { if(j==argmax(active_pot,6))
                  { if(active_pot[j]>output_layer[j].Pth)
                    { 
                      count_spikes[j]+=1;
                      output_layer[j].Pth-=1;
                      hyperpolarisation(&output_layer[j],time_of_learning[i]);

                      for(int p=0;p<6;p++)
                      {
                        if(p!=j)
                       {if(output_layer[p].P>output_layer[p].Pth)
                          inhibit(&output_layer[p],time_of_learning[i]);  //inhibit when above threshold
                        }
                        
                       }
                       
                      break;
                     }
                   }
                       
                }
             }
             //prediction
             printf("\n%s\n",filenames[imgno]);
             printf("\nPredicted_class = %d\n",label_matrix[argmax(count_spikes,6)]);
             printf("\nActual class = %d\n",imgno/2);
              
             //time for inference
             clock_t time=clock()-start;
             double t = ((double)time)/CLOCKS_PER_SEC;
             printf("\nTime for inference= %f seconds\n",t);
             for(int i=0; i<6; i++)
                 printf("%f\t",count_spikes[i]);
             printf("\nArgmax: %d\n",argmax(count_spikes,6)); 
             //prediction count matrix-aids in finding accuracy
             predicted_class[c_index]=label_matrix[argmax(count_spikes,6)];
             actual_class[c_index]=imgno/2;
	     prediction_count[imgno/2][label_matrix[argmax(count_spikes,6)]]+=1;
             c_index++;
             
}
    



//RESULTS OF PREDICTION

int accuracy_sum=0;
for(int i=0; i<6; i++)   
 accuracy_sum+=(predicted_class[i]==actual_class[i]); //how many predictions were correct?
printf("\nAccuracy sum: %d\n",accuracy_sum); 
float accuracy=(float)(accuracy_sum)/(6.0) *100.0;  //accuracy %

 
char str[5];

for(int i=0; i<3; i++)
{ for(int j=0; j<3; j++)
  {snprintf(str,3, "%d",prediction_count[i][j]); 
   printf("%s", str);  //print prediction matrix
   
   }
  printf("\n");
}


printf("\nAccuracy of model= %f percent \n",accuracy);
exit(0);
}
