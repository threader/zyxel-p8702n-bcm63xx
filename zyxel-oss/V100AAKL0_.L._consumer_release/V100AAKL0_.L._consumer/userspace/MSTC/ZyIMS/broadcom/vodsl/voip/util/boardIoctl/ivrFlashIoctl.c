#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "board.h"

#define BOARD_DEVICE_NAME  "/dev/brcmboard"


int	BroadcomBoardIoctl(	int boardIoctl,
                         BOARD_IOCTL_ACTION action,
                         char *string,
                         int strLen,
                         int offset,
                         void *data)
{
	int boardFd;
	BOARD_IOCTL_PARMS ioctlParms;
	int rc=-1;
	boardFd = open(BOARD_DEVICE_NAME, O_RDWR);

	if ( boardFd != -1 )
    {
        ioctlParms.string = string;
        ioctlParms.strLen = strLen;
        ioctlParms.offset = offset;
        ioctlParms.action = action;
        ioctlParms.buf    = data;
        ioctlParms.result = -1;

        rc = ioctl(boardFd, boardIoctl, &ioctlParms);
        close(boardFd);
	}
	return rc;
}



int	BroadcomFlashWriteIvr(	char *string,
                     int strLen,
                     int offset)
{
	#ifdef ZYXEL_IVR_PORTING
	return BroadcomBoardIoctl(	BOARD_IOCTL_FLASH_WRITE,
                         ZYXEL_IVR,
                         string,
                         strLen,
                         offset,
                         0);
	#else
	return -1;
	#endif
	
}

int	BroadcomFlashReadIvr(	char *string,
                     int strLen,
                     int offset)
{
#ifdef ZYXEL_IVR_PORTING
	return BroadcomBoardIoctl(	BOARD_IOCTL_FLASH_READ,
                         ZYXEL_IVR,
                         string,
                         strLen,
                         offset,
                         0);
#else
	return -1;
#endif
	
}

