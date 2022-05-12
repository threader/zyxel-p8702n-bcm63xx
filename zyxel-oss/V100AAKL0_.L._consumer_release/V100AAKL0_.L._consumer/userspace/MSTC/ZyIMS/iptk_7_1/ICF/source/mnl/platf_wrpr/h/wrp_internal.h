/*-----------------------------------------------------------------------------
 *
 * File name        : wrp_internal.h
 *
 * Purpose          : This file contains the internal classes, CMNLWrapper, 
 *						CGPRSConnector and CNwMonitor definitions.
 *						
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 15-Aug-2005  Irfan Khan						Initial
 *
 *
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __WRP_INTERNAL_H
#define __WRP_INTERNAL_H

/*Includes*/
#include <e32base.h>
#include "etelagsm.h"
#include "etelbgsm.h"
#include <in_sock.h>

#ifdef __SYMBIAN32__
#include "hssutilityhandle.h"
#include "gprsconnector.h"
#endif
#include "wrp_types.h"

/*Forward declarations*/
class CNwMonitor;

#define NO_OF_SOCKETS 30

/*Class declarations*/

/**
* CMNLWrapper
* Global object to the wrapper, contains CGPRSConnector and CNwMonitor
* class's objects
*/
class CMNLWrapper:public CBase,public MGPRSConnectorCallback
					,public MNetworkMonitorCallback
{

public:
	/**
    * Static fuction creates and returns the objects of CMNLWrapper.
	* It also ensures that not more then one objects of the class could be
	* created. It creates the object and keeps the pointer of the object into
	* TLS and returns same pointer next call onwards
    * @return A pointer to the created CMNLWrapper object.
    */
	static CMNLWrapper* NewL();
		
	/*Default destructor	*/
	~CMNLWrapper();
private:
	
	/*Default Constructor*/
	CMNLWrapper();
	
	/*Second phase constructor*/
	void ConstructL();

public:
	/*Pointer to GPRSCOnnector*/
	CGPRSConnector				*iGprsConnectorPtr;
	/*Pointer to GPRSEventObserver*/
	CGprsEventObserver			*iGprsEventObserverPtr;
	/*Pointer to CNwMonitor*/
	CNwMonitor* iLocMonitor;
	CHSSUtilityHandle *iHSSUtilityHandle;
	icf_return_t (*smcRsporIndCallBack)(icf_msg_st *pMsg);

	/*from MNetworkMonitorCallback*/
	virtual void HandleCallback(TGPRSNotifyEvent aEvent,TInt aError);

	/*from MNetworkMonitorCallback*/
	virtual void HandleCallback(TGPRSMonitorNotifyEvent aNetworkStatus);	
};


/**
* CNwMonitor
* Retrieve information from the platform and also Monitors changes
* in the current network
*/
class CNwMonitor:public CActive
{
public:
	/**
	* Creates and returns the object of CNwMonitor
	* @return pointer to the object of CNwMonitor
	*/
	static CNwMonitor* NewL();

	/*Default Destructor*/
	virtual ~CNwMonitor();
	
	/*To set the iNotifier*/
	void SetNotifier(TAny* (*notify) (TAny* aCallblData, TAny* aRes));

	/*Inlines*/
	/*To get the refrence of iStatus*/
	TRequestStatus& Status(){return iStatus;}
	/*To set Active the CNwMonitor*/
	void SetCNMActive(){SetActive();}

protected:
	/*Construcotr*/
	CNwMonitor();
	/*Second phase constructor*/
	void ConstructL();	

protected:
	/*From CActive*/
	void DoCancel();
	void RunL();

public:
	/*Current network information*/
	MBasicGsmPhoneNetwork::TCurrentNetworkInfo  iNwInfo;

	/*Handle of basic GSM phone	*/
	RBasicGsmPhone iBPhone;
	/*Handle of advance GSM phone*/
	RAdvGsmPhone iAPhone;
	
	RTelServer iServer;

    /*call back data*/
    TAny* iCallbackData;
	/*call back function pointer*/
	TAny* (*iNotifier)(TAny* aCallBackData, TAny* aMsg);
};


#endif /*__WRP_INTERNAL_H*/
