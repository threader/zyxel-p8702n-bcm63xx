
#include <stdio.h>

extern int PhoneInit(void);

void phonetest (
	void
) {
	printf("phonetest done\n");	
	if((PhoneInit())!=0)
		printf("PhoneInit failed\n");	
}

