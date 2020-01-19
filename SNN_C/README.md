SNN_C

Implementation of Spiking Neural Network in C language, meant for RISC V architecture processors.

Files to be run :
1. classify.c (for <dirent.h> implementation)
2. finalclassifier.c (for implementation using minimal libraries- runs on actual chip)

Compilation steps (for above files):
 gcc -o filename filename.c -lm

Extra files/folders:
1. ./img and ./img1 : hold image intensity values for test images
2. ./mnist_png: holds actual test images
3. ./train_mnist: holds images used for training 
4. ./neuron : holds images obtained from result of training
5. read.py : transfers image intensities to folder

