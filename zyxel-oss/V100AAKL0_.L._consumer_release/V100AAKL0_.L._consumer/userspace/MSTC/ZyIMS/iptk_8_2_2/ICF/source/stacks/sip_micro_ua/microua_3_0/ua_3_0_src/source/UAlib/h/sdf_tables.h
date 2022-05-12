

#ifndef __SDF_TABLES_H
#define __SDF_TABLES_H

/******************************************************************************
 ** FUNCTION:
 **	 	This file has some table mappings maintained by the UA Toolkit
 ******************************************************************************
 **
 ** FILENAME		: sdf_tables.h
 **
 ** DESCRIPTION		: This file has some table mappings maintained by 
 **						the UA Toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 24/10/00	Anitha						Creation
 **
 ******************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ******************************************************************************/

/*
 * Table mapping the status codes to reason phrases.
 */
extern Sdf_st_statusCodeMapping sdf_gl_dCodeReasonMapping[];

/*
 * Table mapping payload types to their registered IANA names
 * Table mapping codec Numbers to the corresponding names
 */
extern Sdf_st_avtProfileMap sdf_gl_dCodecProfile[];
	

/* 
 * CSM Table for state transitions based on incoming SIP messages:
 * Rows 	=> Current state
 * Columns 	=> Message Received
 * Element 	=> Next state 
 */
extern Sdf_st_nextCallState dCsmNxtStateOnMsgRecd\
		[Sdf_mc_noOfMessageTypes][Sdf_mc_noOfStates];

/* 
 * CSM Table for state transitions based on outgoing SIP messages:
 * Rows 	=> Current state
 * Columns	=> Message Sent 
 * Element 	=> Next state 
 */
extern Sdf_ty_state dCsmNxtStateOnMsgSent[Sdf_mc_noOfMessageTypes][Sdf_mc_noOfStates];


/*Csm Table for Media State*/
extern Sdf_ty_mediaState dMediaCsmNxtStateOnMsgSent[Sdf_mc_noOfMessageTypes]\
											[Sdf_mc_noOfMediaStates];
extern Sdf_ty_mediaState dMediaCsmNxtStateOnMsgRecv[Sdf_mc_noOfMessageTypes]\
											[Sdf_mc_noOfMediaStates];
/*Media State Validation Funciton Pointer table*/
extern Sdf_ty_fptrMediaStateValidationFunc	dGlbMediaStateValidationTable\
												[Sdf_mc_noOfMediaStates];
/*Array Of Strings Specifying the Groups to be Supported by UATK.
 * By Default FID and LS will be supported.If application wants
 * to support anything else,it needs to add the Group name here
 * and increment the Macro Sdf_mc_noOfExtraGroups
 */
extern Sdf_ty_s8bit*	dOtherSupportedGroups[];
#endif


