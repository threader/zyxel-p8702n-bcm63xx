/* <:copyright-BRCM:2013:DUAL/GPL:standard
/* 
/*    Copyright (c) 2013 Broadcom Corporation
/*    All Rights Reserved
/* 
/* This program is free software; you can redistribute it and/or modify
/* it under the terms of the GNU General Public License, version 2, as published by
/* the Free Software Foundation (the "GPL").
/* 
/* This program is distributed in the hope that it will be useful,
/* but WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/* GNU General Public License for more details.
/* 
/* 
/* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
/* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
/* Boston, MA 02111-1307, USA.
/* 
:>	
*/

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define swap2bytes(a)           ( ( ( (a) & 0xFF ) << 8 ) | ( ( (a) & 0xFF00 ) >> 8 ) )
#define swap4bytes(a)           ( ( ( (a) & 0xFF ) << 24 ) | ( ( (a) & 0xFF00 ) << 8 ) | ( ( (a) & 0xFF0000 ) >> 8 ) | ( ( (a) & 0xFF000000 ) >> 24 ) )

// format-rule : uncrustify bcm_minimal_i4

typedef enum {
	_Undefined_,
	_DumpWord_,
	_DumpHalfWord_,
	_DumpByte_,
	_SetWord_,
	_SetHalfWord_,
	_SetByte_,
	_FillWord_,
	_FillHalfWord_,
	_FillByte_
} eMode;


#define NUM_WORDS_PER_LINE 4
#define NUM_HALFWORDS_PER_LINE 8
#define NUM_BYTES_PER_LINE 16
#define MAX_DATA_ALLOWED 64

typedef unsigned int word;
typedef unsigned short halfword;
typedef unsigned char byte;

typedef int bool;
#define true 1
#define false 0


// Globals
long pageSize;

void printUsage(char *cmd)
{
	printf("\n*** Usage:\n"
			" dw/dh/db <physical address in hex> <number> \n"
			" dw/dh/db <-k> <virtual address in hex> <number> \n"
			" sw/sh/sb <physical address in hex> <data value1> <data value2> ..<data valueN> \n"
			" sw/sh/sb <-k> <virtual address in hex> <data value1> <data value2> ..<data valueN> \n"
			" fw/fh/fb  <physical address in hex> <data value> <length> \n"
			" fw/fh/fb <-k> <virtual address in hex> <data value> <length> \n"
			"  -s (currently works with physical addresses for d*/s*/f* commands\n"
			"	and virtual addresses for s*/f* commands)\n"
			"\n", cmd);
}


bool isReadMode(eMode mode)
{
	switch(mode)
	{
	case _DumpWord_:
	case _DumpHalfWord_:
	case _DumpByte_:
		return true;
	default:
		return false;
	}
}

bool isWriteMode(eMode mode)
{
	switch(mode)
	{
	case _SetWord_:
	case _SetHalfWord_:
	case _SetByte_:
		return true;
	default:
		return false;
	}
}

bool isFillMode(eMode mode)
{
	switch(mode)
	{
	case _FillWord_:
	case _FillHalfWord_:
	case _FillByte_:
		return true;
	default:
		return false;
	}
}


eMode modeStrToEnum(char *modeStr)
{
	if(strcmp(modeStr, "dw") == 0) { return _DumpWord_; }
	else if(strcmp(modeStr, "dh") == 0) { return _DumpHalfWord_; }
	else if(strcmp(modeStr, "db") == 0) { return _DumpByte_; }
	else if(strcmp(modeStr, "sw") == 0) { return _SetWord_; }
	else if(strcmp(modeStr, "sh") == 0) { return _SetHalfWord_; }
	else if(strcmp(modeStr, "sb") == 0) { return _SetByte_; }
	else if(strcmp(modeStr, "fw") == 0) { return _FillWord_; }
	else if(strcmp(modeStr, "fh") == 0) { return _FillHalfWord_; }
	else if(strcmp(modeStr, "fb") == 0) { return _FillByte_; }
	else { return _Undefined_; }
}

char modeToKernStr(eMode mode)
{
	switch(mode)
	{
	case _DumpWord_:
	case _SetWord_:
	case _FillWord_:
		return 'w';
	case _DumpHalfWord_:
	case _SetHalfWord_:
	case _FillHalfWord_:
		return 'h';
	case _DumpByte_:
	case _SetByte_:
	case _FillByte_:
		return 'b';
	default:
		return -1;
	}

}

void exitWithUsage(char *cmd)
{
	printUsage(cmd);
	exit(0);
}


void DumpWordsFromAddr(unsigned long addr, unsigned long baseAddr, void *baseLoc, int numWords, int swap)
{
	word *wordptr = NULL;
	unsigned int offset = 0;
	unsigned int nthword = 0;

	offset = (addr - baseAddr) / sizeof(word);
	wordptr = ((word *) baseLoc) + offset;

	for(nthword = 0; nthword < numWords; nthword++)
		{
			if((nthword % NUM_WORDS_PER_LINE) == 0)
			{
				printf("\n0x%X : ", addr);
			}
		
			if (swap) 
			{
				printf("%08x ", swap4bytes(*(wordptr)));
			}	 
			else 
			{
				printf("%08x ", *(wordptr));
			}
			addr += sizeof(word);
			wordptr++;
		}

	printf("\n");
}

void DumpHalfWordsFromAddr(unsigned long addr, unsigned long baseAddr, void *baseLoc, int numHalfWords, int swap)
{
	halfword *halfwordptr = NULL;
	unsigned int offset = 0;
	unsigned int nthhalfword = 0;

	offset = (addr - baseAddr) / sizeof(halfword);
	halfwordptr = ((halfword *) baseLoc) + offset;

	for(nthhalfword = 0; nthhalfword < numHalfWords; nthhalfword++)
		{
		if((nthhalfword % NUM_HALFWORDS_PER_LINE) == 0)
		{
			printf("\n0x%X : ", addr);
		}

		if((nthhalfword != 0) && ((nthhalfword % NUM_HALFWORDS_PER_LINE) != 0) && 
				((nthhalfword % sizeof(halfword)) == 0))
		{
			printf(" ");
		}
		
		if (swap)
		{
			printf("%04x ", swap2bytes(*(halfwordptr)));

		}
		else 
		{
			printf("%04x ", *(halfwordptr));
		}
		addr += sizeof(halfword);
		halfwordptr++;
		}

	printf("\n");
}


void DumpBytesFromAddr(unsigned long addr, unsigned long baseAddr, void *baseLoc, int numBytes, int swap)
{
	byte *byteptr = NULL;
	unsigned int offset = 0;
	unsigned int nthbyte = 0;

	offset = (addr - baseAddr) / sizeof(byte);
	byteptr = ((byte *) baseLoc) + offset;

	for(nthbyte = 0; nthbyte < numBytes; nthbyte++)
		{
		if((nthbyte % NUM_BYTES_PER_LINE) == 0)
		{
			printf("\n0x%X : ", addr);
		}

		if((nthbyte != 0) && ((nthbyte % NUM_BYTES_PER_LINE) != 0) && 
  	  ((nthbyte % (sizeof(byte) * 4) == 0)))
		{
			printf(" ");
		}

		printf("%02x ", *(byteptr));
		addr += sizeof(byte);
		byteptr++;
		}

	printf("\n");
}

void SetWordsFromAddr(unsigned long addr, unsigned long baseAddr, void *baseLoc, 
			int numWords, unsigned long *data, bool fill, int swap)
{
	word *wordptr = NULL;
	unsigned int offset = 0;
	unsigned int nthword = 0;

	//printf("Set word addr=0x%X numwords=%d data[0]=0x%X\n", addr, numWords, data[0]);

	offset = (addr - baseAddr) / sizeof(word);
	wordptr = ((word *)baseLoc) + offset;

	for(nthword = 0; nthword < numWords; nthword++)
	{
	if(fill)
		{
		*wordptr = data[0];
		}
	else
		{
		if (swap)
		{
			*wordptr = swap4bytes(data[nthword]);
		}
		else 
		{
			*wordptr = data[nthword];
		}
	}
	wordptr++;
	}
}

void SetHalfWordsFromAddr(unsigned long addr, unsigned long baseAddr, void *baseLoc, 
			int numHalfWords, unsigned long *data, bool fill, int swap)
{
#ifdef MSTC_MODIFY
	halfword *halfwordptr = NULL;
	unsigned int offset = 0;
	unsigned int nthhalfword = 0;

	offset = (addr - baseAddr) / sizeof(halfword);
	halfwordptr = ((halfword *)baseLoc) + offset;

	for(nthhalfword = 0; nthhalfword < numHalfWords; nthhalfword++)
	{
		if (fill)
		{
			
			*halfwordptr = data[0];
		}
		else 
		{
			if (swap)
			{
				*halfwordptr = swap2bytes(data[nthhalfword]);
			}
			else 
			{
					*halfwordptr = data[nthhalfword];
			}
		}
	halfwordptr++;
	}
#endif
}

void SetBytesFromAddr(unsigned long addr, unsigned long baseAddr, void *baseLoc, 
			int numBytes, unsigned long *data, bool fill)
{
#ifdef MSTC_MODIFY
	byte *byteptr = NULL;
	unsigned int offset = 0;
	unsigned int nthbyte = 0;

	offset = (addr - baseAddr) / sizeof(byte);
	byteptr = ((byte *)baseLoc) + offset;

	for(nthbyte = 0; nthbyte < numBytes; nthbyte++)
	{
		if (fill)
		{
			*byteptr = data[0];
		}
		else 
		{
			*byteptr = data[nthbyte];
		}
	byteptr++;
	}
#endif
}

void KernelModeWrite(unsigned long addr, unsigned long *data, unsigned int totNumData, eMode mode, bool fill, bool swap)
{
	unsigned int nthData = 0;
	char kerncmd[200] = "";
	char dataStr[200];
	char * dataStrPtr = dataStr;
	
	if (fill)
		{
			sprintf(dataStrPtr, "0x%X ", data[0]); 
		}
	else 
		{
			for (nthData = 0;  nthData < totNumData; nthData++)
				{
					if (swap)
						{				
							if (modeToKernStr(mode) == 'w')
								{
									sprintf(dataStrPtr, "0x%X ", swap4bytes(data[nthData])); 
								}
							else  if (modeToKernStr(mode) == 'h')
								{
									sprintf(dataStrPtr, "0x%X ", swap2bytes(data[nthData])); 
								} 
						}		
					else 
					{
							sprintf(dataStrPtr, "0x%X ", data[nthData]); 
					}
					dataStrPtr = dataStrPtr + strlen(dataStrPtr);
				}
		}
	sprintf(kerncmd, "echo %s 0x%X %s %d %c > /proc/bcmlog", "w", 
			addr, dataStr, totNumData, modeToKernStr(mode));
	
	system(kerncmd);
}


void KernelModeRead(unsigned long addr, unsigned int numData, eMode mode)
{

	char kerncmd[200] = "";
	sprintf(kerncmd, "echo %s 0x%X %d %c > /proc/bcmlog", "m", 
			addr, numData, modeToKernStr(mode));

	system(kerncmd);

/*	FILE *blog;
	blog = 	fopen("/proc/bcmlog", 'r');
	fprintf(blog, "%s 0x%X %d %c\n", "m", addr, numData, modeToKernStr(mode));
	fclose(blog);  

*/
}




int main(int argc, char *argv[])
{

	int opt,num,swap,kernelspace;
	eMode mode = _Undefined_;
	unsigned long addr = 0; 
	long data[MAX_DATA_ALLOWED];
	int fd;
	char *baseLoc;
	char **endptr = NULL;
	int totNumData, nthData;
	bool addrDefined = false;
 	char *cmd;
	
	opt = 0;
	num = 1;
	swap = 0;
	kernelspace = 0;
	totNumData = 0;
	
	static struct option long_options[] = {
		
		{ "kernel", no_argument	, 0, 'k' },
		{ "swap", no_argument	, 0, 's' },
    {0, 0, 0, 0 }
	
	};

	cmd = strrchr(argv[0], '/');
	if(cmd == NULL)
	{
		cmd = argv[0];
	}
	else
	{
		cmd++;
	}


	if(argc < 2)
	{
		exitWithUsage(cmd);
	}
	
	pageSize = sysconf(_SC_PAGE_SIZE);

	mode = modeStrToEnum(cmd);

	int long_index = 0;
	while ((opt = getopt_long(argc, argv, "sk", long_options, &long_index)) != -1)
	{

		switch (opt) {
			
			case 'k' : 
				{
					kernelspace = 1;
				}
				break;
			case 's' : 
				{
					swap = 1;
				}
				break;
			default:
				{
					printf("Error: Invalid switch\n");
					exitWithUsage(cmd);
				}
		}
	}

		if(mode  == _Undefined_)
			{
			printf ("Error: Invalid operation. \n");
			exitWithUsage(cmd);
		}

		addr = strtoul(argv[optind], endptr, 16);
		if(addr == 0)
		{
			printf("Error: Address %s is invalid\n", argv[optind]);
			exitWithUsage(cmd);
		}
		optind++;

#if defined (__mips__) 
		if ( (kernelspace == 1) && (((addr & 0xF0000000) != (0xB0000000)) && ((addr & 0xF0000000) != (0x80000000))) )
		{
			printf("when using -k (kernel) switch, address should start with 0xb or 0x8\n");
			exitWithUsage(cmd);
		}

		if ( (kernelspace == 0 ) && (((addr & 0xF0000000) == (0xB0000000)) || ((addr & 0xF0000000) == (0x80000000))) )

		{
			printf("when using address with 0xb or 0x8, kernel switch (-k) is mandatory\n");
			exitWithUsage(cmd);
		}
#endif

/* 
#if defined (__arm__) 
		if ( (kernelspace == 1 ) && ((addr & 0xFF000000) != 0xFC000000))
		{
			printf("when using -k (kernel) switch, address should start with 0xfc\n");
			exitWithUsage(cmd);
		}

		if ( (kernelspace == 0 ) && ((addr & 0xFF000000) == 0xFC000000))
		{
			printf("when using address with 0xfc, kernel switch (-k) is mandatory\n");
			exitWithUsage(cmd);
		}
#endif

*/
	
		if(isReadMode(mode))
		{
		if(optind < argc)
			{
			num = atoi(argv[optind]);
			}
		}

		if(isWriteMode(mode))
		{
		if(optind == argc)
			{
			printf("Error: Missing data to write\n");
			exitWithUsage(cmd);
			}

		while(optind < argc)
			{
				data[totNumData] = strtoul(argv[optind++], endptr, 16);
				totNumData++;
			
				if(totNumData >= MAX_DATA_ALLOWED)
					{
						printf("Max data write reached - allowed %d values\n", MAX_DATA_ALLOWED);
						exitWithUsage(cmd);
					}
			}
		}

		if(isFillMode(mode))
		{
		if(optind == argc)
			{
			printf("Error: Missing data to write\n");
			exitWithUsage(cmd);
			}
		else 
			{
				data[0] = strtoul(argv[optind++], endptr, 16);
			}
		if (argv[optind] == 0 )
			{
				printf("Error: Missing length argument.\n");
				exitWithUsage(cmd);
			} 
		else
			{
				totNumData = strtoul(argv[optind++], endptr, 10);
			}
		}


	unsigned int baseAddr =  addr & ~(pageSize - 1);


	if(kernelspace == 1)
		{

		if(isWriteMode(mode)) 
			{
				KernelModeWrite(addr, data, totNumData, mode, false, swap);
			}
		else if(isFillMode(mode))
			{
				KernelModeWrite(addr, data, totNumData, mode, true, swap);
			}
		else
			{
			KernelModeRead(addr, num, mode);
			}

		}
	else
		{
			if ((fd = open ("/dev/mem", O_RDWR|O_SYNC)) < 0)
				{
					printf ("Can't open /dev/mem for reading");
				}

			if ((baseLoc = mmap (0, num, PROT_READ|PROT_WRITE, MAP_SHARED, fd, baseAddr)) == MAP_FAILED)
				{
					printf("mmap error for given input:- phyaddr: %d, num: %d", addr, num);
				}

				switch(mode)
				{
				case _DumpWord_:
					{
					DumpWordsFromAddr(addr, baseAddr, (void *)baseLoc, num, swap);
					break;
					}
				case _DumpHalfWord_:
					{
					DumpHalfWordsFromAddr(addr, baseAddr, (void *)baseLoc, num, swap);
					break;
					}
				case _DumpByte_:
					{
					DumpBytesFromAddr(addr, baseAddr, (void *)baseLoc, num, swap);
					break;
					}
				case _SetWord_:
					{
					SetWordsFromAddr(addr, baseAddr, (void *)baseLoc, totNumData, data, false, swap);
					break;
					}
				case _SetHalfWord_:
					{
					SetHalfWordsFromAddr(addr, baseAddr, (void *)baseLoc, totNumData, data, false, swap);
					break;
					}
				case _SetByte_:
					{
					SetBytesFromAddr(addr, baseAddr, (void *)baseLoc, totNumData, data,false);
					break;
					}
				case _FillWord_:
					{
					SetWordsFromAddr(addr, baseAddr, (void *)baseLoc, totNumData, data, true, swap);
					break;
					}
				case _FillHalfWord_:
					{
					SetHalfWordsFromAddr(addr, baseAddr, (void *)baseLoc, totNumData, data, true, swap);
					break;
					}
				case _FillByte_:
					{
					SetBytesFromAddr(addr, baseAddr, (void *)baseLoc, totNumData, data,true);
					break;
					}
				default:
					printf("Should never get here\n");
					exit(-1);
					break;
				}
		
				if (munmap(0,num) == -1)
				{
					printf("unmap failed with error\n");	
				}

		close(fd);
		}

return 0;
}


