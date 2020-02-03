/*******************************************************************************
 *
 * Copyright (c) 2000-2003 Intel Corporation 
 * All rights reserved. 
 * Copyright (C) 2011-2012 France Telecom All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met: 
 *
 * * Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer. 
 * * Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * * Neither name of Intel Corporation nor the names of its contributors 
 * may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/


#ifndef UPNPAPI_H
#define UPNPAPI_H

#include <list>
#include <string>
#include <vector>

#include "service_table.h"
#include "upnp.h"
#include "VirtualDir.h"		/* for struct VirtualDirCallbacks */
#include "TimerThread.h"
#include "description.h"
#include "client_table.h"

typedef struct _IXML_Node IXML_Node;
typedef struct _IXML_NodeList IXML_NodeList;


#define MAX_INTERFACES 256

#define DEFAULT_INTERFACE 1

#define DEV_LIMIT 200

#define DEFAULT_MX 5

#define DEFAULT_MAXAGE 1800

#define DEFAULT_SOAP_CONTENT_LENGTH 16000
#define MAX_SOAP_CONTENT_LENGTH (size_t)32000

extern size_t g_maxContentLength;
extern int g_UpnpSdkEQMaxLen;
extern int g_UpnpSdkEQMaxAge;

/* 30-second timeout */
#define UPNP_TIMEOUT	30

typedef enum {HND_INVALID=-1,HND_CLIENT,HND_DEVICE} Upnp_Handle_Type;

struct SsdpSearchArg;

/* Data to be stored in handle table for */
struct Handle_Info
{
	Handle_Info() {}
	/*! . */
	Upnp_Handle_Type HType{Upnp_Handle_Type(0)};
	/*! Callback function pointer. */
	Upnp_FunPtr  Callback{nullptr};
	/*! . */
	char *Cookie{nullptr};
	/*! 0 = not installed; otherwise installed. */
	int   aliasInstalled;

	/* Device Only */
#ifdef INCLUDE_DEVICE_APIS
	/*! URL for the use of SSDP. */
	char  DescURL[LINE_SIZE];
	/*! URL for the use of SSDP when answering to legacy CPs (CP searching
	 * for a v1 when the device is v2). */
	char  LowerDescURL[LINE_SIZE];
	/*! XML file path for device description. */
	char  DescXML[LINE_SIZE];
	/* Advertisement timeout */
	int MaxAge{0};
	/* Power State as defined by UPnP Low Power. */
	int PowerState{0};
	/* Sleep Period as defined by UPnP Low Power. */
	int SleepPeriod{0};
	/* Registration State as defined by UPnP Low Power. */
	int RegistrationState{0};
	/*! Parsed Device Description document. */
	UPnPDeviceDesc devdesc;
	/*! Table holding subscriptions and URL information. */
	service_table ServiceTable;
	/*! . */
	int MaxSubscriptions{0};
	/*! . */
	int MaxSubscriptionTimeOut{0};
	/*! Address family: AF_INET or AF_INET6. */
	int DeviceAf{0};
#endif

	/* Client only */
#ifdef INCLUDE_CLIENT_APIS
	/*! Client subscription list. */
	std::list<ClientSubscription> ClientSubList;
	/*! Active SSDP searches. */
	std::list<SsdpSearchArg*> SsdpSearchList;
#endif
};

extern ithread_rwlock_t GlobalHndRWLock;

/*!
 * \brief Get handle information.
 *
 * \return HND_DEVICE, UPNP_E_INVALID_HANDLE
 */
Upnp_Handle_Type GetHandleInfo(
	/*! handle pointer (key for the client handle structure). */
	int Hnd,
	/*! handle structure passed by this function. */
	struct Handle_Info **HndInfo); 


#define HandleLock() HandleWriteLock()


#define HandleWriteLock()  \
	UpnpPrintf(UPNP_INFO, API, __FILE__, __LINE__, "Trying a write lock\n"); \
	ithread_rwlock_wrlock(&GlobalHndRWLock); \
	UpnpPrintf(UPNP_INFO, API, __FILE__, __LINE__, "Write lock acquired\n");


#define HandleReadLock()  \
	UpnpPrintf(UPNP_INFO, API, __FILE__, __LINE__, "Trying a read lock\n"); \
	ithread_rwlock_rdlock(&GlobalHndRWLock); \
	UpnpPrintf(UPNP_INFO, API, __FILE__, __LINE__, "Read lock acquired\n");


#define HandleUnlock() \
	UpnpPrintf(UPNP_INFO, API,__FILE__, __LINE__, "Trying Unlock\n"); \
	ithread_rwlock_unlock(&GlobalHndRWLock); \
	UpnpPrintf(UPNP_INFO, API, __FILE__, __LINE__, "Unlocked rwlock\n");


/*!
 * \brief Get client handle info.
 *
 * \note The logic around the use of this function should be revised.
 *
 * \return HND_CLIENT, HND_INVALID
 */
Upnp_Handle_Type GetClientHandleInfo(
	/*! [in] client handle pointer (key for the client handle structure). */
	int *client_handle_out, 
	/*! [out] Client handle structure passed by this function. */
	struct Handle_Info **HndInfo);
/*!
 * \brief Retrieves the device handle and information of the first device of
 * 	the address family specified. The search begins at the 'start' index, which should
 *  be 0 for the first call, then the last successful value returned. This allows 
 *  listing all entries for the address family.
 *
 * \return HND_DEVICE or HND_INVALID
 */
Upnp_Handle_Type GetDeviceHandleInfo(
	/*! [in] place to start the search (i.e. last value returned). */
	UpnpDevice_Handle start, 
	/*! [in] Address family. */
	int AddressFamily,
	/*! [out] Device handle pointer. */
	int *device_handle_out, 
	/*! [out] Device handle structure passed by this function. */
	struct Handle_Info **HndInfo);

/*!
 * \brief Retrieves the device handle and information of the first device of
 * 	the address family specified, with a service having a controlURL or
 * 	eventSubURL matching the path.
 *
 * \return HND_DEVICE or HND_INVALID
 */
Upnp_Handle_Type GetDeviceHandleInfoForPath(
	/*! The Uri path. */
	const char *path, 
	/*! [in] Address family. */
	int AddressFamily,
	/*! [out] Device handle pointer. */
	int *device_handle_out, 
	/*! [out] Device handle structure passed by this function. */
	struct Handle_Info **HndInfo,
	/*! [out] Service info for found path. */
	service_info **serv_info
	);


extern char gIF_NAME[LINE_SIZE];
extern char gIF_IPV4[INET_ADDRSTRLEN];
extern char gIF_IPV6[INET6_ADDRSTRLEN];

extern char gIF_IPV6_ULA_GUA[INET6_ADDRSTRLEN];

extern unsigned gIF_INDEX;


extern unsigned short LOCAL_PORT_V4;
extern unsigned short LOCAL_PORT_V6;


/*! NLS uuid. */
extern Upnp_SID gUpnpSdkNLSuuid;

extern TimerThread *gTimerThread;
extern ThreadPool gRecvThreadPool;
extern ThreadPool gSendThreadPool;
extern ThreadPool gMiniServerThreadPool;

extern struct VirtualDirCallbacks virtualDirCallback;

void UpnpThreadDistribution(struct UpnpNonblockParam * Param);

/*!
 * \brief Print handle info.
 *	
 * \return UPNP_E_SUCCESS if successful, otherwise returns appropriate error.
 */
int PrintHandleInfo(
	/*! [in] Handle index. */
	UpnpClient_Handle Hnd);


#endif /* UPNPAPI_H */
