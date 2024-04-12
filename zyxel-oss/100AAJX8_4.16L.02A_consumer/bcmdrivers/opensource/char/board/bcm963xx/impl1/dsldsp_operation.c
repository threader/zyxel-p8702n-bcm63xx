/*
<:copyright-BRCM:2002:GPL/GPL:standard

   Copyright (c) 2002 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/

#include "board.h"
#include "dsldsp_operation.h"
#include <linux/fs.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <asm/uaccess.h>

int download_dsp_booter(void)
{
    struct file *fp;
    mm_segment_t fs;
    int i=0,j=0,len;
    char data;
    char string[512];
    int strLen=512;
    unsigned int booterSize=0;
    /*int dbgOn=10;*/

    unsigned short sprom_clk, sprom_data, sprom_reset;
    BpGetGpioSpromClk(&sprom_clk);
    BpGetGpioSpromData(&sprom_data);
    BpGetGpioSpromRst(&sprom_reset);

    printk("Loading booter into DSP\n");
    /*init GPIO, configure gpio as output*/
    kerSysSetGpioDir(sprom_reset);
    kerSysSetGpioDir(sprom_data);
    kerSysSetGpioDir(sprom_clk);
    /*initial data and clk state is high*/
    kerSysSetGpioState(sprom_data,kGpioActive);
    kerSysSetGpioState(sprom_clk,kGpioActive);
    /*warm reset dsp at first*/
    kerSysSetGpioState(sprom_reset,kGpioInactive);
    udelay(1);
    kerSysSetGpioState(sprom_reset,kGpioActive);
    udelay(1);
    /*read booter.bin, and write to DSP*/
    fp=filp_open("/etc/booter.bin",O_RDONLY,0);
    if(!IS_ERR(fp)&&fp->f_op&&fp->f_op->read)
    {
        printk("reading booter.bin\n");
        fs=get_fs();
        set_fs(get_ds());
        fp->f_pos=0;
        /*len=(int)fp->f_op->read(fp,(void *)string,strLen,&fp->f_pos);*/
        while((len=(int)fp->f_op->read(fp,(void *)string,strLen,&fp->f_pos))>0)
        {
            /*booterSize+=len;*/
            for(;len>0;len--)
            {
                data=string[j];
                /*if(dbgOn) {printk("===> %2x\n",(unsigned char)data); dbgOn--;}*/
                for(i=0;i<8;i++) /*bit in Byte*/
                {
                    /*output MSB bit at first*/
                    /*if(data&(0x1<<(7-i))) kerSysSetGpioState(sprom_data,kGpioActive);*/
                    /*reverse bit order*/
                    if(data&(0x1<<i)) kerSysSetGpioState(sprom_data,kGpioActive);
                    else kerSysSetGpioState(sprom_data,kGpioInactive);

                    udelay(1);	/*data setup time*/
                    /*clk falling edge,latch data*/
                    kerSysSetGpioState(sprom_clk,kGpioInactive);
                    udelay(1);
                    kerSysSetGpioState(sprom_clk,kGpioActive);	/*pull high clk*/
                    udelay(1);
                }
                j++;
                booterSize++;
            }
            j=0;
        }
        printk("DSP booter loading finished! booterSize=%d\n",booterSize);
        filp_close(fp,NULL);
        set_fs(fs);
    }
    else printk("open '%s' fail\n","booter.bin");

    kerSysSetGpioState(sprom_clk,kGpioInactive);
    return 0;
}
