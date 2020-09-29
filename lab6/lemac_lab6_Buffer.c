/*
 * File:   lemac_lab6_Buffer.c
 * Author: Alex
 *
 * Created on November 13, 2019, 2:46 AM
 */


#include <stddef.h>

#include "xc.h"
#define BUFFER_SIZE 1024

volatile int theBuffer[BUFFER_SIZE];
volatile int bufferFront=0;

void putVal(int newValue)
{
    theBuffer[bufferFront++] = newValue;
    if(bufferFront == BUFFER_SIZE)
        bufferFront = 0;
}

int getAvg()
{
    long avg = 0;
    int realPos=0;
    int offsetPos=bufferFront;
    int nonZeroVals=0;
    for(realPos=0;realPos<BUFFER_SIZE;realPos++)
    {
        if(theBuffer[offsetPos] != 0){
            nonZeroVals++;
            avg+=theBuffer[offsetPos];
        }
        if((++offsetPos)==BUFFER_SIZE)
            offsetPos = 0;
    }
    // Should never occur before the first 1k samples, but coded to.
    if(nonZeroVals==0)
        return 0;
    
    return (int)(avg/nonZeroVals);
}

void initBuffer()
{
    int counter = 0;
    bufferFront = 0;
    for(counter = 0;counter<BUFFER_SIZE;theBuffer[counter++] = 0);
}


/*
 #define BUF_SIZE 1024
unsigned long avg = 0;
long buffer[BUF_SIZE];
long myIndex = 0;

void putVal(int newValue) { // add a new value to the buffer
    buffer[myIndex++] = newValue;
    if(myIndex >= BUF_SIZE) {
        myIndex = 0;
    }
}    
int getAvg() {
    long sum = 0;
    long i;
    for(i = 0; i < BUF_SIZE; i++) {
        sum += buffer[i];
    }
    avg = sum / BUF_SIZE;
    return avg;
} // average all buffer vals
void initBuffer() {
    long i;
    for(i = 0; i < BUF_SIZE; i++) {
        buffer[i] = 0;
    }
}
 
 */