/*
 * main.c
 *
 *  Created on: 2024. 12. 1.
 *      Author: josuh
 */
#include "xparameters.h"
#include <xil_types.h>
#include "xil_io.h"
#include <stdlib.h>
#include "xtime_l.h"
#include "xscugic.h"
unsigned int timerValue = 0; // XPAR_TIMER_0_S00_AXI_BASEADDR + 4
unsigned int keyNumHw;
volatile int interruptOccurred = 0;

typedef struct oledControl {
    u32 baseAddress;
} oledControl;

unsigned int hexToMilliseconds(unsigned int timerValue) {
    return timerValue / 100000;
}

void Delay(unsigned int delay)
{
    volatile unsigned int d;
    for(d = 0; d < delay; d++);
}

///////////////////////////oled Function////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int initOled(oledControl *myOled, u32 baseAddress){
	myOled->baseAddress = baseAddress;
	//printf("initOled....\n");
	return 0;
}

void writeCharOled(oledControl *myOled, char myChar){
	int num = 0;
	//printf("writeCharOled....\n");
	u32 status = 0;
	Xil_Out32(myOled->baseAddress+8, myChar);
	Xil_Out32(myOled->baseAddress, 0x1);
	while(status == 0){
		status = Xil_In32(myOled ->baseAddress+4); // polling mode
		num++;
	}
	Xil_Out32(myOled->baseAddress + 4, 0x0);
	//printf("Writing reg1 0\n");
}

void printOled(oledControl *myOled, char *myString){
	while(*myString != 0){
		writeCharOled(myOled,*myString);
		myString++;
	}
}
void clearOled(oledControl *myOled){
	u32 i;
	u32 j;
	for(i=0; i< 64; i++){
		for(j=0; j <1000 ; j++);
		writeCharOled(myOled,' ');
	}
	printf("clear oled Done");
}

void clearTimer(){
    Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 8, 0x1);
    Delay(10);
    Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 8, 0x0);
    Delay(10);
    Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 8, 0x1);
    Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 4, 0x0);
}

////////////////////////KEYPAD Function/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define KEYPAD_ST (volatile unsigned int*)(XPAR_MYKEYPAD2_0_S00_AXI_BASEADDR + 0)
#define KEYPAD_NUM (volatile unsigned int*)(XPAR_MYKEYPAD2_0_S00_AXI_BASEADDR + 4)

XScuGic INTCInst;

void MyKEYPADISR(void *arg)
{
    keyNumHw = *KEYPAD_NUM & 0xFF;
    printf("MyKEYPADISR. KeyNum = %d. \n", keyNumHw);
    Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR,0x0);
    timerValue = Xil_In32(XPAR_TIMER_0_S00_AXI_BASEADDR);
    printf("TimerValue: %X",timerValue);
    //reset procedure
    Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 8,0x0);	// timer clear
    Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 8,0x1);	// timer disbaled
    interruptOccurred = 1;
}

void InitGIC_Interrupt(){
    XScuGic_Config *IntcConfig;

    //return the address of system generated config instance corresponding to devicve ID
    IntcConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
    //XScuGic instance INTCInst init. using the config instance from XScuGic_LookupConfig
    XScuGic_CfgInitialize(
        &INTCInst,  // XScuGic instance to be initialized
        IntcConfig, // system generated config instance
        IntcConfig -> CpuBaseAddress //CPU base addr. in config instance
    );
    // Priority and edge/level trigger setting (default: level trigger)
    XScuGic_SetPriorityTriggerType(
        &INTCInst,
        XPAR_FABRIC_MYKEYPAD2_0_OIRQ_INTR,
        0,
        3 );
    //connect user defined interrupt handler and data to SCUGIC instance
    XScuGic_Connect(
        &INTCInst,                                //SCUGIC instance
        XPAR_FABRIC_MYKEYPAD2_0_OIRQ_INTR, //ID of the interrupt source
        (Xil_ExceptionHandler)MyKEYPADISR,         //User defined interrupt handler
        NULL//(void*)&Gpio0                             //ARg. to be passed to the handler
    );
    // Enable interrupt source provided by ID of the interrupt source
    XScuGic_Enable(
        &INTCInst,
        XPAR_FABRIC_MYKEYPAD2_0_OIRQ_INTR
    );
}

void InitCortex_Interrupt(){
    //INTCInst object passing
    //Before this call, (XScuGic)INTCInst should be prepared
    //XScuGic_InterruptHandler calls user defined interrupt handler defined in INTCInst
    Xil_ExceptionRegisterHandler(
        XIL_EXCEPTION_ID_INT,                           // exception ID
        (Xil_ExceptionHandler)XScuGic_InterruptHandler, // sys.generated handler
        &INTCInst                                       // argument data to interrupt handler
    );
    Xil_ExceptionEnable();
}

void EnableInterrupt() {
    XScuGic_Enable(&INTCInst, XPAR_FABRIC_MYKEYPAD2_0_OIRQ_INTR);
    printf("Interrupt enabled.\n");
}

void DisableInterrupt() {
    XScuGic_Disable(&INTCInst, XPAR_FABRIC_MYKEYPAD2_0_OIRQ_INTR);
    printf("Interrupt disabled.\n");
}
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int main(){
	XTime seed;
	XTime_GetTime(&seed);
	srand((unsigned int)seed);

    InitGIC_Interrupt();
    InitCortex_Interrupt();

    DisableInterrupt();

    unsigned int msTime = 0;

    unsigned int minDelay = 100000000;
    unsigned int maxDelay = 9000000000;
    unsigned int randomDelay = (rand() % (maxDelay - minDelay + 1)) + minDelay;
    unsigned int minNumber = 1;
    unsigned int maxNumber = 9;
    unsigned int randomNumber = (rand() % (maxNumber - minNumber + 1)) + minNumber;

	char *myString = "                Waiting....";
	char myString2[10];
	char myString3[20];
    char *myString4 = " Wrong Number!!";
    char *myString5 = "Press <A> to Start.";
	sprintf(myString2, "%u", randomNumber);
	oledControl myOled;

	Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 8,0x1);
	initOled(&myOled, XPAR_OLEDCONTROL_0_S00_AXI_BASEADDR);
    EnableInterrupt();
    clearOled(&myOled);
    printOled(&myOled,myString5);

    while (1)
    {
    	XTime seed;
    	XTime_GetTime(&seed);
    	srand((unsigned int)seed);
        unsigned int minDelay = 100000000;
        unsigned int maxDelay = 9000000000;
        unsigned int randomDelay = (rand() % (maxDelay - minDelay + 1)) + minDelay;
        unsigned int minNumber = 1;
        unsigned int maxNumber = 9;
        unsigned int randomNumber = (rand() % (maxNumber - minNumber + 1)) + minNumber;

        EnableInterrupt();
        while (1)
        {
            if (keyNumHw == 0xa)
            {
            	keyNumHw = *KEYPAD_NUM & 0x00000000;
            	interruptOccurred = 0;
                break;
            }
        }

	    clearOled(&myOled);
	    printOled(&myOled, myString);
	    // Waiting
	    Delay(randomDelay);
	    clearOled(&myOled);
		sprintf(myString2, "%u", randomNumber);
	    printOled(&myOled, myString2);
	    // random number occurred
        Xil_Out32(XPAR_TIMER_0_S00_AXI_BASEADDR + 4,0x1);

        while(1){
        if(interruptOccurred){
        	DisableInterrupt();
        	interruptOccurred = 0;
        	break;
        	}
        }

        if(randomNumber == keyNumHw){
            msTime = hexToMilliseconds(timerValue);
            sprintf(myString3, "\t%ums", msTime);
            clearOled(&myOled);
            printOled(&myOled,myString3);
        }
        else{
            clearOled(&myOled);
            printOled(&myOled,myString4);
        }
        clearTimer();
    }
}



