#ifndef __SYS_ITF_H__
#define __SYS_ITF_H__

#ifdef BUILD_IGNORE_DIRECT_IP
#include <netinet/in.h>
#endif
/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
typedef enum
{
   TASK_CLASS_HIGH,             /**< High priority task.               */
   TASK_CLASS_MED_HIGH,         /**< Medium-High priority task.        */
   TASK_CLASS_MED,              /**< Medium priority task.             */
   TASK_CLASS_MED_LOW,          /**< Medium-Low priority task.         */
   TASK_CLASS_LOW,              /**< Low priority task.                */

} TASK_CLASS;

int vendorOsInit(void);
int taskCreate(const char *name, int stackSize, TASK_CLASS taskClass, void *taskMain);

#endif


