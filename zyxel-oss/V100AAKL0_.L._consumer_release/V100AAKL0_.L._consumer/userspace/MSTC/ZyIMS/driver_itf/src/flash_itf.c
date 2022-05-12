#include "global.h"

int flashIvrRead(char * string,int strLen, int offset){
	/* Platform porting: Read IVR file from Flash */
	/* END of Platform porting */
#ifdef INC_NAND_FLASH_DRIVER 
	FILE *fp= NULL;
	int i=0;
#endif

	#ifndef INC_NAND_FLASH_DRIVER
	int ret = BroadcomFlashReadIvr(string, strLen, offset);
	if(ret == 0){
		return TRUE;
	}
#else
	printf("flashIvrRead\n");
	printf("strLen:%d\n",strLen);
	printf("offset:%d\n",offset);
	if(offset==0) /*user*/
	{
		if( (fp = fopen("/data/ivr_data_usr","rb")) != NULL ) {
			//fread(string, strLen, 1, fp);
			//fseek(fp,offset,SEEK_SET);
			fread(string, strLen, 1, fp);
			fclose(fp);
			return TRUE;
		}
	}
	else
	{
		if( (fp = fopen("/data/ivr_data_sys","rb")) != NULL ) {
			//fread(string, strLen, 1, fp);
			//fseek(fp,offset,SEEK_SET);
			fread(string, strLen, 1, fp);
			fclose(fp);
			return TRUE;
		}
	}
	#endif

	return FALSE;
}

int flashIvrWrite(char * string,int strLen, int offset){
	/* Platform porting: Write IVR file to Flash */
	/* END of Platform porting */	

#ifdef INC_NAND_FLASH_DRIVER
	FILE *fp= NULL;
	int i=0;
#endif
	
	#ifndef INC_NAND_FLASH_DRIVER
	int ret = BroadcomFlashWriteIvr(string, strLen, offset);
	if(ret == 0){
		return TRUE;
	}
#else
	printf("flashIvrWrite\n");
	printf("strLen:%d\n",strLen);
	printf("offset:%d\n",offset);
	if(offset==0) /*user*/
	{
		if( (fp = fopen("/data/ivr_data_usr","w+b")) != NULL ) {
			//fseek(fp,offset,SEEK_SET);
			fwrite(string, strLen, 1, fp);
			fclose(fp);
			return TRUE;
		}	
	}
	else
	{
		if( (fp = fopen("/data/ivr_data_sys","w+b")) != NULL ) {
			//fread(string, strLen, 1, fp);
			//fseek(fp,offset,SEEK_SET);
			fwrite(string, strLen, 1, fp);
			fclose(fp);
			return TRUE;
		}
	}
	#endif
	return FALSE;

}

