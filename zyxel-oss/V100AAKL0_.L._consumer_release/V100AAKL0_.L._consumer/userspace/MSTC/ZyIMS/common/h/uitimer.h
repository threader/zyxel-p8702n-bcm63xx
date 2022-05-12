#ifndef __UI_TIMER_H
#define __UI_TIMER_H


#include <stdio.h>
#define IDLE_TIME 10000


#define EDIT_TIMER 0x99
#define IDLE_TIMER 0x199
#define VOL_TIMER 0x201
#define SD_TIMER 0x299
#define CON_TIMER 0x399
#define SWITCH_TIMER 0x498
#define RESTORE_TIMER 0x499

typedef int (*TMRFUNC)(int,void *);

typedef struct CMTimer_t {
	int TimerID;
	void *data;		/* private data of timer */
	int TimerLen;
	struct timeval	timeout;		/* timeout time of timer */
	TMRFUNC fn;
}CMTimer_t;

void UITimer_Init(void);


int UIAddTimer(int id,int timerLen,TMRFUNC timefn,void *privdata);
int UIDelTimer(int id);
int UIDelAllTimer(void);

#endif

