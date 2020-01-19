#ifndef _NEURONH_
#define _NEURONH_
#include<stdio.h>
#include "parameters.h"
struct neuron //define neuron dynamics
{ float P;
  float Pth;
  float t_rest;
  float t_ref;
};//self
void initial(struct neuron *self) //initial values for a neuron
{ self->Pth=Pth;
  self->t_rest=-1.0;
  self->t_ref=15.0;
  self->P=Prest;
}
void hyperpolarisation(struct neuron *self,int t) //hyperpolarisation
{ self->P=Phyperpolarisation;
  self->t_rest=(float)t+self->t_ref;
 }
void inhibit(struct neuron *self,int t) //inhibiting process
{ self->P=Pinhibit;
  self->t_rest=(float)t+self->t_ref;
}
/*void main()
{}*/
#endif 
