/*
    Copyright 2000-2010 Broadcom Corporation

    <:label-BRCM:2011:DUAL/GPL:standard
    
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

/**************************************************************************
* File Name  : boardparms_voice.c
*
* Description: This file contains the implementation for the BCM63xx board
*              parameter voice access functions.
*
***************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "boardparms_voice.h"
#include "bp_defs.h"
#include <bcm_map_part.h>

#if !defined(_CFE_)
#include <linux/kernel.h>
#endif

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
static char voiceCurrentDgtrCardCfgId[BP_BOARD_ID_LEN] = VOICE_BOARD_ID_DEFAULT;
static int g_BpDectPopulated = 1;

/* ---- External Functions ------------------------------------------------ */
extern bp_elem_t * BpGetElem(enum bp_id id, bp_elem_t **pstartElem, enum bp_id stopAtId);
extern char *BpGetSubCp(enum bp_id id, bp_elem_t *pstartElem, enum bp_id stopAtId );
extern void *BpGetSubPtr(enum bp_id id, bp_elem_t *pstartElem, enum bp_id stopAtId );
extern unsigned char BpGetSubUc(enum bp_id id, bp_elem_t *pstartElem, enum bp_id stopAtId );
extern unsigned short BpGetSubUs(enum bp_id id, bp_elem_t *pstartElem, enum bp_id stopAtId );
extern unsigned long BpGetSubUl(enum bp_id id, bp_elem_t *pstartElem, enum bp_id stopAtId );

/* ---- Private Functions ------------------------------------------------ */
static void bpmemcpy( void* dstptr, const void* srcptr, int size );
static char * bpstrcpy( char* dest, const char* src );
static int bpstrlen( char * src );
static enum bp_id mapDcRstPinToBpType( BP_RESET_PIN rstPin );
static enum bp_id mapDcSpiDevIdToBpType( BP_SPI_SIGNAL spiId );
static bp_elem_t * BpGetVoiceBoardStartElemPtr( char * pszBaseBoardId );
static unsigned int BpGetZSISpiDevID( void );
static unsigned short BpGetSlaveSelectGpioNum( BP_SPI_PORT ssNum);
/* ---- Private Constants and Types -------------------------------------- */

/*
 * -------------------------- Voice Daughter Board Configs ------------------------------
 */

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88506 =
{
   VOICECFG_LE88506_STR,     /* szBoardId */
   2,             /* numFxsLines */
   0,             /* numFxoLines */
   {
      /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_88506,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */

         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      /* Always end the device list with BP_NULL_DEVICE */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};


VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32267 =
{
   VOICECFG_SI32267_STR,   /*Daughter board ID */
   2,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32267,
         BP_SPI_SS_NOT_REQUIRED,   /* ISI SPI CS handled internally. It is mapped by the zsiChipMap list. */
         BP_RESET_FXS1,
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,
   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI3217x =
{
   VOICECFG_SI3217X_STR,   /*Daughter Card ID */
   2,   /*Number of FXS Lines */
   1,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32176,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_SILABS_32178,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            }
         }
      },

      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

#ifdef MSTC_MODIFY
VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_VE890_INVBOOST =
{
   VOICECFG_VE890_INVBOOST_STR,   /* daughter card ID */
   3,   /* FXS number is 2 */
   0,   /* FXO number is 1 */
   {   /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_89156,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_89156,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            {
			   BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID			   
            }
         }
      },
	  /* voiceDevice2 parameters */
      {
         /* Device type */
         BP_VD_ZARLINK_89116_VIRTUAL,
         /* SPI control */
         /* SPI GPIO */
            BP_NOT_DEFINED,
         /* Reset pin */
         0,
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {  BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {  BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};
#else
VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_VE890_INVBOOST =
{
   VOICECFG_VE890_INVBOOST_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   1,   /* FXO number is 1 */
   {   /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_89116,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_89316,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};
#endif
VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260x2 =
{
   VOICECFG_SI32260x2_STR,   /*Daughter Card ID*/
   4,   /*Number of FXS Lines*/
   0,   /*Number of FXO Lines*/
   {   /* voiceDevice0 Parameters */
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2*/
         BP_VD_SILABS_32261,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260 =
{
   VOICECFG_SI32260_STR,   /*Daughter Card ID*/
   2,   /*Number of FXS Lines*/
   0,   /*Number of FXO Lines*/
   {   /* voiceDevice0 Parameters */
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_PMOS_BUCK_BOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260_SI3050 =
{
   VOICECFG_SI32260_SI3050_STR,   /*Daughter card ID */
   2,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_SILABS_3050,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on Si3050 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_PMOS_BUCK_BOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260_SI3050_QC =
{
   VOICECFG_SI32260_SI3050_QC_STR,   /*Daughter card ID */
   2,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_SILABS_3050,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on Si3050 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_QCUK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260_LCQC =
{
   VOICECFG_SI32260_LCQC_STR,   /*Daughter card ID */
   2,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_LCQCUK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE88536_ZSI =
{
   VOICECFG_LE88536_ZSI_STR,   /* Daughter Board ID */
   2,   /* Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {   /* Voice Device 0 Parameters */
      {
         BP_VD_ZARLINK_88536,   /* Device Type */
         BP_SPI_SS_NOT_REQUIRED,   /* ZSI SPI CS handled internally. It is mapped using the zsiMapList. */
         BP_RESET_FXS1,
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_ZL88601 =
{
   VOICECFG_ZL88601_STR,   /* szBoardId */
   2,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_88601,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_ZL88701 =
{
   VOICECFG_ZL88701_STR,   /* szBoardId */
   2,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_88701,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_89010,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B3 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* FXO reset pin tied with FXS on this board.*/
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on Le89010 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_ZL88702_ZSI =
{
   VOICECFG_ZL88702_ZSI_STR,   /* szBoardId */
   2,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* First 2 lines */
         /* Device Type */
         BP_VD_ZARLINK_88702_ZSI,
         BP_SPI_SS_NOT_REQUIRED,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9661_ZSI =
{
   VOICECFG_LE9661_ZSI_STR,   /* szBoardId */
   1,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_9661,
         BP_SPI_SS_NOT_REQUIRED,  /* ZSI SPI CS handled internally. It is mapped using the zsiMapList */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_ZL88801_89010_BB = /* for Microsemi ZLR88842L REV A0 DC */
{
   VOICECFG_ZL88801_89010_BB_STR,   /* szBoardId */
   2,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* First 2 lines */
         /* Device Type */
         BP_VD_ZARLINK_88801,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_89010,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* FXO reset pin tied with FXS on this board.*/
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* There is no second channel on Le89010 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW
   },
   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9662_ZSI =
{
   VOICECFG_LE9662_ZSI_STR,   /* szBoardId */
   2,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_9662,
         BP_SPI_SS_NOT_REQUIRED,  /* ZSI SPI CS handled internally. It is mapped using the zsiMapList */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9662_ZSI_BB =
{
   VOICECFG_LE9662_ZSI_BB_STR,   /* szBoardId */
   2,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_9662,
         BP_SPI_SS_NOT_REQUIRED,  /* ZSI SPI CS handled internally. It is mapped using the zsiMapList */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel Description */
         {
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_BUCKBOOST,
   /* General-purpose flags */
   ( BP_FLAG_ZSI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE89116 =
{
   VOICECFG_LE89116_STR,   /* Daughter Card ID */
   1,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_89116,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID},
            }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32176 =
{
   VOICECFG_SI32176_STR,   /* Daughter Board ID */
   1,   /* Number of FXS Lines */
   0,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32176,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_VE890HV =
{
   VOICECFG_VE890HV_STR,   /*Daughter Card ID */
   2,   /*Number of FXS Lines */
   1,   /*Number of FXO Lines */
   {
      {
         /* Device type */
         BP_VD_ZARLINK_89136,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* There is no second channel on 89116 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },
      {
         /* Device type 2 */
         BP_VD_ZARLINK_89336,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
         }
      },

      /* Always end the device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE89316 =
{
   VOICECFG_LE89316_STR,   /* Daughter Card ID */
   1,   /*Number of FXS Lines */
   1,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_ZARLINK_89316,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device 0 */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_INVBOOST,
   /* General-purpose flags */
   ( 0 )

};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32178 =
{
   VOICECFG_SI32178_STR,   /* Daughter Board ID */
   1,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32178,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },

   /* SLIC Device Profile */
    BP_VD_FLYBACK,
    /* General-purpose flags */
    ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_NOSLIC =
{
   VOICECFG_NOSLIC_STR, /*Daughter Board ID */
   0,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      BP_NULL_DEVICE_MACRO_NEW,
   },

   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32267_NTR =
{
   VOICECFG_SI32267_NTR_STR,   /* Daughter Board ID */
   2,   /*Number of FXS Lines */
   0,   /*Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32267,
         BP_SPI_SS_NOT_REQUIRED, /* ISI SPI CS handled internally. It is mapped by the zsiChipMap list. */
         BP_RESET_FXS1,          /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            },
         }
      },

      /* Always end the device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS_ISO,
   /* General-purpose flags */
   ( BP_FLAG_ISI_SUPPORT )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32260x2_SI3050 =
{
   VOICECFG_SI32260x2_SI3050_STR,   /*Daughter card ID */
   4,   /* Number of FXS Lines */
   1,   /* Number of FXO Lines */
   {
      {
         /* Device Type */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_SILABS_32261,
         BP_SPI_SS_B2,  /* Device uses SPI_SS_B2 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS2, /* Device uses FXS2 reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               2,
               2
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               3,
               3
            },
         }
      },
      {
         /* Device Type 3 */
         BP_VD_SILABS_3050,
         BP_SPI_SS_B3,  /* Device uses SPI_SS_B3 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXO,  /* Device uses FXO reset pin. Pin on base board depends on base board parameters. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_DAA,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               4,
               4
            },
            /* There is no second channel on Si3050 so mark it as inactive */
            {
               BP_VOICE_CHANNEL_INACTIVE,
               BP_VCTYPE_NONE,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_VD_FB_TSS,
   /* General-purpose flags */
   ( 0 )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9530 =
{
   VOICECFG_LE9530_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {  /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9530,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9530_WB =
{
   VOICECFG_LE9530_WB_STR,	/* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9530,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9540 =
{
   VOICECFG_LE9540_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {  /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9540,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9540_WB =
{
   VOICECFG_LE9540_WB_STR,	/* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9540,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9541 =
{
   VOICECFG_LE9541_STR,   /* daughter card ID */
   1,   /* FXS number is 1 */
   0,   /* FXO number is 0 */
   {  /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9541,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9541_WB =
{
   VOICECFG_LE9541_WB_STR,	/* daughter card ID */
   1,   /* FXS number is 1 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device Type */
         BP_VD_ZARLINK_9541,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_WIDEBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               BP_TIMESLOT_INVALID,
               BP_TIMESLOT_INVALID
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI3239 =
{
   VOICECFG_SI3239_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_3239,
         BP_SPI_SS_B1,           /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_SI32392 =
{
   VOICECFG_SI32392_STR,   /* daughter card ID */
   2,   /* FXS number is 2 */
   0,   /* FXO number is 0 */
   {   /* voiceDevice0 parameters */
      {
         /* Device type */
         BP_VD_SILABS_32392,
         BP_SPI_SS_NOT_REQUIRED,  /* Device doesn't use SPI_SS_Bx pin. Hard coded in board HAL. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         {
            /* Channel 0 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               0,
               0
            },
            /* Channel 1 on device */
            {
               BP_VOICE_CHANNEL_ACTIVE,
               BP_VCTYPE_SLIC,
               BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
               BP_VOICE_CHANNEL_NARROWBAND,
               BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
               BP_VOICE_CHANNEL_ENDIAN_BIG,
               1,
               1
            }
         }
      },

      /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },
   /* SLIC Device Profile */
   BP_NOT_DEFINED,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

VOICE_DAUGHTER_BOARD_PARMS g_voiceBoard_LE9530_LE88506 = {
   VOICECFG_LE9530_LE88506_STR,   /* Daughter board ID*/
   4,   /*Num FXS Lines */
   0,   /*Num FXO Lines */
   {   /*voiceDevice0 Params */
      {
         /* Device Type */
         BP_VD_ZARLINK_88506,
         BP_SPI_SS_B1,  /* Device uses SPI_SS_B1 pin. Pin on base board depends on base board parameters. */
         BP_RESET_FXS1, /* Device uses FXS1 reset pin. Pin on base board depends on base board parameters. */
         /* Channel description */
            {
               /* Channel 0 on device */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  0,
                  0
               },
               /* Test a single channel on 88506 */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  1,
                  1
               }
         }
      },
      {
         /* Device Type 2 */
         BP_VD_ZARLINK_9530,
         BP_SPI_SS_B1,           /*The 9530 chips are actually internal, device ID is always 0. */
         BP_RESET_NOT_REQUIRED,  /* Device does not require a reset pin. */
         /* Channel description */
            {
               /* Channel 0 on device */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  BP_TIMESLOT_INVALID,
                  BP_TIMESLOT_INVALID
               },
               /* Channel 1 on device */
               {
                  BP_VOICE_CHANNEL_ACTIVE,
                  BP_VCTYPE_SLIC,
                  BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
                  BP_VOICE_CHANNEL_NARROWBAND,
                  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
                  BP_VOICE_CHANNEL_ENDIAN_BIG,
                  BP_TIMESLOT_INVALID,
                  BP_TIMESLOT_INVALID
               }
            }
      },

     /* Always end device list with this macro. */
      BP_NULL_DEVICE_MACRO_NEW,
   },

   /* SLIC Device Profile */
   BP_VD_FLYBACK,
   /* General-purpose flags */
   ( BP_FLAG_DSP_APMHAL_ENABLE )
};

BP_VOICE_CHANNEL g_iDectStdCfg[BP_MAX_CHANNELS_PER_DEVICE*BP_MAX_DECT_DEVICE+1] = {						
   { 														
      BP_VOICE_CHANNEL_ACTIVE,					
      BP_VCTYPE_DECT,								
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		
      BP_VOICE_CHANNEL_WIDEBAND,					
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	
      BP_VOICE_CHANNEL_ENDIAN_BIG,				
      BP_TIMESLOT_INVALID,							
      BP_TIMESLOT_INVALID							
   },														
   {  													
      BP_VOICE_CHANNEL_ACTIVE,					
      BP_VCTYPE_DECT,								
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		
      BP_VOICE_CHANNEL_WIDEBAND,					
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	
      BP_VOICE_CHANNEL_ENDIAN_BIG,				
      BP_TIMESLOT_INVALID,							
      BP_TIMESLOT_INVALID							
   },														
   { 														
      BP_VOICE_CHANNEL_ACTIVE,					
      BP_VCTYPE_DECT,								
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		
      BP_VOICE_CHANNEL_WIDEBAND,					
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	
      BP_VOICE_CHANNEL_ENDIAN_BIG,				
      BP_TIMESLOT_INVALID,							
      BP_TIMESLOT_INVALID							
   },														
   {  													
      BP_VOICE_CHANNEL_ACTIVE,					
      BP_VCTYPE_DECT,								
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		
      BP_VOICE_CHANNEL_WIDEBAND,					
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	
      BP_VOICE_CHANNEL_ENDIAN_BIG,				
      BP_TIMESLOT_INVALID,							
      BP_TIMESLOT_INVALID							
   },														
			   											
   BP_NULL_CHANNEL_DESCRIPTION_MACRO			
};


/* End of Daughter Card Definitions */


/*
 * -------------------------- Voice Mother Board Configs ------------------------------
 */
#if defined(_BCM96328_) || defined(CONFIG_BCM96328)

static VOICE_DAUGHTER_BOARD_PARMS * g_96328avngr_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI32176,
   &g_voiceBoard_SI3217x,
   &g_voiceBoard_LE89116,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   0
};

static bp_elem_t g_voice_bcm96328avngr[] = {                 
   {bp_cpBoardId,               .u.cp = "96328avngr"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_6},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_29_AL},          
   {bp_daughterCardList,        .u.ptr = g_96328avngr_dCardList},
   {bp_last}                                                    
};                                                              
                                                                

static bp_elem_t * g_VoiceBoardParms[]=
{
   g_voice_bcm96328avngr,
   0
};

#endif


#if defined(_BCM96362_) || defined(CONFIG_BCM96362)


static VOICE_DAUGHTER_BOARD_PARMS * g_96362advngr2_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI3217x,
   &g_voiceBoard_SI32267,   
   &g_voiceBoard_LE88506,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   &g_voiceBoard_SI32260_SI3050_QC,
   &g_voiceBoard_ZL88601,
   0
};

static bp_elem_t g_voice_bcm96362advngr2[] = {                 
   {bp_cpBoardId,               .u.cp = "96362ADVNgr2"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_DECT},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_0},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_28_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_31_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_29_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_44_AH},          
   {bp_iDectCfg,                .u.ptr = g_iDectStdCfg},        
   {bp_daughterCardList,        .u.ptr = g_96362advngr2_dCardList},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm96362ravngr2[] = {                 
   {bp_cpBoardId,               .u.cp = "96362RAVNGR2"},           
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_35_AH},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm96362advngr2},
   {bp_last}                                                    
};                                                                                                                                                                                             

static bp_elem_t g_voice_bcm96362rpvt[] = {                 
   {bp_cpBoardId,               .u.cp = "96362RPVT"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_26_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_35_AH},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm96362advngr2},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm96362rpvt_2u[] = {                 
   {bp_cpBoardId,               .u.cp = "96362RPVT_2U"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm96362rpvt},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm96362advn2xh[] = {                 
   {bp_cpBoardId,               .u.cp = "96362ADVN2xh"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_26_AL},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm96362advngr2},
   {bp_last}                                                    
};                                                              
  
static bp_elem_t g_voice_bcm96362radvn2xh[] = {                 
   {bp_cpBoardId,               .u.cp = "96362RADVN2XH"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_26_AL},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm96362advngr2},
   {bp_last}                                                    
};                                                              
                                                              
static bp_elem_t * g_VoiceBoardParms[]=
{
   g_voice_bcm96362rpvt,
   g_voice_bcm96362rpvt_2u,
   g_voice_bcm96362advn2xh,
   g_voice_bcm96362ravngr2,
   g_voice_bcm96362advngr2,
   g_voice_bcm96362radvn2xh,
   0
};

#endif


#if defined(_BCM963268_) || defined(CONFIG_BCM963268)

static VOICE_DAUGHTER_BOARD_PARMS * g_963168_dCardList_full[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_VE890_INVBOOST,  
   &g_voiceBoard_LE88506,         
   &g_voiceBoard_LE89116,         
   &g_voiceBoard_SI3217x,         
   &g_voiceBoard_SI32267,         
   &g_voiceBoard_SI32260x2,       
   &g_voiceBoard_SI32260x2_SI3050,
   &g_voiceBoard_SI32260,         
   &g_voiceBoard_SI32260_SI3050,  
   &g_voiceBoard_SI32260_SI3050_QC,
   &g_voiceBoard_ZL88601,         
   &g_voiceBoard_ZL88701,         
   &g_voiceBoard_ZL88702_ZSI,     
   &g_voiceBoard_LE9662_ZSI,      
   &g_voiceBoard_LE9662_ZSI_BB, 
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_LE9661_ZSI,
   &g_voiceBoard_ZL88801_89010_BB,     
   0                              
};

static VOICE_DAUGHTER_BOARD_PARMS * g_963168_dCardList_noFxoRst[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_VE890_INVBOOST,  
   &g_voiceBoard_LE88506,         
   &g_voiceBoard_LE89116,         
   &g_voiceBoard_SI3217x,         
   &g_voiceBoard_SI32267,         
   &g_voiceBoard_SI32260x2,       
   &g_voiceBoard_SI32260,         
   &g_voiceBoard_SI32260_SI3050,  
   &g_voiceBoard_SI32260_SI3050_QC,
   &g_voiceBoard_ZL88601,         
   &g_voiceBoard_ZL88702_ZSI,     
   &g_voiceBoard_LE9662_ZSI,      
   &g_voiceBoard_LE9662_ZSI_BB, 
   &g_voiceBoard_LE88536_ZSI,     
   &g_voiceBoard_LE9661_ZSI,
   0                              
};
                                                                                                                                                                                                                                                                                                       
static bp_elem_t g_voice_bcm963168mbv_17a[] = {                                                         
   {bp_cpBoardId,               .u.cp = "963168MBV_17A"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_DECT},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_7},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_14_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_15_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_23_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_35_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_8_AH},          
   {bp_iDectCfg,                .u.ptr = g_iDectStdCfg},        
   {bp_daughterCardList,        .u.ptr = g_963168_dCardList_full},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963168mbv_30a[] = {                 
   {bp_cpBoardId,               .u.cp = "963168MBV_30A"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168mbv_17a},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963168mbv17a302[] = {                 
   {bp_cpBoardId,               .u.cp = "963168MBV17A302"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_10_AL},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168mbv_17a},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963168mbv30a302[] = {                 
   {bp_cpBoardId,               .u.cp = "963168MBV30A302"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168mbv17a302},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963168xh[] = {                 
   {bp_cpBoardId,               .u.cp = "963168XH"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_21_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_39_AH},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168mbv_17a},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963168xh5[] = {                 
   {bp_cpBoardId,               .u.cp = "963168XH5"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168xh},
   {bp_last}                                                    
};                                                              
         
static bp_elem_t g_voice_bcm963168mp[] = {                 
   {bp_cpBoardId,               .u.cp = "963168MP"},           
   {bp_usFxsFxoRst3,            .u.us = BP_NOT_DEFINED},          
   {bp_usDectRst,               .u.us = BP_GPIO_19_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_39_AH},   
   {bp_daughterCardList,        .u.ptr = g_963168_dCardList_noFxoRst},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168mbv_17a},
   {bp_last}                                                    
};                                                              
                                                       
static bp_elem_t g_voice_bcm963168mbv3[] = {                 
   {bp_cpBoardId,               .u.cp = "963168MBV3"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_DECT},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_5},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_14_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_15_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_23_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_35_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_12_AH},          
   {bp_iDectCfg,                .u.ptr = g_iDectStdCfg},        
   {bp_daughterCardList,        .u.ptr = g_963168_dCardList_full},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963268v30a[] = {                 
   {bp_cpBoardId,               .u.cp = "963268V30A"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_50_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_51_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_39_AH},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168mbv3},
   {bp_last}                                                    
};                       
                                       
static bp_elem_t g_voice_bcm963268bu[] = {                 
   {bp_cpBoardId,               .u.cp = "963268BU"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_18_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_19_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_39_AH},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168mbv3},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963268bu_p300[] = {                 
   {bp_cpBoardId,               .u.cp = "963268BU_P300"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963268bu},
   {bp_last}                                                    
};                                                                                                                                                                                             

static bp_elem_t g_voice_bcm963168vx[] = {                 
   {bp_cpBoardId,               .u.cp = "963168VX"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_0},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_5},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_13_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_13_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_NOT_DEFINED},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_NOT_DEFINED},          
   {bp_daughterCardList,        .u.ptr = g_963168_dCardList_full},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963168vx_p400[] = {                 
   {bp_cpBoardId,               .u.cp = "963168VX_P400"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168vx},
   {bp_last}                                                    
};                                                              
                                                                                                   
static bp_elem_t g_voice_bcm963168xn5[] = {                 
   {bp_cpBoardId,               .u.cp = "963168XN5"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_7},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_14_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_18_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_8_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_39_AH},          
   {bp_daughterCardList,        .u.ptr = g_963168_dCardList_full},
   {bp_last}                                                    
};                                                                                                                              

static bp_elem_t g_voice_bcm963168wfar[] = {                 
   {bp_cpBoardId,               .u.cp = "963168WFAR"},           
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_15_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_10_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_NOT_DEFINED},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168xn5},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963168ac5[] = {                 
   {bp_cpBoardId,               .u.cp = "963168AC5"},           
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_15_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_21_AL},          
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963168xn5},
   {bp_last}                                                    
}; 

static bp_elem_t g_voice_bcm963268sv1[] = {                 
   {bp_cpBoardId,               .u.cp = "963268SV1"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_DECT},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_3},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_14_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_15_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_35_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_8_AH},          
   {bp_iDectCfg,                .u.ptr = g_iDectStdCfg},        
   {bp_daughterCardList,        .u.ptr = g_963168_dCardList_noFxoRst},
   {bp_last}                                                    
};                                                                                                                         
                                                                
static bp_elem_t * g_VoiceBoardParms[]=
{
   g_voice_bcm963168mbv_17a,
   g_voice_bcm963168mbv_30a,
   g_voice_bcm963168mbv17a302,
   g_voice_bcm963168mbv30a302,
   g_voice_bcm963168xh,
   g_voice_bcm963168xh5,
   g_voice_bcm963168mp,
   g_voice_bcm963168mbv3,
   g_voice_bcm963268v30a,
   g_voice_bcm963268bu,
   g_voice_bcm963268bu_p300,
   g_voice_bcm963168vx,
   g_voice_bcm963168vx_p400,
   g_voice_bcm963168xn5,
   g_voice_bcm963168wfar,
   g_voice_bcm963168ac5,
   g_voice_bcm963268sv1,
   0
};

#endif


#if defined(_BCM96838_) || defined(CONFIG_BCM96838)

static VOICE_DAUGHTER_BOARD_PARMS * g_968380_dCardList_Le9540[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_LE9540,
   0
};

static VOICE_DAUGHTER_BOARD_PARMS * g_968380_dCardList_Si32392[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI32392,
   0
};

static bp_elem_t g_voice_bcm968380fhgu[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FHGU"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_APM},
   {bp_usZarIfEnable,           .u.us = BP_GPIO_0_AH}, 
   {bp_usZarIfSdin,             .u.us = BP_GPIO_1_AH}, 
   {bp_usZarIfSdout,            .u.us = BP_GPIO_2_AH}, 
   {bp_usZarIfSclk,             .u.us = BP_GPIO_3_AH}, 
   {bp_daughterCardList,        .u.ptr = g_968380_dCardList_Le9540},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380fehg[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FEHG"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380fggu[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FGGU"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380fegu[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FEGU"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};            
                                                  
static bp_elem_t g_voice_bcm968380gerg[] = {                 
   {bp_cpBoardId,               .u.cp = "968380GERG"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};     
                                                         
static bp_elem_t g_voice_bcm968380eprg[] = {                 
   {bp_cpBoardId,               .u.cp = "968380EPRG"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380ffhg[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FFHG"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380fesfu[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FESFU"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};                                                              
                                                                    
static bp_elem_t g_voice_bcm968385sfu[] = {                 
   {bp_cpBoardId,               .u.cp = "968385SFU"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968385esfu[] = {                 
   {bp_cpBoardId,               .u.cp = "968385ESFU"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fhgu},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm968380fhgu_si[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FHGU_SI"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_APM},
   {bp_usHvgMaxPwm,             .u.us = BP_GPIO_0_AH}, 
   {bp_usSi32392SpiSSNum,       .u.us = SPI_DEV_1},  
   {bp_daughterCardList,        .u.ptr = g_968380_dCardList_Si32392},
   {bp_last}                                                    
};                                                              
                                                                                                                                                                                            
static VOICE_DAUGHTER_BOARD_PARMS * g_968380fsv_g_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_LE9540,
   &g_voiceBoard_SI32392,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88702_ZSI,
   0
};

static bp_elem_t g_voice_bcm968380fsv_g[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FSV_G"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_APM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_6},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_7},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_5_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_6_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_7_AL},          
   {bp_usZarIfEnable,           .u.us = BP_GPIO_40_AH}, 
   {bp_usZarIfSdin,             .u.us = BP_GPIO_6_AH}, 
   {bp_usZarIfSdout,            .u.us = BP_GPIO_42_AH}, 
   {bp_usZarIfSclk,             .u.us = BP_GPIO_41_AH}, 
   {bp_usLe9540Reset,           .u.us = BP_GPIO_4_AL}, 
   {bp_usHvgMaxPwm,             .u.us = BP_GPIO_33_AH}, 
   {bp_usSi32392SpiSSNum,       .u.us = SPI_DEV_7},  
   {bp_daughterCardList,        .u.ptr = g_968380fsv_g_dCardList},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380fsv_e[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FSV_E"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fsv_g},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380fsv_ge[] = {                 
   {bp_cpBoardId,               .u.cp = "968380FSV_GE"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380fsv_g},
   {bp_last}                                                    
};                                                              
                                                                
                                                                
static VOICE_DAUGHTER_BOARD_PARMS * g_968380gerg_si_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI32392,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   &g_voiceBoard_SI32260_SI3050_QC,
   &g_voiceBoard_ZL88701,
   &g_voiceBoard_LE9661_ZSI,
   0
};

static bp_elem_t g_voice_bcm968380gerg_si[] = {                 
   {bp_cpBoardId,               .u.cp = "968380GERG_SI"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_APM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_0},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_36_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_40_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_38_AL},          
   {bp_usHvgMaxPwm,             .u.us = BP_GPIO_0_AH}, 
   {bp_usSi32392SpiSSNum,       .u.us = SPI_DEV_1},  
   {bp_daughterCardList,        .u.ptr = g_968380gerg_si_dCardList},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380sv_g[] = {                 
   {bp_cpBoardId,               .u.cp = "968380SV_G"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_APM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_6},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_7},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_5_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_38_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_7_AL},          
   {bp_usZarIfEnable,           .u.us = BP_GPIO_40_AH}, 
   {bp_usZarIfSdin,             .u.us = BP_GPIO_6_AH}, 
   {bp_usZarIfSdout,            .u.us = BP_GPIO_42_AH}, 
   {bp_usZarIfSclk,             .u.us = BP_GPIO_41_AH}, 
   {bp_daughterCardList,        .u.ptr = g_968380_dCardList_Le9540},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968380sv_e[] = {                 
   {bp_cpBoardId,               .u.cp = "968380SV_E"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968380sv_g},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm968385sfu_si[] = {                 
   {bp_cpBoardId,               .u.cp = "968385SFU_SI"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_APM},
   {bp_usHvgMaxPwm,             .u.us = BP_GPIO_0_AH}, 
   {bp_usSi32392SpiSSNum,       .u.us = SPI_DEV_1},  
   {bp_daughterCardList,        .u.ptr = g_968380_dCardList_Si32392},
   {bp_last}                                                    
};                                                              
                                                                
static VOICE_DAUGHTER_BOARD_PARMS * g_968385sv_g_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_LE9540,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32392,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88701,
   &g_voiceBoard_ZL88702_ZSI,
   &g_voiceBoard_LE9662_ZSI_BB,
   &g_voiceBoard_LE9661_ZSI,
   0
};

static bp_elem_t g_voice_bcm968385sv_g[] = {                 
   {bp_cpBoardId,               .u.cp = "968385SV_G"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_APM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_6},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_6},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_5_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_6_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_7_AL},          
   {bp_usZarIfEnable,           .u.us = BP_GPIO_40_AH}, 
   {bp_usZarIfSdin,             .u.us = BP_GPIO_6_AH}, 
   {bp_usZarIfSdout,            .u.us = BP_GPIO_42_AH}, 
   {bp_usZarIfSclk,             .u.us = BP_GPIO_41_AH}, 
   {bp_daughterCardList,        .u.ptr = g_968385sv_g_dCardList},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm968385sv_e[] = {                 
   {bp_cpBoardId,               .u.cp = "968385SV_E"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm968385sv_g},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t * g_VoiceBoardParms[]=
{
   g_voice_bcm968380fhgu,
   g_voice_bcm968380fhgu_si,
   g_voice_bcm968380fehg,
   g_voice_bcm968380fggu,
   g_voice_bcm968380fegu,
   g_voice_bcm968380fsv_g,
   g_voice_bcm968380fsv_e,
   g_voice_bcm968380fsv_ge,
   g_voice_bcm968380gerg,
   g_voice_bcm968380gerg_si,
   g_voice_bcm968380eprg,
   g_voice_bcm968380ffhg,
   g_voice_bcm968380fesfu,
   g_voice_bcm968380sv_g,
   g_voice_bcm968380sv_e,
   g_voice_bcm968385sfu,
   g_voice_bcm968385sfu_si,
   g_voice_bcm968385esfu,
   g_voice_bcm968385sv_g,
   g_voice_bcm968385sv_e,
   0
};

#endif


#if defined(_BCM963138_) || defined(CONFIG_BCM963138)

static VOICE_DAUGHTER_BOARD_PARMS * g_963138_dCardListFull[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI3217x,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88701,
   &g_voiceBoard_ZL88702_ZSI,
   &g_voiceBoard_SI32260x2,
   &g_voiceBoard_LE9662_ZSI,
   &g_voiceBoard_LE9662_ZSI_BB,
   &g_voiceBoard_LE9661_ZSI,
   &g_voiceBoard_ZL88801_89010_BB,
   &g_voiceBoard_SI32260x2_SI3050,   
   0
};

static VOICE_DAUGHTER_BOARD_PARMS * g_963138_dCardListNoFxoRst[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI3217x,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88701,
   &g_voiceBoard_ZL88702_ZSI,
   &g_voiceBoard_SI32260x2,
   &g_voiceBoard_LE9662_ZSI,
   &g_voiceBoard_LE9662_ZSI_BB,
   &g_voiceBoard_LE9661_ZSI,
   &g_voiceBoard_ZL88801_89010_BB,
   0
};


static bp_elem_t g_voice_bcm963138dvt[] = {                 
   {bp_cpBoardId,               .u.cp = "963138DVT"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_DECT},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_7_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_19_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_36_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_37_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_6_AH},          
   {bp_iDectCfg,                .u.ptr = g_iDectStdCfg},        
   {bp_daughterCardList,        .u.ptr = g_963138_dCardListFull},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963138dvt_p300[] = {                 
   {bp_cpBoardId,               .u.cp = "963138DVT_P300"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_11_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_12_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_26_AH},    
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963138dvt},      
   {bp_last}                                                    
};                                                                
                                                                
static bp_elem_t g_voice_bcm963138ref_bmu[] = {                 
   {bp_cpBoardId,               .u.cp = "963138REF_BMU"},           
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_60_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_61_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_NOT_DEFINED},          
   {bp_usDectRst,               .u.us = BP_GPIO_62_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_NOT_DEFINED},          
   {bp_daughterCardList,        .u.ptr = g_963138_dCardListNoFxoRst},
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963138dvt},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963138bmu_p202[] = {                 
   {bp_cpBoardId,               .u.cp = "963138BMU_P202"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963138ref_bmu},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963138ref[] = {                 
   {bp_cpBoardId,               .u.cp = "963138REF"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_7_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_117_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_116_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_6_AH},          
   {bp_daughterCardList,        .u.ptr = g_963138_dCardListFull},
   {bp_last}                                                    
};                                                              
                                                                

static bp_elem_t g_voice_bcm963138ref_p402[] = {                 
   {bp_cpBoardId,               .u.cp = "963138REF_P402"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_5},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_4_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_5_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_6_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_3_AH},          
   {bp_daughterCardList,        .u.ptr = g_963138_dCardListFull},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963138ref_p502[] = {                 
   {bp_cpBoardId,               .u.cp = "963138REF_P502"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963138ref_p402},
   {bp_last}                                                    
};                                                              
                                                                                                                               
static VOICE_DAUGHTER_BOARD_PARMS * g_963138ref_lte_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_LCQC,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88702_ZSI,
   &g_voiceBoard_LE9662_ZSI,
   &g_voiceBoard_LE9662_ZSI_BB,
   &g_voiceBoard_LE9661_ZSI,
   &g_voiceBoard_LE89116,
   0
};

static bp_elem_t g_voice_bcm963138ref_lte[] = {                 
   {bp_cpBoardId,               .u.cp = "963138REF_LTE"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_34_AL},          
   {bp_daughterCardList,        .u.ptr = g_963138ref_lte_dCardList},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963138lte_p302[] = {                 
   {bp_cpBoardId,               .u.cp = "963138LTE_P302"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963138ref_lte},
   {bp_last}                                                    
};                                                              

static bp_elem_t * g_VoiceBoardParms[]=
{
   g_voice_bcm963138dvt,
   g_voice_bcm963138dvt_p300,
   g_voice_bcm963138ref_bmu,
   g_voice_bcm963138bmu_p202,
   g_voice_bcm963138ref,
   g_voice_bcm963138ref_p402,
   g_voice_bcm963138ref_p502,
   g_voice_bcm963138ref_lte,
   g_voice_bcm963138lte_p302,
   0
};

#endif


#if defined(_BCM963148_) || defined(CONFIG_BCM963148)

static VOICE_DAUGHTER_BOARD_PARMS * g_963148_dCardListFull[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI3217x,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88701,
   &g_voiceBoard_ZL88702_ZSI,
   &g_voiceBoard_ZL88801_89010_BB,
   0
};

static VOICE_DAUGHTER_BOARD_PARMS * g_963148_dCardListFullNoFxoRst[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI3217x,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   &g_voiceBoard_SI32260_SI3050_QC,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88702_ZSI,
   0
};

static bp_elem_t g_voice_bcm963148dvt[] = {                 
   {bp_cpBoardId,               .u.cp = "963148DVT"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_DECT},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_7_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_19_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_36_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_37_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_6_AH},          
   {bp_iDectCfg,                .u.ptr = g_iDectStdCfg},        
   {bp_daughterCardList,        .u.ptr = g_963148_dCardListFull},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963148dvt_p300[] = {                 
   {bp_cpBoardId,               .u.cp = "963148DVT_P300"},           
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_11_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_12_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_26_AH}, 
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963148dvt},              
   {bp_last}                                                    
};                                                                   

static bp_elem_t g_voice_bcm963148sv[] = {                 
   {bp_cpBoardId,               .u.cp = "963148SV"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963148dvt},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963148ref[] = {                 
   {bp_cpBoardId,               .u.cp = "963148REF"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_5},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_4_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_5_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_6_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_3_AH},          
   {bp_daughterCardList,        .u.ptr = g_963148_dCardListFull},
   {bp_last}                                                    
};                                                              

static bp_elem_t g_voice_bcm963148ref_bmu[] = {                 
   {bp_cpBoardId,               .u.cp = "963148REF_BMU"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_DECT},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxo3SpiSSNum,       .u.us = SPI_DEV_4},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_60_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_61_AL},          
   {bp_usDectRst,               .u.us = BP_GPIO_62_AL},          
   {bp_iDectCfg,                .u.ptr = g_iDectStdCfg},        
   {bp_daughterCardList,        .u.ptr = g_963148_dCardListFullNoFxoRst},
   {bp_last}                                                    
};                                                              
                                                                

static bp_elem_t * g_VoiceBoardParms[]=
{
   g_voice_bcm963148dvt,
   g_voice_bcm963148dvt_p300,
   g_voice_bcm963148sv,
   g_voice_bcm963148ref,
   g_voice_bcm963148ref_bmu,
   0
};

#endif


#if defined(_BCM963381_) || defined(CONFIG_BCM963381)

static VOICE_DAUGHTER_BOARD_PARMS * g_963381ref1_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_LE9662_ZSI_BB,
   &g_voiceBoard_LE9661_ZSI,
   0
};

static bp_elem_t g_voice_bcm963381ref1[] = {                 
   {bp_cpBoardId,               .u.cp = "963381REF1"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_22_AL},          
   {bp_daughterCardList,        .u.ptr = g_963381ref1_dCardList},
   {bp_last}                                                    
};                                                              
                                                                
static bp_elem_t g_voice_bcm963381ref1_a0[] = {                 
   {bp_cpBoardId,               .u.cp = "963381REF1_A0"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963381ref1},
   {bp_last}                                                    
};                                                              
                                                                
static VOICE_DAUGHTER_BOARD_PARMS * g_963381dvt_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_LE9662_ZSI_BB,
   &g_voiceBoard_LE9661_ZSI,
   &g_voiceBoard_ZL88702_ZSI,
   0
};

static bp_elem_t g_voice_bcm963381dvt[] = {                 
   {bp_cpBoardId,               .u.cp = "963381DVT"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_3},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_22_AL},          
   {bp_usFxsFxoRst3,            .u.us = BP_GPIO_14_AL},          
   {bp_usVoipRelayCtrl1,        .u.us = BP_GPIO_23_AH},          
   {bp_daughterCardList,        .u.ptr = g_963381dvt_dCardList},
   {bp_last}                                                    
};                                                              
                                                                
static VOICE_DAUGHTER_BOARD_PARMS * g_963381sv_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI3217x,
   &g_voiceBoard_VE890_INVBOOST,
   &g_voiceBoard_LE88506,
   &g_voiceBoard_SI32267,
   &g_voiceBoard_LE88536_ZSI,
   &g_voiceBoard_SI32260,
   &g_voiceBoard_SI32260_SI3050,
   &g_voiceBoard_ZL88601,
   &g_voiceBoard_ZL88702_ZSI,
   &g_voiceBoard_LE9662_ZSI_BB,
   &g_voiceBoard_LE9661_ZSI,
   0
};

static bp_elem_t g_voice_bcm963381sv[] = {                 
   {bp_cpBoardId,               .u.cp = "963381SV"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxo2SpiSSNum,       .u.us = SPI_DEV_2},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_22_AL},          
   {bp_usFxsFxoRst2,            .u.us = BP_GPIO_23_AL},          
   {bp_daughterCardList,        .u.ptr = g_963381sv_dCardList},
   {bp_last}                                                    
};                                                              
                                                                

static VOICE_DAUGHTER_BOARD_PARMS * g_963381a_ref1_dCardList[] = {
   &g_voiceBoard_NOSLIC,
   &g_voiceBoard_SI32260_LCQC,
   0
};

static bp_elem_t g_voice_bcm963381a_ref1[] = {                 
   {bp_cpBoardId,               .u.cp = "963381A_REF1"},           
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_HS_SPI},
   {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_PCM},
   {bp_usFxsFxo1SpiSSNum,       .u.us = SPI_DEV_1},             
   {bp_usFxsFxoRst1,            .u.us = BP_GPIO_22_AL},          
   {bp_daughterCardList,        .u.ptr = g_963381a_ref1_dCardList},
   {bp_last}                                                    
};                                                              
  
static bp_elem_t g_voice_bcm963381ref2[] = {                 
   {bp_cpBoardId,               .u.cp = "963381REF2"},           
   {bp_elemTemplate,            .u.bp_elemp = g_voice_bcm963381a_ref1},
   {bp_last}                                                    
};                                                              
                                                              

static bp_elem_t * g_VoiceBoardParms[]=
{
   g_voice_bcm963381ref1,
   g_voice_bcm963381ref1_a0,
   g_voice_bcm963381dvt,
   g_voice_bcm963381sv,
   g_voice_bcm963381a_ref1,
   g_voice_bcm963381ref2,
   0
};

#endif

#if !defined(_BCM963268_) && !defined(CONFIG_BCM963268) && !defined(_BCM96362_) && !defined(CONFIG_BCM96362) && !defined(_BCM96328_) && !defined(CONFIG_BCM96328) && !defined(_BCM96838_) && !defined(CONFIG_BCM96838) && !defined(_BCM963138_) && !defined(CONFIG_BCM963138) && !defined(_BCM963381_) && !defined(CONFIG_BCM963381) && !defined(_BCM963148_) && !defined(CONFIG_BCM963148)

static bp_elem_t * g_VoiceBoardParms[]=
{
   0
};

#endif
/* Voice Boardparams End */


/*****************************************************************************
*
*  String and memory manipulation private functions
*
******************************************************************************/
static void bpmemcpy( void* dstptr, const void* srcptr, int size )
{
   char* dstp = dstptr;
   const char* srcp = srcptr;
   int i;
   for( i=0; i < size; i++ )
   {
      *dstp++ = *srcp++;
   }
}

static char * bpstrcpy( char* dest, const char* src)
{
   while(*src)
   {
      *dest++ = *src++;
   }

   *dest = '\0';

   return dest;
}

static int bpstrlen( char * src )
{
   char *s;

	for(s = src; (s != 0) && (*s != 0); ++s);
	
	return(s - src);
}

   
/*****************************************************************************
*
*  voice Daughtercard type to BoardParam Type mapping functions
*
******************************************************************************/
static enum bp_id mapDcRstPinToBpType( BP_RESET_PIN rstPin )
{
   return( bp_usFxsFxoRst1 + (enum bp_id)( rstPin - BP_RESET_FXS1 ) );
}

static enum bp_id mapDcSpiDevIdToBpType( BP_SPI_SIGNAL spiId )
{
   return( bp_usFxsFxo1SpiSSNum + (enum bp_id)( spiId - BP_SPI_SS_B1 ) );
}

/*****************************************************************************
 * Name:          BpGetZSISpiDevID()
 *
 * Description:     This function returns the SPI Device ID for the ZSI daughter
*                   boards based on the current chip.
 *
 * Parameters:    Nothing
 *
 * Returns:       SPI Dev ID for ZSI Daughter Boards
 *
 *****************************************************************************/
static unsigned int BpGetZSISpiDevID( void )
{
#ifdef ZSI_SPI_DEV_ID
   return ZSI_SPI_DEV_ID;
#else
   return BP_NOT_DEFINED;
#endif
}

/*****************************************************************************
 * Name:          BpSetDectPopulatedData()
 *
 * Description:     This function sets the g_BpDectPopulated variable. It is
 *                used for the user to specify in the board parameters if the
 *                board DECT is populated or not (1 for populated, 0 for not).
 *
 * Parameters:    int BpData - The data that g_BpDectPopulated will be set to.
 *
 * Returns:       Nothing
 *
 *****************************************************************************/
void BpSetDectPopulatedData( int BpData )
{
   g_BpDectPopulated = BpData;
}

/*****************************************************************************
 * Name: 	      BpDectPopulated()
 *
 * Description:	  This function is used to determine if DECT is populated on
 * 				  the board.
 *
 * Parameters:    None
 *
 * Returns:       BP_DECT_POPULATED if DECT is populated, otherwise it will
 *                return BP_DECT_NOT_POPULATED.
 *
 *****************************************************************************/
int BpDectPopulated( void )
{
   return (g_BpDectPopulated ? BP_DECT_POPULATED:BP_DECT_NOT_POPULATED);
}

/*****************************************************************************
 * Name:          BpGetVoiceParms()
 *
 * Description:     Finds the voice parameters based on the daughter board and
 *                base board IDs and fills the old parameters structure with
 *                information.
 *
 * Parameters:    pszVoiceDaughterCardId - The daughter board ID that is being used.
 *                voiceParms - The old voice parameters structure that must be
 *                             filled with data from the new structure.
 *                pszBaseBoardId - The base board ID that is being used.
 *
 * Returns:       If the board is not found, returns BP_BOARD_ID_NOT_FOUND.
 *                If everything goes properly, returns BP_SUCCESS.
 *
 *****************************************************************************/
int BpGetVoiceParms( char* pszVoiceDaughterCardId, VOICE_BOARD_PARMS* voiceParms, char* pszBaseBoardId )
{
   int nRet = BP_BOARD_ID_NOT_FOUND;
   int i = 0;
   int nDeviceCount = 0;
   bp_elem_t * pBpStartElem;
   PVOICE_DAUGHTER_BOARD_PARMS *ppDc;
   BP_VOICE_CHANNEL * dectChanCfg = 0;   
   VOICE_BOARD_PARMS currentVoiceParms;
   
   /* Get start element of voice board params structure */
   if( !(pBpStartElem = BpGetVoiceBoardStartElemPtr(pszBaseBoardId)) )
   {
      /* No matching base board found */
      return nRet;     
   }

   /* Get dectcfg pointer */
   if( BpDectPopulated() == BP_DECT_POPULATED )
   {
      dectChanCfg = (BP_VOICE_CHANNEL *)BpGetSubPtr(bp_iDectCfg, pBpStartElem, bp_last); 
   }
      
   /* Get daughtercard list pointer */
   ppDc = (PVOICE_DAUGHTER_BOARD_PARMS *)BpGetSubPtr(bp_daughterCardList, pBpStartElem, bp_last);     
   if( !ppDc ) 
   {
      /* No matching daughtercard list was found */
      return nRet;      
   }
   
   /* Iterate through daughter card list */
   for(; *ppDc; ppDc++)
   {
      if( (0 == bpstrcmp((*ppDc)->szBoardId, pszVoiceDaughterCardId)))
      {
         nRet = BP_SUCCESS;
         
         /* Succesfully found base board + daughter card combination
          * Must now fill the currentVoiceParms structure with data and copy to voiceParms
          * First set base board and daughter board strings */
         bpmemcpy(currentVoiceParms.szBoardId, pszVoiceDaughterCardId, bpstrlen(pszVoiceDaughterCardId));
         bpmemcpy(currentVoiceParms.szBaseBoardId, pszBaseBoardId, bpstrlen(pszBaseBoardId));

         /* Set the FXS and FXO line numbers. */
         currentVoiceParms.numFxsLines = (*ppDc)->numFxsLines;
         currentVoiceParms.numFxoLines = (*ppDc)->numFxoLines;

         /* Set the number of DECT Lines. */  
         currentVoiceParms.numDectLines = 0;
         
         if( dectChanCfg )
         {  
            for( ; dectChanCfg[currentVoiceParms.numDectLines].status != BP_VOICE_CHANNEL_NONE; 
                 currentVoiceParms.numDectLines++)
            { }                            
         }
         
         /*This prevents the total number of channels from being greater than 7. */
         if(currentVoiceParms.numFxsLines + currentVoiceParms.numFxoLines + currentVoiceParms.numDectLines > 7)
         {
            if(currentVoiceParms.numDectLines == 4)
            {
               /* If there are four DECT lines and it is exceeding limit, can
                * cut two of the DECT lines for board/daughter card combinations
                * with 4 FXS lines such as 963268V30A with Si32260x2.*/
               currentVoiceParms.numDectLines = 2;
            }
            else
            {
               return BP_MAX_CHANNELS_EXCEEDED; /* Return a failure. */
            }
         }

         /*Set the relay GPIO pins*/
         currentVoiceParms.pstnRelayCtrl.relayGpio[0] = BpGetSubUs(bp_usVoipRelayCtrl1, pBpStartElem, bp_last);
         currentVoiceParms.pstnRelayCtrl.relayGpio[1] = BpGetSubUs(bp_usVoipRelayCtrl2, pBpStartElem, bp_last);
         for( i = 0, currentVoiceParms.numFailoverRelayPins = 0; i < BP_MAX_RELAY_PINS; i++ )             
         {
            if(currentVoiceParms.pstnRelayCtrl.relayGpio[i] != BP_NOT_DEFINED)
            {
               currentVoiceParms.numFailoverRelayPins++;
            }
         }

         /*Set DECT UART to Not Defined always for now. */
         currentVoiceParms.dectUartControl.dectUartGpioTx = BP_NOT_DEFINED;
         currentVoiceParms.dectUartControl.dectUartGpioRx = BP_NOT_DEFINED;

         /* Set the device profile */
         currentVoiceParms.deviceProfile = (*ppDc)->deviceProfile;

         /*Set the flags*/
         currentVoiceParms.flags = (*ppDc)->flags;

         /*Set DECT*/
         if(currentVoiceParms.numDectLines)
         {
            currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType = 
                  currentVoiceParms.voiceDevice[nDeviceCount+1].voiceDeviceType = BP_VD_IDECT1;
            currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = 
                  currentVoiceParms.voiceDevice[nDeviceCount+1].spiCtrl.spiDevId = 0;
            currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = 
                  currentVoiceParms.voiceDevice[nDeviceCount+1].spiCtrl.spiGpio = BP_NOT_DEFINED;
            currentVoiceParms.voiceDevice[nDeviceCount].requiresReset = 
                  currentVoiceParms.voiceDevice[nDeviceCount+1].requiresReset = 1;
            currentVoiceParms.voiceDevice[nDeviceCount].resetGpio = 
                  currentVoiceParms.voiceDevice[nDeviceCount+1].resetGpio = BpGetSubUs(bp_usDectRst, pBpStartElem, bp_last);

            switch(currentVoiceParms.numDectLines)
            {
               case 1:
               case 2:
               {
                  bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount].channel[nDeviceCount],
                           &dectChanCfg[0],
                           sizeof(BP_VOICE_CHANNEL)*currentVoiceParms.numDectLines);
                  nDeviceCount++;
               }
               break;

               case 3:
               case 4:
               {
                  bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount].channel[nDeviceCount], 
                           &dectChanCfg[0], sizeof(BP_VOICE_CHANNEL)*2);
                  bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount+1].channel[nDeviceCount],
                           &dectChanCfg[2],sizeof(BP_VOICE_CHANNEL)*2);
                  nDeviceCount+=2;
               }
               break;
               default:
               {
                  /* Return a failure */
                  return BP_MAX_CHANNELS_EXCEEDED;
               }
               break;
            }
         }

         for( i = 0; (i < BP_MAX_VOICE_DEVICES && (*ppDc)->voiceDevice[i].nDeviceType != BP_VD_NONE); i++ )
         {
            /* Loop through the voice devices and copy to currentVoiceParms */

            currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType = (*ppDc)->voiceDevice[i].nDeviceType;

            if( (*ppDc)->voiceDevice[i].nRstPin == BP_RESET_NOT_REQUIRED )                
            {
               currentVoiceParms.voiceDevice[nDeviceCount].requiresReset = 0;
               currentVoiceParms.voiceDevice[nDeviceCount].resetGpio = BP_NOT_DEFINED;
            }
            else
            {
               currentVoiceParms.voiceDevice[nDeviceCount].requiresReset = 1;
               
               /* Retrieve the Reset GPIO */
               if( BpGetSubUs( mapDcRstPinToBpType((*ppDc)->voiceDevice[i].nRstPin), pBpStartElem, bp_last ) != BP_NOT_DEFINED )
               {
                  currentVoiceParms.voiceDevice[nDeviceCount].resetGpio = 
                                          BpGetSubUs( mapDcRstPinToBpType((*ppDc)->voiceDevice[i].nRstPin), pBpStartElem, bp_last );
               }
               else
               {
                  currentVoiceParms.voiceDevice[nDeviceCount].resetGpio = BP_NOT_DEFINED;
               }
            }

            /* Handle the ZSI/ISI devices */
            if((*ppDc)->voiceDevice[i].nSPI_SS_Bx == BP_SPI_SS_NOT_REQUIRED)
            {
               /* Current device is a ZSI/ISI device. */
               currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = BpGetZSISpiDevID();

               if(currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId == BP_NOT_DEFINED)
               {
                  /* Failure - Tried to use a ZSI/ISI chip on a board which does not support it*/
                  return BP_NO_ZSI_ON_BOARD_ERR;
               }

               currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = BP_NOT_DEFINED;
            }
            else
            {
               /* Retrieve the system SPI device ID */
               if( BpGetSubUs(mapDcSpiDevIdToBpType((*ppDc)->voiceDevice[i].nSPI_SS_Bx), pBpStartElem, bp_last) != BP_NOT_DEFINED )
               {
                  /* Assign system SPI device ID */
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = BpGetSubUs(mapDcSpiDevIdToBpType((*ppDc)->voiceDevice[i].nSPI_SS_Bx),
                                                                                 pBpStartElem, bp_last);                                                                                 
               }
               else
               {
                  /* SPI ID not defined */
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = BP_NOT_DEFINED;                                                                                
               }
               
               /* Assign SPI associated GPIO pin */
               currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio  = BpGetSlaveSelectGpioNum( currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId );    
                                                                              
                                                                                         
            }

            /* Handle Le9530 and Si3239, which are internal devices */
            switch(currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType)
            {
               case BP_VD_ZARLINK_9530:
               case BP_VD_ZARLINK_9540:
               case BP_VD_ZARLINK_9541:
               {
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = 0;
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = BP_NOT_DEFINED;
               }
               break;
               case BP_VD_SILABS_3239:
               case BP_VD_SILABS_32392:
               {
                  /* FIXME - Add SPI retrieval function for SI3239X in boardhal code */
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId = BpGetSubUs(bp_usSi32392SpiSSNum, pBpStartElem, bp_last);
                  currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiGpio = BpGetSlaveSelectGpioNum( currentVoiceParms.voiceDevice[nDeviceCount].spiCtrl.spiDevId );
               }
               default:
               break;
            }

            bpmemcpy(&currentVoiceParms.voiceDevice[nDeviceCount].channel[0], &((*ppDc)->voiceDevice[i].channel[0]), sizeof(BP_VOICE_CHANNEL)*2);

            nDeviceCount++;
         }

         /*Add a NULL Device*/
         currentVoiceParms.voiceDevice[nDeviceCount].voiceDeviceType=BP_VD_NONE;

         /* Copy over params */
         bpmemcpy( voiceParms, &currentVoiceParms, sizeof(VOICE_BOARD_PARMS) );
         nRet = BP_SUCCESS;

         break;
      }
   }

   return( nRet );
}

/**************************************************************************
* Name       : BpSetVoiceBoardId
*
* Description: This function find the BOARD_PARAMETERS structure for the
*              specified board id string and assigns it to a global, static
*              variable.
*
* Parameters : [IN] pszVoiceDaughterCardId - Board id string that is saved into NVRAM.
*
* Returns    : BP_SUCCESS - Success, value is returned.
*              BP_BOARD_ID_NOT_FOUND - Error, board id input string does not
*                  have a board parameters configuration record.
***************************************************************************/
int BpSetVoiceBoardId( char *pszVoiceDaughterCardId )
{
   bp_elem_t * pBpStartElem;
   int nRet = BP_BOARD_ID_NOT_FOUND;
   PVOICE_DAUGHTER_BOARD_PARMS *ppDc;
  
   /* Get start element of voice board params structure - Pass 0 to retrieve base board id in utility function */
   if( !(pBpStartElem = BpGetVoiceBoardStartElemPtr(0)) )
   {
      /* No matching base board found */
      return nRet;     
   }
      
   /* Get daughtercard list pointer */
   ppDc = (PVOICE_DAUGHTER_BOARD_PARMS *)BpGetSubPtr(bp_daughterCardList, pBpStartElem, bp_last);     
   if( !ppDc ) 
   {
      /* No matching daughtercard list was found */
      return nRet;      
   }
   
   /* Iterate through daughter card list */
   for(; *ppDc; ppDc++)
   {
      if( (0 == bpstrcmp((*ppDc)->szBoardId, pszVoiceDaughterCardId)))
      {
         bpmemcpy(voiceCurrentDgtrCardCfgId,pszVoiceDaughterCardId,BP_BOARD_ID_LEN);
         nRet = BP_SUCCESS;   
         return nRet;
      }
   }
   
   return( nRet );
} /* BpSetVoiceBoardId */


/**************************************************************************
* Name       : BpGetVoiceBoardId
*
* Description: This function returns the current board id strings.
*
* Parameters : [OUT] pszVoiceDaughterCardId - Address of a buffer that the board id
*                  string is returned in.
*
* Returns    : BP_SUCCESS - Success, value is returned.
***************************************************************************/
int BpGetVoiceBoardId( char *pszVoiceDaughterCardId )
{
   int i;

   if (0 == bpstrcmp(voiceCurrentDgtrCardCfgId, VOICE_BOARD_ID_DEFAULT))
   {
      return -1;
   }

   for (i = 0; i < BP_BOARD_ID_LEN; i++)
   {
      pszVoiceDaughterCardId[i] = voiceCurrentDgtrCardCfgId[i];
   }

   return 0;
}


/**************************************************************************
* Name       : BpGetVoiceBoardIds
*
* Description: This function returns all of the supported voice board id strings.
*
* Parameters : [OUT] pszVoiceDaughterCardIds - Address of a buffer that the board id
*                  strings are returned in.  Each id starts at BP_BOARD_ID_LEN
*                  boundary.
*              [IN] nBoardIdsSize - Number of BP_BOARD_ID_LEN elements that
*                  were allocated in pszBoardIds.
*              [IN] pszBaseBoardId - Name of base Board ID to associate Voice
*                  Board ID with.
*
* Returns    : Number of board id strings returned.
***************************************************************************/
int BpGetVoiceBoardIds( char *pszVoiceDaughterCardIds, int nBoardIdsSize, char *pszBaseBoardId )
{
   int count = 0;
   bp_elem_t * pBpStartElem;
   PVOICE_DAUGHTER_BOARD_PARMS *ppDc;
  
   /* Get start element of voice board params structure */
   if( !(pBpStartElem = BpGetVoiceBoardStartElemPtr(pszBaseBoardId)) )
   {
      /* No matching base board found */
      return 0;     
   }
      
   /* Get daughtercard list pointer */
   ppDc = (PVOICE_DAUGHTER_BOARD_PARMS *)BpGetSubPtr(bp_daughterCardList, pBpStartElem, bp_last);     
   if( !ppDc ) 
   {
      /* No matching daughtercard list was found */
      return count;      
   }
   
   /* Iterate through daughter card list */
   for(; (*ppDc != 0) && (*ppDc != 0) && (nBoardIdsSize != 0); ppDc++)
   {
      /* Copy over daughtercard Ids */
      bpstrcpy(pszVoiceDaughterCardIds, (*ppDc)->szBoardId);
      pszVoiceDaughterCardIds += BP_BOARD_ID_LEN;
      nBoardIdsSize--;
      count++;
   }
   
   return( count );
} /* BpGetVoiceBoardIds */


/**************************************************************************
* Name       : BpGetVoiceDectType
*
* Description: This function returns whether or not Dect is supported on a given board.
*
* Parameters : [IN] pszBaseBoardId - Name of the base Board ID
*
* Returns    : Status indicating if the base board supports dect.
***************************************************************************/
int BpGetVoiceDectType( char *pszBaseBoardId )
{
   bp_elem_t * pBpStartElem;
   BP_VOICE_CHANNEL * dectChanCfg = 0;   
   int dectStatus = BP_VOICE_NO_DECT;
  
   /* Get start element of voice board params structure */
   if( !(pBpStartElem = BpGetVoiceBoardStartElemPtr(pszBaseBoardId)) )
   {
      /* No matching base board found */
      return dectStatus;     
   }
   
   /* Get IDECT Cfg */
   dectChanCfg = (BP_VOICE_CHANNEL *)BpGetSubPtr(bp_iDectCfg, pBpStartElem, bp_last);    
   if( dectChanCfg )
   {
      dectStatus = BP_VOICE_INT_DECT;
   }

   return dectStatus;
}

/**************************************************************************
* Name       : BpGetVoiceBoardStartElemPtr
*
* Description: This function returns the start element of a voice board params stucture
*              when given a baseboard id. If base boardid is not specified then the 
*              currently active base board id is retreived and used
*
* Parameters : [IN] pszBaseBoardId - Name of the base Board ID
*
* Returns    : Start element of matching voice boardparams structure.
***************************************************************************/
static bp_elem_t * BpGetVoiceBoardStartElemPtr( char * pszBaseBoardId )
{
   int bpPtrIndex;
   char * baseBoardId;
   char boardIdStr[BP_BOARD_ID_LEN];
  
   /* Get Base board Id if not specified*/
   if( !pszBaseBoardId )
   {
      if ( BpGetBoardId(boardIdStr) != BP_SUCCESS )
      {
         /* No matching base board found */
         return 0;      
      }
   }
   else
   {
      /* Copy over specified base board id */
      bpstrcpy(boardIdStr, pszBaseBoardId);
   }

   /* Iterate through list of voice board params to find matching structure to base board*/
   for( bpPtrIndex=0; g_VoiceBoardParms[bpPtrIndex]; bpPtrIndex++ )
   {
      baseBoardId = BpGetSubCp(bp_cpBoardId, g_VoiceBoardParms[bpPtrIndex], bp_last);
      if( baseBoardId && (0 == bpstrcmp(baseBoardId, boardIdStr)) )
      {
         /* Found the matching board */
         break;
      }      
   }   
   
   return g_VoiceBoardParms[bpPtrIndex];      
}

/**************************************************************************
* Name       : BpGetSlaveSelectGpioNum
*
* Description: This function returns the gpio number associated with a particular
*              SPI slave select
*
* Parameters : ssNum - Slave select ID
*
* Returns    : Start element of matching voice boardparams structure.
***************************************************************************/
static unsigned short BpGetSlaveSelectGpioNum( BP_SPI_PORT ssNum)
{
   bp_elem_t * pElem;
   
   for( pElem = g_pCurrentBp; pElem && (pElem->id != bp_last); pElem++ ) 
   {
      /* check for spi slave select definition.bp_usSpiSlaveSelectNum must be follwed by bp_usSpiSlaveSelectGpioNum */ 
      if( pElem->id == bp_usSpiSlaveSelectNum )
      {
         /* If ssNum matches, retrieve gpio num */
         if( pElem->u.us == (unsigned short)ssNum )
         {
            pElem++;
            if( (pElem->id != bp_last) && (pElem->id == bp_usSpiSlaveSelectGpioNum) )
            {
               /* Return active low for compatibility with legacy code */
               return (pElem->u.us|BP_ACTIVE_LOW);
            }
         }
      }
      
      /* Assign parent bp if pointer present */
      if( pElem->id == bp_elemTemplate )
      {
         pElem = pElem->u.bp_elemp;
      }                         
   }
   return BP_NOT_DEFINED;           
}
