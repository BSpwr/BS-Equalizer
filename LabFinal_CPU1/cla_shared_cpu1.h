#ifndef CLA_SHARED_CPU1_H_
#define CLA_SHARED_CPU1_H_

#include "F2837xD_Cla_defines.h"
#include <stdint.h>


#define NUM_BANDS 10

#define IIR_FILTER_NUM_STAGES 5
typedef struct IIR_FILTER {
    float taps[IIR_FILTER_NUM_STAGES][6];
    float buffer[IIR_FILTER_NUM_STAGES][4];
} IIR_FILTER;


extern IIR_FILTER h1;
extern IIR_FILTER h2;
extern IIR_FILTER h3;
extern IIR_FILTER h4;
extern IIR_FILTER h5;
extern IIR_FILTER h6;
extern IIR_FILTER h7;
extern IIR_FILTER h8;
extern IIR_FILTER h9;
extern IIR_FILTER h10;

//
// Globals
//
//Task 1 (C) Variables
//
extern float gains_cla[NUM_BANDS];
extern float output_sample_left;           //Result
extern float input_sample_left;           //Holds the input argument to the task

//
//Task 2 (C) Variables
//



//
//Task 3 (C) Variables
//

//
//Task 4 (C) Variables
//

//
//Task 5 (C) Variables
//

//
//Task 6 (C) Variables
//

//
//Task 7 (C) Variables
//

//
//Task 8 (C) Variables
//

//
//Common (C) Variables
//

//
// Function Prototypes
//
// The following are symbols defined in the CLA assembly code
// Including them in the shared header file makes them
// .global and the main CPU can make use of them.
//

//
//CLA C Tasks
//
interrupt void Cla1Task1();
interrupt void Cla1Task2();
interrupt void Cla1Task3();
interrupt void Cla1Task4();
interrupt void Cla1Task5();
interrupt void Cla1Task6();
interrupt void Cla1Task7();
interrupt void Cla1Task8();

#endif //end of _CLA_ASIN_SHARED_H_ definition

