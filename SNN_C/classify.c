/*##################### README ###################################################
# This file executes the classification algorithm over input testing images.
# Winner neurons inhibit other neurons by a phenomenon called Lateral inhibition
# Spike for each output neuron at each time stamp is monitored.
################################################################################*/


//header file inclusion
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include "neuron.h"
#include "recep_field.h"
#include <time.h>
#include "spike_train.h"
#include "rl.h"
#include "parameters.h"
#include<fcntl.h>
#include "csv.h"
#include<dirent.h>
#include<time.h>

#define MAXBUFLEN 1000000

//time series 
int time_of_learning[350];

// creating the hidden layer of neurons
struct neuron output_layer[6];

//spike train matrix
//float train[784];

//synapse matrix
float synapse[6][784];

//method to return the index of max element in a float array 
int argmax(float* arr, int n)
{int max=0,i;
 for(i=1;i<n;i++)
  if(arr[i]>arr[max])
    max=i;
  return max;}

//performing dot operation on 2 arrays
int dot(float synapse[6][784],int n,float** train,int m)
{float sum=0;
for(int i=0; i<784;i++)
 sum+=synapse[n][i]*train[i][m];
return sum;
 }

//main() function definition starts here.
void main()
{

int i,j;

//initializing synapse matrix.
for(i=0; i<6; i++){
for(j=0; j<784;j++){
synapse[i][j]=0;}}


for(i=1; i<=350; i++)
time_of_learning[i]=i;

//reading content from csv files into CSV structure.
CSV* learning_weights=csv_create(784,6);
csv_open(learning_weights,"weights.csv");
CSV* neuron_labels=csv_create(1,6);
csv_open(neuron_labels,"labels.csv");

//copy contents of csv into matrices
float weight_matrix[6][784];
int label_matrix[6];
int row, col;
        //creating label_matrix array from labels.csv file.
	char * content1;
	for (row=0; row<neuron_labels->rows; row++) {
		
		for (col=0; col<neuron_labels->cols; col++) {
			content1 = csv_get(neuron_labels, col, row);
                        label_matrix[row]=(int)atof(content1);
                        
		}
       }
   
       char * content2;
       //similarly,creating weight_matrix matrix, from weights.csv file.
       for (row=0; row<learning_weights->rows; row++) {
		content2="";
		for (col=0; col<learning_weights->cols; col++) {
			content2 = csv_get(learning_weights, col, row);
                         weight_matrix[row][col]=atof(content2);
                        
		}
       }
	

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



//READING IMAGE VALUES FROM TEXT FILE

//variables for reading directory containing text files having image values.
char source[MAXBUFLEN + 1];
    const char delim[4]=",";
    char* tok;
    int img[28][28];
    struct dirent *de;  // Pointer for directory entry 
    char name[50]="";
    char file[70]="";
    struct dirent *de1;
    FILE *input_file;
    FILE *next_file;
    int c_index=0;

// opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("./img"); 
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        exit(0); 
    } 

//opening /img directory
while ((de = readdir(dr)) != NULL) 
     {  
        if(strcmp(de->d_name,".")&&strcmp(de->d_name,".."))
        //printf("%s\n", de->d_name); for debugging.

        if((de->d_type==DT_DIR)&&(strcmp(de->d_name,".")&&strcmp(de->d_name,"..")))
        { 
          strcpy(name,"./img/");
          strcat(name,de->d_name);
          //printf("%s\n",name); for debugging.

          DIR *sdr= opendir(name); //opens directories inside ./img directory.

          while((de1=readdir(sdr))!=NULL)
          {  
            memset(&source[0], 0, sizeof(source)); //buffer for reading values from .txt file
            if(strcmp(de1->d_name,".")&&strcmp(de1->d_name,".."))
            {
             //printf("%s\n", de1->d_name); for debugging
             strcpy(file,name);
             strcat(file,"/");
             strcat(file,de1->d_name);//entire path of image
             //printf("%s\n",file); for debugging.

             if(de1->d_type==DT_REG)
             {
               input_file = fopen(file, "r");
               if ( ferror( input_file ) != 0 ) {
                 fputs("Error reading file", stderr);
              } 
         
             size_t newLen = fread(source, sizeof(char), MAXBUFLEN, input_file); 
             source[newLen++] = '\0';
             tok=strtok(source,delim);  //tokenizing the file content


             for(int i=0; i<28; i++){
                for(int j=0; j<28; j++){

                  if(tok!=0){
                   img[i][j]=atoi(tok); //initializing img array with intensity values read from file.
                    tok=strtok(0,delim);} 

                  else 
                     goto end; //end of file.
                }  }


             end: 
                 fclose(input_file);
    

             for(int i=0; i<6; i++)
               initial(&output_layer[i]);
             clock_t start=clock();    //Record starting time
             float count_spikes[]={0,0,0,0,0,0}; //initializing spike count to zero.

             float** pot=rf(img);//receptive field
 
             float** train=encode(pot);

             int f_spike=0;
             float active_pot[]={0,0,0,0,0,0}; //active potential
             int winner=0;

             for(int i=0; i<351; i++)
            {  for(int j=0; j<6; j++)
               {
                  if(output_layer[j].t_rest<time_of_learning[i])
                  {  output_layer[j].P+=dot(synapse,j,train,time_of_learning[i]);
                     //printf("%f\t",output_layer[j].P);
                     if(output_layer[j].P>Prest)
                      output_layer[j].P-=Pdrop;   
                   }
                   active_pot[j]=output_layer[j].P; 
                   //printf("%f\t",active_pot[j]);
                }


                for(int j=0; j<6;j++) 
                { if(j==argmax(active_pot,6))
                  { if(active_pot[j]>output_layer[j].Pth)
                    { 
                      count_spikes[j]+=1;
                      output_layer[j].Pth-=1;
                      hyperpolarisation(&output_layer[j],time_of_learning[i]);

                      for(int p=0;p<28;p++)
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
             printf("\n%s\n",file);
             printf("\nPredicted_class = %d\n",label_matrix[argmax(count_spikes,6)]);
             printf("\nActual class = %s\n",de->d_name);
              
             //time for inference
             clock_t time=clock()-start;
             double t = ((double)time)/CLOCKS_PER_SEC;
             printf("\nTime for inference= %f seconds\n",t);
             for(int i=0; i<6; i++)
                 printf("%f\t", count_spikes[i]);
             //prediction count matrix-aids in finding accuracy
             predicted_class[c_index]=label_matrix[argmax(count_spikes,6)];
             actual_class[c_index]=atoi(de->d_name);
	     prediction_count[atoi(de->d_name)][label_matrix[argmax(count_spikes,6)]]+=1;
             c_index++;
             
            }
           }
          }
        }
      }
    
closedir(dr);


//RESULTS OF PREDICTION

int accuracy_sum=0;
for(int i=0; i<6; i++)   
 accuracy_sum+=(predicted_class[i]==actual_class[i]); //how many predictions were correct?

float accuracy=(float)accuracy_sum/6.0 *100.0;  //accuracy %

CSV *pred_mat=csv_create(3,3); //create CSV structure 
char str[5];

for(int i=0; i<3; i++)
{ for(int j=0; j<3; j++)
  {snprintf(str,3, "%d",prediction_count[i][j]); //load content into csv structure 
   printf("%s", str);  //for debugging
   csv_set(pred_mat,j,i,str); 
   }
  printf("\n");
}

csv_display(pred_mat); //for debugging
csv_save(pred_mat,"prediction_matrix.csv"); //writing it to the CSV file.

printf("\nAccuracy of model= %f\n",accuracy);
exit(0);
}
