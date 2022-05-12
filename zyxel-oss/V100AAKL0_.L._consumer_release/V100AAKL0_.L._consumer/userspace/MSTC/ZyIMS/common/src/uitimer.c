/************************************************************************
 *
 *      Copyright (C) 2004-2008 ZyXEL Communications, Corp.
 *      All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Corp. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * ZyXEL Communications, Corp.
 *
 *************************************************************************/



#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>

#include "timer_list.h"
#include "uitimer.h"	


void *UITimerProc(void *arg);
void* freeTime( void* pData );

pthread_t timer_thread;
pthread_mutex_t timerMutex;
struct timer_list time_list;
struct timer_list periodic_time_list;


void UITimer_Init(void){
	#if 1/*ZyXEL, rt_priority_Eason*/
	pthread_attr_t attributes;
    struct sched_param schedparams;
    #endif

	timer_list_init(&time_list);
	timer_list_init(&periodic_time_list);
	pthread_mutex_init(&timerMutex, NULL);

	#if 1 /*ZyXEL, rt_priority_Eason*/
	pthread_attr_init( &attributes );
	pthread_attr_setschedpolicy( &attributes, SCHED_RR );
	schedparams.sched_priority = 75;
	pthread_attr_setschedparam( &attributes, &schedparams );
	pthread_create( (pthread_t*)&timer_thread, &attributes, (void*)UITimerProc, 0 );
	#else
	pthread_create(&timer_thread, NULL, UITimerProc, 0);
	#endif
	
	return;	
}

void *UITimerProc(void *arg){
	struct timeval timer_val;
	CMTimer_t *uitimer=NULL;
	timer_list_iterator_t iter;
	/*Eason, register application signal handler*/
	AppRegisterSignal();
	while(1){
		gettimeofday(&timer_val, NULL);
		zyMemset(&iter,0,sizeof(timer_list_iterator_t));
		pthread_mutex_lock(&timerMutex);
		uitimer =(CMTimer_t*) timer_list_get_first( &time_list, &iter );
		pthread_mutex_unlock(&timerMutex);
		while(uitimer){
			if( 	(uitimer->timeout.tv_sec>timer_val.tv_sec) ||
			((uitimer->timeout.tv_sec == timer_val.tv_sec) &&(uitimer->timeout.tv_usec > timer_val.tv_usec))){
				pthread_mutex_lock(&timerMutex);
				uitimer =(CMTimer_t*) timer_list_get_next( &iter );
				pthread_mutex_unlock(&timerMutex);
				continue;
			}
			pthread_mutex_lock(&timerMutex);
			timer_list_iterator_remove(&iter);
			pthread_mutex_unlock(&timerMutex);
			if(uitimer->fn) {
				uitimer->fn(uitimer->TimerID, uitimer->data);
			}
			zyFree(uitimer);
			uitimer =NULL;
			zyMemset(&iter,0,sizeof(timer_list_iterator_t));
			pthread_mutex_lock(&timerMutex);
			uitimer =(CMTimer_t*) timer_list_get_first( &time_list, &iter );
			pthread_mutex_unlock(&timerMutex);
		}

		zyMemset(&iter,0,sizeof(timer_list_iterator_t));
		pthread_mutex_lock(&timerMutex);
		uitimer =(CMTimer_t*) timer_list_get_first( &periodic_time_list, &iter );
		pthread_mutex_unlock(&timerMutex);
		while(uitimer){
			if( 	(uitimer->timeout.tv_sec>timer_val.tv_sec) ||
			((uitimer->timeout.tv_sec == timer_val.tv_sec) &&(uitimer->timeout.tv_usec > timer_val.tv_usec))){
				pthread_mutex_lock(&timerMutex);
				uitimer =(CMTimer_t*) timer_list_get_next( &iter );
				pthread_mutex_unlock(&timerMutex);
				continue;
			}
			if(uitimer->fn) {
				uitimer->fn(uitimer->TimerID, uitimer->data);
			}
			zyMemset(&(uitimer->timeout), 0, sizeof(struct timeval));
			zyMemcpy(&(uitimer->timeout), &timer_val, sizeof(struct timeval));
			uitimer->timeout.tv_sec  +=  uitimer->TimerLen/1000;
			uitimer->timeout.tv_usec  +=  (uitimer->TimerLen%1000)*1000;
			pthread_mutex_lock(&timerMutex);
			uitimer =(CMTimer_t*) timer_list_get_next( &iter );
			pthread_mutex_unlock(&timerMutex);
		}
		
		timer_val.tv_sec = 0;		
		timer_val.tv_usec = IDLE_TIME;		/* 100 ms idle */
		select(0, NULL, NULL, NULL, &timer_val);		
	}	
	return;
}

int UIAddTimer(int id,int timerLen,TMRFUNC timefn,void *privdata)
{
	timer_list_iterator_t iter;
	CMTimer_t *uitimer,*intimer;
	int res=-1;

	uitimer=zyMalloc(sizeof(CMTimer_t));
	if(uitimer==NULL) {
		return res;
	}

	gettimeofday(&uitimer->timeout, NULL);
	uitimer->TimerID = id;
	uitimer->TimerLen = timerLen;
#if 1  /* Solve inaccurate timer problem. Root cause: no consider that milisecond carry to second. Wayne fixed at 20090703. */
		uitimer->timeout.tv_usec  +=  (timerLen%1000)*1000;
		uitimer->timeout.tv_sec  +=  timerLen/1000 + uitimer->timeout.tv_usec/1000000;
		uitimer->timeout.tv_usec  =  uitimer->timeout.tv_usec%1000000;
#else
	uitimer->timeout.tv_sec  +=  timerLen/1000;
	uitimer->timeout.tv_usec  +=  (timerLen%1000)*1000;
#endif
	uitimer->data = (void*)privdata;
	uitimer->fn = timefn;

	pthread_mutex_lock(&timerMutex);
	intimer =(CMTimer_t*) timer_list_get_first( &time_list, &iter );
	pthread_mutex_unlock(&timerMutex);
	while(intimer){
		if(intimer->TimerID==id){
			intimer->TimerID=id;
			intimer->timeout.tv_sec=uitimer->timeout.tv_sec;
			intimer->timeout.tv_usec=uitimer->timeout.tv_usec;
			intimer->data= (void*)privdata;
			intimer->fn=timefn;
			
			return 0;
		}
		pthread_mutex_lock(&timerMutex);
		intimer=timer_list_get_next( &iter );
		pthread_mutex_unlock(&timerMutex);
	}
	pthread_mutex_lock(&timerMutex);
	res=timer_list_add(& time_list, uitimer, iter.pos )>0 ? 0: -1;
	pthread_mutex_unlock(&timerMutex);
	return res;
}

int UIAddPeriodicTimer(int id,int timerLen,TMRFUNC timefn,void *privdata)
{
	timer_list_iterator_t iter;
	CMTimer_t *uitimer,*intimer;
	int res=-1;
	uitimer=zyMalloc(sizeof(CMTimer_t));
	if(uitimer==NULL)
		return res;
	gettimeofday(&uitimer->timeout, NULL);
	uitimer->TimerID = id;
	uitimer->TimerLen = timerLen;
	uitimer->timeout.tv_sec  +=  timerLen/1000;
	uitimer->timeout.tv_usec  +=  (timerLen%1000)*1000;
	uitimer->data = (void*)privdata;
	uitimer->fn = timefn;

	pthread_mutex_lock(&timerMutex);
	intimer =(CMTimer_t*) timer_list_get_first( &periodic_time_list, &iter );
	pthread_mutex_unlock(&timerMutex);
	while(intimer){
		if(intimer->TimerID==id){
			intimer->TimerID=id;
			intimer->timeout.tv_sec=uitimer->timeout.tv_sec;
			intimer->timeout.tv_usec=uitimer->timeout.tv_usec;
			intimer->data= (void*)privdata;
			intimer->fn=timefn;
			
			return 0;
		}
		pthread_mutex_lock(&timerMutex);
		intimer=timer_list_get_next( &iter );
		pthread_mutex_unlock(&timerMutex);
	}
	pthread_mutex_lock(&timerMutex);
	res=timer_list_add(& periodic_time_list, uitimer, iter.pos )>0 ? 0: -1;
	pthread_mutex_unlock(&timerMutex);
	return res;
}

int UIDelTimer(int id){
	CMTimer_t *uitimer=NULL;
	timer_list_iterator_t iter;
	pthread_mutex_lock(&timerMutex);
	uitimer =(CMTimer_t*) timer_list_get_first( &time_list, &iter );
	pthread_mutex_unlock(&timerMutex);
	while(uitimer){	
		if(uitimer->TimerID !=id){
			pthread_mutex_lock(&timerMutex);
			uitimer =(CMTimer_t*) timer_list_get_next( &iter );
			pthread_mutex_unlock(&timerMutex);
			continue;
		}
		timer_list_iterator_remove(&iter);
		zyFree(uitimer);
		uitimer = NULL;
		zyMemset(&iter,0,sizeof(timer_list_iterator_t));
		pthread_mutex_lock(&timerMutex);
		uitimer =(CMTimer_t*) timer_list_get_first( &time_list, &iter );
		pthread_mutex_unlock(&timerMutex);
	}
	
	return 0;
}

int UIDelPeriodicTimer(int id){
	CMTimer_t *uitimer=NULL;
	timer_list_iterator_t iter;
	pthread_mutex_lock(&timerMutex);
	uitimer =(CMTimer_t*) timer_list_get_first( &periodic_time_list, &iter );
	pthread_mutex_unlock(&timerMutex);
	while(uitimer){	
		if(uitimer->TimerID !=id){
			pthread_mutex_lock(&timerMutex);
			uitimer =(CMTimer_t*) timer_list_get_next( &iter );
			pthread_mutex_unlock(&timerMutex);
			continue;
		}
		timer_list_iterator_remove(&iter);
		zyFree(uitimer);
		uitimer = NULL;
		zyMemset(&iter,0,sizeof(timer_list_iterator_t));
		pthread_mutex_lock(&timerMutex);
		uitimer =(CMTimer_t*) timer_list_get_first( &periodic_time_list, &iter );
		pthread_mutex_unlock(&timerMutex);
	}
	
	return 0;
}

int UIDelAllTimer(void){
	struct timer_list *list;

	pthread_mutex_lock(&timerMutex);
	list=&time_list;
	if(list->nb_elt>0){
		timer_list_special_free( &time_list, freeTime );
		timer_list_init( &time_list );
	}
	list=&periodic_time_list;
	if(list->nb_elt>0){
		timer_list_special_free( &periodic_time_list, freeTime );
		timer_list_init( &periodic_time_list );
	}
	pthread_mutex_unlock(&timerMutex);

	return 0;
}


void* freeTime( void* pData )
{
	CMTimer_t *uitimer = (CMTimer_t*)pData;
	if( uitimer )
		zyFree( uitimer );
	return NULL;
}

