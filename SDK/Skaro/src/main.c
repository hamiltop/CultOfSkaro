/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMP
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREELEMENTATION, INCLUDING BUT NOT LIMITED TO
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Xilinx EDK 11.1 EDK_L.29.1
 *
 * This file is a sample test application
 *
 * This application is intended to test and/or illustrate some
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * XPS project when you run the "Generate Libraries" menu item
 * in XPS.
 *
 * Your XPS project directory is at:
 *    C:\EDK11\Rebuild2\
 */


// Located in: ppc405_0/include/xparameters.h
#include "xparameters.h"

#include "xcache_l.h"

#include "stdio.h"

#include "xutil.h"
#include "xcache_l.h"

#include <stdlib.h>

#include "xintc.h"
#include "xil_exception.h"
#include "InterruptControl.h"
#include "xtime_l.h"


#include  <fcntl.h>

//#include "SystemTypes.h"
#include "Header.h"
#include "ServoControl.h"
#include "PID_Control.h"
#include <unistd.h>
#include "xuartlite.h"
#include "xuartlite_l.h"

//====================================================

//more #includes to use the encoder
#include "plb_quad_encoder.h"
#include "Timer.h"
#include "HeliosIO.h"
#include "skaro_wireless.h"
#include "skaro_queue.h"
#include "scheduler.h"
#include "Serial.h"


#include "USB_IO.h"

//#include "ISR.h"

#define SET_PID_KP   		0x1
#define SET_PID_KI   		0x2
#define SET_PID_KD   		0x3
#define SET_PID_KP_D 		0x4
#define SET_PID_KI_D 		0x5
#define SET_PID_KD_D 		0x6
#define REQUEST_CONTROL_LOG 0x7
#define SET_DISTANCE		0x8
#define SET_STEERING		0x9
#define SET_MAX_VELOCITY	0xa
#define SET_25_CIRCLE1		0xb
#define SET_20_CIRCLE1		0xc
#define SET_OVERLAP			0xd
#define SET_25_CIRCLE2		0xe
#define SET_20_CIRCLE2		0xf
extern PID pid;

#define	STATE_DEFAULT					0x0
#define STATE_FIGURE_EIGHT_RIGHT_START	0x1
#define STATE_FIGURE_EIGHT_RIGHT_END	0x2
#define STATE_FIGURE_EIGHT_LEFT_START	0x3
#define	STATE_FIGURE_EIGHT_LEFT_END		0x4
#define STATE_FIGURE_EIGHT_RIGHT_HALF 	0x5
#define STATE_FIGURE_EIGHT_RIGHT_END_HALF	0x6
#define STATE_FIGURE_EIGHT_LEFT_START_2	0x7
#define	STATE_FIGURE_EIGHT_LEFT_END_2	0x8

#define CIRCLE_2_5_M_TICKS		16550
#define CIRCLE_2_0_M_TICKS		14100



//int logValues[1000];
//int logIndex = 0;

int print_flag = 1;
int state = STATE_DEFAULT;
int max_velocity = 4000;

int circle25_ticks1 = CIRCLE_2_5_M_TICKS;
int circle20_ticks1 = CIRCLE_2_0_M_TICKS;
int circle25_ticks2 = CIRCLE_2_5_M_TICKS;
int circle20_ticks2 = CIRCLE_2_0_M_TICKS;
int loop_count = 0;

int overlap = 0;

XIntc InterruptController;     /* The instance of the Interrupt Controller */

Skaro_Wireless wireless;
Scheduler scheduler;

int PIDvel(Xuint32 target, Xuint32 cur){
	if (cur < (target-5))
		return 1;
	else if (cur > (target+10))
		return 3;
	else
		return 7;
}

void showHelp(){
	xil_printf("Use the number pad to control the car via Terminal\r\n");
	xil_printf("8 Increment forward throttle\r\n");
	xil_printf("4 Steer Left\r\n");
	xil_printf("5 Center Steering\r\n");
	xil_printf("6 Steer Right\r\n");
	xil_printf("2 Increment reverse throttle\r\n");
	xil_printf("x Fire kill shot\r\n");
	xil_printf("z Fire pass shot\r\n");
	xil_printf("s Print truck status\r\n");
	xil_printf("h Print this help message\r\n");
	//xil_printf("q Center servos and exit\r\n\r\n");
}

void logData(int actual, int expected){
	/*
	int i;
	if(logIndex > 999){
		return 0;
	}
	logValues[logIndex++] = actual;
	logValues[logIndex++] = expected;\
	*/
}

void WirelessSendHandler(void *CallBackRef, unsigned int EventData)
{
	unsigned char c;
	wireless.send_in_progress = 0;
	if(!QueueIsEmpty(&(wireless.write_queue))) {
			wireless.send_in_progress = 1;
			c = QueuePop(&(wireless.write_queue));
			XUartLite_Send(&wireless.uart, &c, 1);
	}
}

void WirelessRecvHandler(void *CallBackRef, unsigned int EventData)
{
	char c = 0;

	if(!XUartLite_Recv(&(wireless.uart), &c, 1)){
		return;
	}

	//xil_printf("Command received: %d\r\n", c);

	char word[11];
	word[10] = 0;
	float temp;
	int temp_int;
	int i;
	char d;
	switch (c){
		case SET_PID_KP:
			read(0,&c,1);
			pid.Kp = c/25500.0;
//			xil_printf("We are setting Kp to ");
//			PrintFloat(pid.Kp);
			print("\n\r");
			break;
		case SET_PID_KI:
			read(0,&c,1);
			pid.Ki = c/25500.0;
//			xil_printf("We are setting Ki to ");
//			PrintFloat(pid.Ki);
//			print("\n\r");
			break;
		case SET_PID_KD:
			read(0,&c,1);
			pid.Kd = c/25500.0;
//			xil_printf("We are setting Kd to ");
//			PrintFloat(pid.Kd);
//			print("\n\r");
			break;
		case SET_PID_KP_D:
			read(0,&c,1);
//			xil_printf("received %d", c);
			pid.Kp_d = c/25500.0;
//			xil_printf("We are setting Kp_d to ");
//			PrintFloat(pid.Kp_d);
//			print("\n\r");
			break;
		case SET_PID_KI_D:
			read(0,&c,1);
			pid.Ki_d = c/25500.0;
//			xil_printf("We are setting Ki_d to ");
//			PrintFloat(pid.Ki_d);
//			print("\n\r");
			break;
		case SET_PID_KD_D:
			read(0,&c,1);
			pid.Kd_d = c/25500.0;
//			xil_printf("We are setting Kd_d to ");
//			PrintFloat(pid.Kd_d);
//			print("\n\r");
			break;
		case REQUEST_CONTROL_LOG:
//			for(i = 0; i < 1000; i++){
//				xil_printf("%d\n",logValues[i]);
//			}
			break;
		case SET_DISTANCE:
			read(0,word,10);
			//getWirelessBytes(word, 13);

			//word[11] = 0;
			//xil_printf("received %s\r\n", word);
			//print("\r\n");
			i = atoi(word);
			setDistance(i);
			//xil_printf("set distance to %d\r\n", i);
			break;
		case SET_STEERING:
			read(0,&c,1);
			SetServo(RC_STR_SERVO, (signed char)c);
			//xil_printf("Set Steering servo to %d\n\r", (signed char)c);
			break;
		case SET_MAX_VELOCITY:
			read(0,word,10);
			//xil_printf("received %s\r\n", word);
			i = atoi(word);
			//setVelocity(i);
			max_velocity = i;
			//xil_printf("set max velocity to %d\r\n", i);
			break;
		case SET_25_CIRCLE1:
			read(0,word,10);
//			xil_printf("received %s", word);
			circle25_ticks1 = atoi(word);
//			xil_printf("set distance to %d", circle25_ticks1);
			break;
		case SET_20_CIRCLE1:
			read(0,word,10);
//			xil_printf("received %s", word);
			circle20_ticks1 = atoi(word);
//			xil_printf("set distance to %d", circle20_ticks1);
			break;
		case SET_25_CIRCLE2:
			read(0,word,10);
//			xil_printf("received %s", word);
			circle25_ticks2 = atoi(word);
//			xil_printf("set distance to %d", circle25_ticks2);
			break;
		case SET_20_CIRCLE2:
			read(0,word,10);
//			xil_printf("received %s", word);
			circle20_ticks2 = atoi(word);
//			xil_printf("set distance to %d", circle20_ticks2);
			break;
		case SET_OVERLAP:
			read(0,word,10);
			//xil_printf("received %s", word);
			overlap = atoi(word);
			//xil_printf("set distance to %d", overlap);
			scheduler.events.flags.hello = 1;
			break;
		case 'a':
			loop_count = 2;
			state = STATE_FIGURE_EIGHT_LEFT_START;
			break;
		case 'b':
			loop_count = 2;
			state = STATE_FIGURE_EIGHT_RIGHT_START;
			break;
		case 'd':
			print("USB Status: ");
			xil_printf("%x\r\n", USB_getStatus());
			print("Testing writeReady of USB: ");
			if (!USB_writeReady()) {
				print("FAILED\r\n");
			} else {
				print("PASSED\r\n\r\n");
			}

			USB_setBurstSize(128);
			u16 data[100];
			for (i = 0; i < 100; i++) {
				data[i] = i;
			}
			//u32* longdword = (u32*)
			USB_blockWrite((u32*)data, 100);
			USB_forceCommit();

			print("wrote text to usb\r\n");

			break;
		case 'c':
			setDistance(0);
			state = STATE_DEFAULT;
			break;
		default:
			xil_printf("Unknown command received: %d\r\n", c);
			break;
	}
	while(!XUartLite_IsReceiveEmpty(wireless.uart.RegBaseAddress)){
		XUartLite_RecvByte(wireless.uart.RegBaseAddress);
	}
}

int counter = 0;
int update = 0;

// For the pit handler, we increment a counter.  Every time the counter reaches a certain threshold,
// We set the timer flag in the scheduler's events struct.  We reset the counter.
void my_pitHandler(){
	counter++;
	if(counter > 100){
		scheduler.events.flags.timer = 1;
		counter = 0;
	}
	XTime_PITClearInterrupt();
}

// This is a dummy task just to test out different events.
void hello(){
	Wireless_Debug("Hello World");
}

void InitInterrupts() {
		XUartLite_Initialize(&(wireless.uart), XPAR_WIRELESS_UART_DEVICE_ID);
		XUartLite_ResetFifos(&(wireless.uart));
		Wireless_Init(&wireless);

		/*
		 * Initialize the interrupt controller driver so that it is ready to
		 * use.
		 */
		XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);


		/*
		 * Connect a device driver handler that will be called when an interrupt
		 * for the device occurs, the device driver handler performs the
		 * specific interrupt processing for the device.
		 */
		XIntc_Connect(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR,
				(XInterruptHandler)XUartLite_InterruptHandler,
				(void *)&(wireless.uart));

		/*
		 * Start the interrupt controller such that interrupts are enabled for
		 * all devices that cause interrupts, specific real mode so that
		 * the UartLite can cause interrupts through the interrupt controller.
		 */
		XIntc_Start(&InterruptController, XIN_REAL_MODE);

		/*
		 * Enable the interrupt for the UartLite device.
		 */
		XIntc_Enable(&InterruptController, XPAR_XPS_INTC_0_WIRELESS_UART_INTERRUPT_INTR);

		/*
		 * Initialize the exception table.
		 */
		Xil_ExceptionInit();

		/*
		 * Register the interrupt controller handler with the exception table.
		 */
		Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				&InterruptController);

		/*
		 * Enable exceptions.
		 */
		Xil_ExceptionEnable();
		XUartLite_SetSendHandler(&(wireless.uart), WirelessSendHandler, &(wireless.uart));
		XUartLite_SetRecvHandler(&(wireless.uart), WirelessRecvHandler, &(wireless.uart));
		XUartLite_EnableInterrupt(&(wireless.uart));

	    XExceptionHandler pithandler = &my_pitHandler;
	    XExc_RegisterHandler(XEXC_ID_PIT_INT, pithandler,
	                      0);
	    XTime_PITEnableAutoReload();
	    XTime_PITSetInterval(100000);
	    XExc_mEnableExceptions(XEXC_ALL);
	    XTime_PITEnableInterrupt();
	    XTime_PITClearInterrupt();

}

// control_loop is registered with the scheduler on the 'timer' event.
// the timer event is set in the pit handler.
void control_loop(){
	int distance;
	updateDistanceSetVelocity(max_velocity);
	switch(state) {
		case STATE_DEFAULT:
			break;
		case STATE_FIGURE_EIGHT_LEFT_START:
			if(!loop_count){
				state = STATE_DEFAULT;
				break;
			}
			SetServo(RC_STR_SERVO, LEFT*26);
			if(loop_count == 2){
				setDistance(2*(circle25_ticks1 + circle20_ticks1) - 3*overlap);
			}
			loop_count--;
			state = STATE_FIGURE_EIGHT_LEFT_END;
			break;
		case STATE_FIGURE_EIGHT_LEFT_END:
				if (pid.distanceError < (circle20_ticks1 + circle25_ticks1 + circle20_ticks1 )) {
					state = STATE_FIGURE_EIGHT_LEFT_START;
					SetServo(RC_STR_SERVO, RIGHT*31);
				}
				break;
		case STATE_FIGURE_EIGHT_RIGHT_START:
			if(!loop_count){
				state = STATE_DEFAULT;
				break;
			}
			if(loop_count == 2){
				setDistance(circle25_ticks1 + circle25_ticks2 + circle20_ticks1 + circle20_ticks2);
				state = STATE_FIGURE_EIGHT_RIGHT_END;
				SetServo(RC_STR_SERVO, RIGHT*26);
				loop_count--;
			} else if (pid.distanceError < (circle20_ticks2 + circle25_ticks2 )) {
				state = STATE_FIGURE_EIGHT_RIGHT_HALF;
				SetServo(RC_STR_SERVO, RIGHT*26);
				loop_count--;
			}
			break;
		case STATE_FIGURE_EIGHT_RIGHT_HALF:
			if(loop_count == 1){
				distance = circle25_ticks2 + circle20_ticks1 +circle20_ticks2 + circle25_ticks1/2;
			} else {
				distance = circle25_ticks2/2 + circle20_ticks2;
			}
			if (pid.distanceError < distance) {
				state = STATE_FIGURE_EIGHT_RIGHT_END;
				SetServo(RC_STR_SERVO, RIGHT*26);
			}
			break;
		case STATE_FIGURE_EIGHT_RIGHT_END:
			if(loop_count == 1){
				distance = circle25_ticks2 + circle20_ticks1 +circle20_ticks2;
			} else {
				distance = circle20_ticks2;
			}
			if (pid.distanceError < distance) {
				state = STATE_FIGURE_EIGHT_RIGHT_END_HALF;
				SetServo(RC_STR_SERVO, LEFT*31);
			}
			break;
		case STATE_FIGURE_EIGHT_RIGHT_END_HALF:
			if(loop_count == 1){
				distance = circle25_ticks2 + circle20_ticks2 +circle20_ticks1/2;
			} else {
				distance = circle20_ticks2/2;
			}
			if (pid.distanceError < distance) {
				state = STATE_FIGURE_EIGHT_RIGHT_START;
				SetServo(RC_STR_SERVO, LEFT*31);
			}
			break;
	}
}

int main (void) {
	int32 str;
	int32 thr;
//	Xuint32 velcount = 0;
	//Xuint32 eTicks = 0;
	//Xuint32 maxTicks = 10;
//	Xuint32 sTicks1 = 0;
//	Xuint32 sTicks2 = 0;
//	Xuint32 vel = 0;
	//Xuint32 cvel = 0;
//	Xuint32 pid = 0;
	///int ticks;
	//int ticks_prev;

	InitInterrupts();

	USB_init();

	/* Scheduler initialization */
	Scheduler_Init(&scheduler);

	/* Initialize Tasks */
	Events control_events;  // in order to register the control loop,
							// we need an events struct
	control_events.reg = 0;  		// clear all events
	control_events.flags.timer = 1;	// set the flags we want as triggers
	Scheduler_RegisterTask(&scheduler,control_loop,control_events);  // Register task with Scheduler

	// Another example
	Events hello_events;
	hello_events.reg = 0;
	hello_events.flags.hello = 1;
	Scheduler_RegisterTask(&scheduler,hello,hello_events);



	str = 0;
	thr = 0;
	
	XCache_EnableICache(0x00000001);
	XCache_EnableDCache(0x00000001);
	
	InitGameSystem();
	Wireless_Debug("-- Starting PWM Test Mode --\r\n");
	Wireless_Debug("Current max velocity: ");
	PrintInt(max_velocity);
	Wireless_Debug("\r\n");
	Wireless_Debug("NOTICE: Make sure car is placed on stand before proceeding!!\r\n");
	//showHelp();
	//Initialze servos to center position
	ServoInit(RC_STR_SERVO);
	ServoInit(RC_VEL_SERVO);
	//Wireless_Debug("Steering Setting: %d\r\n",GetServo(RC_STR_SERVO));
	//Wireless_Debug("Throttle Setting: %d\r\n\r\n",GetServo(RC_VEL_SERVO));
	initPID();
	//setVelocity(3000);
	setDistance(0);
	SetServo(RC_STR_SERVO, 1);

	while(1){
		/*  Run Scheduler repeatedly */
		Scheduler_Run(&scheduler);
	}

	Wireless_Debug("-- Exiting main() --\r\n");
	XCache_DisableDCache();
	XCache_DisableICache();
	return 0;
}