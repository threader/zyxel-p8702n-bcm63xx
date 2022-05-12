#ifdef SDF_AUTHENTICATE

/******************************************************************************
** FUNCTION:
**			This file has the implementation of the Basic authentication
**			scheme.
**
*******************************************************************************
**
** FILENAME			: basic.c
**
** DESCRIPTION		: This file has the implementation of the Basic
**						authentication scheme
**  	
** DATE			NAME		REFERENCE		REASON
** ----			----		---------		------
** 13/01/01 	Subhash						Creation
**
*******************************************************************************
**				COPYRIGHT , Aricent, 2006
*******************************************************************************/

#include "basic.h"
void encodeBase64(char *input, char *output)
{
static char encodingTable[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};

int lenOfText = Sdf_mc_strlen(input);
int textProcessed = 0;
int textRemaining = 0;
unsigned char inbuf[3],outbuf[4];
int i;
/*printf("string = %s, length = %d\n",input,lenOfText);*/
int count =0;

#ifdef SDF_LINT
    if(input == Sdf_co_null)
    {
        *output = 0;   
        return;
    }
#endif
while(1)
{
	int copy = 4;
	int j;
	textRemaining = lenOfText - textProcessed;
	if(textRemaining <=0)
		break;
	for(i=0;i<3;i++)
 	{
		int currentLocation = textProcessed+i;
		if(currentLocation <lenOfText)
		{
			inbuf[i]=input[currentLocation];
		}
		else
		{
			inbuf[i]=0;
		}
	}/*close the for loop	*/
	outbuf[0] = (unsigned char)(inbuf[0]&(0xFC))>>2;
	outbuf[1] = ((inbuf[0]&(0x03))<<4)|((unsigned char)(inbuf[1]&(0xF0))>>4);
	outbuf[2] = ((inbuf[1]&(0x0F))<<2)|((unsigned char)(inbuf[2]&(0xC0))>>6);
	outbuf[3] =inbuf[2]&(0x3F);
	switch(textRemaining)
	{
		case 1:
				copy =2;
				break;
		case 2: 
				copy = 3;
				break;
	}
	for(j=0;j<copy;j++)	
	{
		/*printf("outbufbuf =%d",outbuf[j]);*/
		output[count] =encodingTable[outbuf[j]];
		count++;
	}
	for(j=copy;j<4;j++)	
	{
		output[count] ='=';
		count++;
	}
	textProcessed+=3;

/*printf("%s\n",output);	*/
	
}
	
}


void decodeBase64(char* input,char* result)
{
	unsigned char outbuffer[3];
	int lenOfText = Sdf_mc_strlen(input);
	int textProcessed = 0,textRemaining =0,copy=3,count =0,countChar=0,counter=0;
	int j;
	char out[4];
	char ch;

#ifdef SDF_LINT
    if(input == Sdf_co_null)
    {   
        *result = 0;
        return;
    }
#endif
	lenOfText=Sdf_mc_strlen(input);
	while(1)
	{
		int flag=0;
		textRemaining = lenOfText - textProcessed;
		if(textRemaining <=0)
			break;
		ch = input[count];
		if((ch>='A')&&(ch<='Z'))
			ch = ch -'A';
		else
		{
			if((ch>='a')&&(ch<='z'))
				ch = ch -'a'+26;
			else
			{
				if((ch>='0')&&(ch<='9'))
					ch = ch -'0'+52;
				else
				{
					if((ch=='+'))
						ch = 62;
					else
					{
						if((ch=='/'))
							ch = 63;
						else
						{
							if((ch=='='))
							{
								flag=1;
							}
						}
					}
				}
			}
		}
		if(flag==1)
		{	
			copy=3;
			if(countChar==0)
			break;
			if((countChar ==1)||(countChar ==2))
				copy=1;	
			else 
				copy=2;
			countChar=3;
		}
		out[countChar]=ch;
		countChar++;
		if(countChar==4)
		{
			outbuffer[0] = (icf_uint8_t)(((out[0])<<2)|((out[1]&(0x30))>>4));
			outbuffer[1] = (icf_uint8_t)(((out[1]&(0x0F))<<4)|((out[2]&(0x3C))>>2));
			outbuffer[2] = (icf_uint8_t)(((out[2]&(0x03))<<6)|((out[3]&(0x3F))));
			for(j=0;j<copy;j++)
			{
				result[counter] = outbuffer[j];
				counter++;
			}
			textProcessed+=3;
			countChar=0;
		}
		if(flag==1)
		{
			return;
		}	
		count++;
	}
	
}
#endif /* SDF_AUTHENTICATE */

