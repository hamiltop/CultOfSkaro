#include "skaro_wireless.h"
#include "Queue.h"
#include "xuartlite.h"
#include <string.h>
#include <stdio.h>
#include <InterruptControl.h>
#include <stdarg.h>
#include <ctype.h>
#include <xil_sprintf.h>

extern Skaro_Wireless wireless;

int Wireless_Init(Skaro_Wireless * w){
	w->send_in_progress = 0;
	QueueInit(&(w->write_queue), (void *)w->write_data, WRITE_QUEUE_SIZE);
	QueueInit(&(w->read_queue), (void *)w->read_data, READ_QUEUE_SIZE);
	return 1;
}

int Wireless_Destroy(Skaro_Wireless * w){
	return 1;
}

int Wireless_Send(Skaro_Wireless * w, char command, char length, char * data){
	unsigned char top;
	CPU_MSR msr = DISABLE_INTERRUPTS();
	QueuePush(&(w->write_queue), (void *)(uint32)command);
	QueuePush(&(w->write_queue),(void *)(uint32)length);

	int i;
	for(i = 0; i < length; i++){
		QueuePush(&w->write_queue, (void *)(uint32)data[i]);
	}

	// If there is no send in progress, initiate a send
	if (!(w->send_in_progress)) {
		// We are about to send it, so pop it off
		top = (unsigned char)(uint32)QueuePop(&(w->write_queue));
		w->send_in_progress = 1;
		XUartLite_Send(&(w->uart), &top, 1);
	}
	RESTORE_INTERRUPTS(msr);
	return 1;
}

void Wireless_Debug(char * message) {
	int length;

	length = strlen(message);
	// do more stuff here (like write to file)
	Wireless_Send(&wireless, WIRELESS_DEBUG, length, message);
}

int Wireless_ControlLog(int actual, int expected){
	int values[2];
	values[0] = actual;
	values[1] = expected;
	return Wireless_Send(&wireless, WIRELESS_CONTROL_LOG, 2*sizeof(int), values);
}

//int mains(){
//  Wireless_Init(&wireless);
//  Wireless_ControlLog(1200,100);
//  Wireless_Debug("How well does this work");
//  char c;
//  int i;
//  int length;
//  char format_string[10];
//  Queue_ReadByte(&wireless.write_queue, &c);
//  printf("%d\n",c);
//  if(c == WIRELESS_DEBUG){
//    strcpy(format_string,"%c\n");
//  } else if (c == WIRELESS_CONTROL_LOG){
//    strcpy(format_string,"%d\n");
//  }
//  Queue_ReadByte(&wireless.write_queue, &c);
//  printf("%d\n",c);
//  length = c;
//  for(i = 0; i < length; i++){
//    Queue_ReadByte(&wireless.write_queue, &c);
//    printf(format_string,c);
//  }
//  Wireless_Destroy(&wireless);
//}