// rawethernet.cpp: implementation of the CRawEthernet class.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version
// 2 of the License, or (at your option) any later version.
// 
// This file is part of the VSCP Project (http://www.vscp.org) 
//
// Copyright (C) 2000-2015 Ake Hedman, 
// Grodans Paradis AB, <akhe@grodansparadis.com>
// 
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this file see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// http://www.opensourceforu.com/2011/02/capturing-packets-c-program-libpcap/
//

#include <stdio.h>
#include "unistd.h"
#include "stdlib.h"
#include <string.h>
#include "limits.h"
#include "syslog.h"
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>
#include <pcap.h>

#include "wx/wxprec.h"
#include "wx/wx.h"
#include "wx/defs.h"
#include "wx/app.h"
#include <wx/xml/xml.h>
#include <wx/listimpl.cpp>
#include <wx/thread.h>
#include <wx/tokenzr.h>
#include <wx/datetime.h>

#include <vscphelper.h>
#include <vscpremotetcpif.h>
#include <vscp_type.h>
#include <vscp_class.h>
#include "dlldrvobj.h"
#include "vscpl2drv_raweth.h"


extern CDllDrvObj theApp;


///////////////////////////////////////////////////////////////////////////////
//                         V S C P   D R I V E R -  A P I
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// VSCPOpen
//

extern "C" long VSCPOpen( const char *pUsername,
							const char *pPassword,
							const char *pHost,
							short port,
							const char *pPrefix,
							const char *pParameter,
							unsigned long flags)
{
	long h = 0;

	CRawEthernet *pdrvObj = new CRawEthernet();
	if (NULL != pdrvObj) {

		if (pdrvObj->open( pUsername,
							pPassword,
							pHost,
							port,
							pPrefix,
							pParameter,
							flags ) ) {

			if ( !( h = theApp.addDriverObject( pdrvObj ) ) ) {
				delete pdrvObj;
			}

		} 
		else {
			delete pdrvObj;
		}

	}

	return h;
}


///////////////////////////////////////////////////////////////////////////////
//  VSCPClose
// 

#ifdef WIN32
extern "C" int WINAPI EXPORT VSCPClose(long handle)
#else

extern "C" int VSCPClose(long handle)
#endif
{
	int rv = 0;

	CRawEthernet *pdrvObj = theApp.getDriverObject(handle);
	if (NULL == pdrvObj) return 0;
	pdrvObj->close();
	theApp.removeDriverObject(handle);
	rv = 1;
	return CANAL_ERROR_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//  VSCPBlockingSend
// 

extern "C" int
VSCPBlockingSend(long handle, const vscpEvent *pEvent, unsigned long timeout)
{
	int rv = 0;

	CRawEthernet *pdrvObj = theApp.getDriverObject(handle);
	if (NULL == pdrvObj) return CANAL_ERROR_MEMORY;
    
    pdrvObj->addEvent2SendQueue( pEvent );
    
	return CANAL_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//  VSCPBlockingReceive
// 

extern "C" int
VSCPBlockingReceive(long handle, vscpEvent *pEvent, unsigned long timeout)
{
	int rv = 0;
 
    // Check pointer
    if ( NULL == pEvent) return CANAL_ERROR_PARAMETER;
    
	CRawEthernet *pdrvObj = theApp.getDriverObject(handle);
	if (NULL == pdrvObj) return CANAL_ERROR_MEMORY;
    
    if ( wxSEMA_TIMEOUT == pdrvObj->m_semReceiveQueue.WaitTimeout( timeout ) ) {
        return CANAL_ERROR_TIMEOUT;
    }
    
	pdrvObj->m_mutexReceiveQueue.Lock();
    vscpEvent *pLocalEvent = pdrvObj->m_receiveList.front();
    pdrvObj->m_receiveList.pop_front();
	pdrvObj->m_mutexReceiveQueue.Unlock();
    if (NULL == pLocalEvent) return CANAL_ERROR_MEMORY;
    
    vscp_copyVSCPEvent( pEvent, pLocalEvent );
    vscp_deleteVSCPevent( pLocalEvent );
	
	return CANAL_ERROR_SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////
//  VSCPGetLevel
// 

#ifdef WIN32
extern "C" unsigned long WINAPI EXPORT VSCPGetLevel(void)
#else

extern "C" unsigned long VSCPGetLevel(void)
#endif
{
	return CANAL_LEVEL_USES_TCPIP;
}


///////////////////////////////////////////////////////////////////////////////
// VSCPGetDllVersion
//

#ifdef WIN32
extern "C" unsigned long WINAPI EXPORT VSCPGetDllVersion(void)
#else

extern "C" unsigned long VSCPGetDllVersion(void)
#endif
{
	return VSCP_DLL_VERSION;
}


///////////////////////////////////////////////////////////////////////////////
// VSCPGetVendorString
//

#ifdef WIN32
extern "C" const char * WINAPI EXPORT VSCPGetVendorString(void)
#else

extern "C" const char * VSCPGetVendorString(void)
#endif
{
	return VSCP_DLL_VENDOR;
}


///////////////////////////////////////////////////////////////////////////////
// VSCPGetDriverInfo
//

#ifdef WIN32
extern "C" const char * WINAPI EXPORT VSCPGetDriverInfo(void)
#else

extern "C" const char * VSCPGetDriverInfo(void)
#endif
{
	return VSCP_RAWETH_DRIVERINFO;
}


///////////////////////////////////////////////////////////////////////////////
//  VSCPGetVSCPGetWebPageTemplate
// 

extern "C" long
VSCPGetWebPageTemplate( long handle, const char *url, char *page )
{
    page = NULL;

    // Not implemented
    return -1;
}


///////////////////////////////////////////////////////////////////////////////
//  VSCPGetVSCPWebPageInfo
// 

extern "C" int
VSCPGetWebPageInfo( long handle, const struct vscpextwebpageinfo *info )
{
    // Not implemented
    return -1;
}


///////////////////////////////////////////////////////////////////////////////
//  VSCPWebPageupdate
// 

extern "C" int
VSCPWebPageupdate( long handle, const char *url )
{
    // Not implemented
    return -1;
}



///////////////////////////////////////////////////////////////////////////////
//                            T H E  C O D E
///////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CRawEthernet
//

CRawEthernet::CRawEthernet()
{
	m_bQuit = false;
	m_preadWorkThread = NULL;
	m_pwriteWorkThread = NULL;
	m_interface = _("eth0");
	memset(m_localMac, 0, 16);

	// Initialize tx channel GUID
	m_localGUIDtx.clear();
	m_localGUIDtx.setAt(0, 0xff);
	m_localGUIDtx.setAt(1, 0xff);
	m_localGUIDtx.setAt(2, 0xff);
	m_localGUIDtx.setAt(3, 0xff);
	m_localGUIDtx.setAt(4, 0xff);
	m_localGUIDtx.setAt(5, 0xff);
	m_localGUIDtx.setAt(6, 0xff);
	m_localGUIDtx.setAt(7, 0xfe);
	m_localGUIDtx.setAt(14, 0x00);
	m_localGUIDtx.setAt(15, 0x00);

	// Initialize rx channel GUID
	m_localGUIDrx.clear();
	m_localGUIDrx.setAt(0, 0xff);
	m_localGUIDrx.setAt(1, 0xff);
	m_localGUIDrx.setAt(2, 0xff);
	m_localGUIDrx.setAt(3, 0xff);
	m_localGUIDrx.setAt(4, 0xff);
	m_localGUIDrx.setAt(5, 0xff);
	m_localGUIDrx.setAt(6, 0xff);
	m_localGUIDrx.setAt(7, 0xfe);
	m_localGUIDrx.setAt(14, 0x00);
	m_localGUIDrx.setAt(15, 0x01);

	vscp_clearVSCPFilter(&m_vscpfilter); // Accept all events
	::wxInitialize();
}

//////////////////////////////////////////////////////////////////////
// ~CRawEthernet
//

CRawEthernet::~CRawEthernet()
{
	close();
	::wxUninitialize();
}

//////////////////////////////////////////////////////////////////////
// addEvent2SendQueue
//

bool 
CRawEthernet::addEvent2SendQueue(const vscpEvent *pEvent)
{
    m_mutexSendQueue.Lock();
    m_sendList.push_back((vscpEvent *)pEvent);
	m_semSendQueue.Post();
	m_mutexSendQueue.Unlock();
    return true;
}

//////////////////////////////////////////////////////////////////////
// open
//
//

bool
CRawEthernet::open(const char *pUsername,
                    const char *pPassword,
                    const char *pHost,
                    short port,
                    const char *pPrefix,
                    const char *pConfig,
					unsigned long flags )
{
	bool rv = true;
	wxString wxstr = wxString::FromAscii(pConfig);

	m_username = wxString::FromAscii(pUsername);
	m_password = wxString::FromAscii(pPassword);
	m_host = wxString::FromAscii(pHost);
	m_port = port;
	m_prefix = wxString::FromAscii(pPrefix);

	// Parse the configuration string. It should
	// have the following form
	// path
	// 
	wxStringTokenizer tkz(wxString::FromAscii(pConfig), _(";\n"));

	// Look for raw Ethernet interface in configuration string
	if (tkz.HasMoreTokens()) {
		// Interface
		m_interface = tkz.GetNextToken();
	}

	// Local Mac
	wxString localMac;
	if (tkz.HasMoreTokens()) {
		localMac = tkz.GetNextToken();
		localMac.MakeUpper();
		wxStringTokenizer tkzmac(localMac, _(":\n"));
		for (int i = 0; i < 6; i++) {
			if (!tkzmac.HasMoreTokens()) break;
			wxString str = _("0X") + tkzmac.GetNextToken();
			m_localMac[ i ] = vscp_readStringValue(str);
			m_localGUIDtx.setAt((9 + i), m_localMac[ i ]);
			m_localGUIDrx.setAt((9 + i), m_localMac[ i ]);
		}
	}


	// First log on to the host and get configuration 
	// variables

	if ( VSCP_ERROR_SUCCESS !=  m_srv.doCmdOpen( m_host,
                                                    m_username,
                                                    m_password ) ) {
		syslog(LOG_ERR,
				"%s",
				(const char *) "Unable to connect to VSCP TCP/IP interface. Terminating!");
		return false;
	}

	// Find the channel id
	uint32_t ChannelID;
	m_srv.doCmdGetChannelID(&ChannelID);

	// The server should hold configuration data for each sensor
	// we want to monitor.
	// 
	// We look for 
	//
	//	 _interface - The ethernet interface to use. Typically this 
	//					is “eth0, eth0, eth1...
	//
	//   _localmac - The MAC address for our outgoing frames.
	//					Typically on the form 00:26:55:CA:1F:DA
	//
	//   _filter - Standard VSCP filter in string form. 
	//				   1,0x0000,0x0006,
	//				   ff:ff:ff:ff:ff:ff:ff:01:00:00:00:00:00:00:00:00
	//				as priority,class,type,GUID
	//				Used to filter what events that is received from 
	//				the socketcan interface. If not give all events 
	//				are received.
	//	 _mask - Standard VSCP mask in string form.
	//				   1,0x0000,0x0006,
	//				   ff:ff:ff:ff:ff:ff:ff:01:00:00:00:00:00:00:00:00
	//				as priority,class,type,GUID
	//				Used to filter what events that is received from 
	//				the socketcan interface. If not give all events 
	//				are received. 
	//

	wxString str;
	wxString strName = m_prefix +
			wxString::FromAscii("_interface");
	if ( VSCP_ERROR_SUCCESS == m_srv.getVariableString( strName, &str ) ) {
		m_interface = str;
	}

	// Local Mac
	strName = m_prefix +
			wxString::FromAscii("_localmac");
	
	if ( VSCP_ERROR_SUCCESS == m_srv.getVariableString( strName, &str ) ) {
		
		localMac = str;
	
		if (tkz.HasMoreTokens()) {
			localMac = tkz.GetNextToken();
			localMac.MakeUpper();
			wxStringTokenizer tkzmac(localMac, _(":\n"));
			for (int i = 0; i < 6; i++) {
				if (!tkzmac.HasMoreTokens()) break;
				wxString str = _("0X") + tkzmac.GetNextToken();
				m_localMac[ i ] = vscp_readStringValue(str);
				m_localGUIDtx.setAt((9 + i), m_localMac[ i ]);
				m_localGUIDrx.setAt((9 + i), m_localMac[ i ]);
			}
		}
		
	}

	strName = m_prefix +
			wxString::FromAscii("_filter");
	if (VSCP_ERROR_SUCCESS ==  m_srv.getVariableString(strName, &str)) {
		vscp_readFilterFromString(&m_vscpfilter, str);
	}

	strName = m_prefix +
			wxString::FromAscii("_mask");
	if (VSCP_ERROR_SUCCESS == m_srv.getVariableString(strName, &str)) {
		vscp_readMaskFromString(&m_vscpfilter, str);
	}

	// start the read workerthread
	m_preadWorkThread = new CWrkReadThread();
	if (NULL != m_preadWorkThread) {
		m_preadWorkThread->m_pObj = this;
		m_preadWorkThread->Create();
		m_preadWorkThread->Run();
	} 
	else {
		rv = false;
	}

	// start the write workerthread
	m_pwriteWorkThread = new CWrkWriteThread();
	if (NULL != m_pwriteWorkThread) {
		m_pwriteWorkThread->m_pObj = this;
		m_pwriteWorkThread->Create();
		m_pwriteWorkThread->Run();
	} else {
		rv = false;
	}

	// Close the channel
	m_srv.doCmdClose();

	return rv;
}


//////////////////////////////////////////////////////////////////////
// close
//

void
CRawEthernet::close(void)
{
	// Do nothing if already terminated
	if (m_bQuit) return;

	close();
	::wxUninitialize();

	m_bQuit = true; // terminate the thread
	wxSleep(1); // Give the thread some time to terminate

}



//////////////////////////////////////////////////////////////////////
//                Workerthread - CWrkReadThread
//////////////////////////////////////////////////////////////////////

CWrkReadThread::CWrkReadThread()
{
	m_pObj = NULL;
}

CWrkReadThread::~CWrkReadThread()
{
	;
}


//////////////////////////////////////////////////////////////////////
// Entry
//

void *
CWrkReadThread::Entry()
{
	pcap_t *fp;
	char errbuf[ PCAP_ERRBUF_SIZE ];
	
	// Open the adapter 
	if ( ( fp = pcap_open_live( (const char *)m_pObj->m_interface.mb_str(), // name of the device
									65536,	// portion of the packet to capture. It doesn't matter in this case 
									1,		// promiscuous mode (nonzero means promiscuous)
									1000,	// read timeout
									errbuf	// error buffer
								) ) == NULL) {
		syslog(LOG_ERR,
				"RawEthDrv: Unable to open the adapter or %s is not supported by pcap. Err=%s",
				(const char *)m_pObj->m_interface.mb_str(), errbuf );
		return NULL;
	}

	int rv;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	while (!TestDestroy() &&
			!m_pObj->m_bQuit  ) {

		rv = pcap_next_ex( fp, &header, &pkt_data );
			
		// Check for timeout            
		if (0 == rv) continue;
		
		// Check for error
		if ( rv < 0 ) {
			
			syslog(LOG_ERR,
				"RawEthDrv: Error while getting packet. Err=%s", pcap_geterr(fp) );
		}

        // If from our own interface we skip
        if ( 0 == memcmp( pkt_data + 6, m_pObj->m_localMac, 6 ) ) continue;

		// Check if this is VSCP
		if ( ( 0x25 == pkt_data[ 12 ] ) &&
			( 0x7e == pkt_data[ 13 ] ) ) {

			// We have a packet - send it as a VSCP event    
			vscpEventEx eventex;

			eventex.head = pkt_data[ 15 ] & 0xe0; // Priority

			eventex.GUID[ 0 ] = 0xff; // Ethernet predefined  GUID
			eventex.GUID[ 1 ] = 0xff;
			eventex.GUID[ 2 ] = 0xff;
			eventex.GUID[ 3 ] = 0xff;
			eventex.GUID[ 4 ] = 0xff;
			eventex.GUID[ 5 ] = 0xff;
			eventex.GUID[ 6 ] = 0xff;
			eventex.GUID[ 7 ] = 0xfe;
			eventex.GUID[ 8 ] = pkt_data[ 6 ]; // Source MAC address
			eventex.GUID[ 9 ] = pkt_data[ 7 ];
			eventex.GUID[ 10 ] = pkt_data[ 8 ];
			eventex.GUID[ 11 ] = pkt_data[ 9 ];
			eventex.GUID[ 12 ] = pkt_data[ 10 ];
			eventex.GUID[ 13 ] = pkt_data[ 11 ];
			eventex.GUID[ 14 ] = pkt_data[ 19 ]; // Device sub address
			eventex.GUID[ 15 ] = pkt_data[ 20 ];

			eventex.timestamp = (pkt_data[ 21 ] << 24) +
					(pkt_data[ 22 ] << 16) +
					(pkt_data[ 23 ] << 8) +
					pkt_data[ 24 ];

			eventex.obid = (pkt_data[ 25 ] << 24) +
					(pkt_data[ 26 ] << 16) +
					(pkt_data[ 27 ] << 8) +
					pkt_data[ 28 ];

			eventex.vscp_class = (pkt_data[ 29 ] << 8) +
                    pkt_data[ 30 ];

			eventex.vscp_type = (pkt_data[ 31 ] << 8) +
					pkt_data[ 32 ];

			eventex.sizeData = (pkt_data[ 33 ] << 8) +
					pkt_data[ 34 ];

            // Validate data size
            if ( eventex.vscp_class < 512 ) {
                if ( eventex.sizeData > 8 ) eventex.sizeData = 8;
            }
            else if ( eventex.vscp_class < 512 ) {
                if ( eventex.sizeData >( 16 + 8 ) ) eventex.sizeData = 24;
            }
            else {
                if ( eventex.sizeData > VSCP_MAX_DATA ) eventex.sizeData = VSCP_MAX_DATA;
            }

			// If the packet is smaller then the set datasize just 
			// disregard it
			if ((eventex.sizeData + 35) > (uint16_t) header->len) continue;

            memcpy( eventex.data, pkt_data + 35, eventex.sizeData );

            vscpEvent *pEvent = new vscpEvent;
            if (NULL != pEvent) {
                
                vscp_convertVSCPfromEx(pEvent, &eventex);

                if (vscp_doLevel2FilterEx( &eventex, &m_pObj->m_vscpfilter)) {
                    m_pObj->m_mutexReceiveQueue.Lock();
                    m_pObj->m_receiveList.push_back(pEvent);
                    m_pObj->m_semReceiveQueue.Post();
                    m_pObj->m_mutexReceiveQueue.Unlock();
                }
                else {
                    vscp_deleteVSCPevent(pEvent);
                }
            }

        }

	} // work loop   

	// Close listner
	pcap_close(fp);

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// OnExit
//

void
CWrkReadThread::OnExit()
{
	;
}



//////////////////////////////////////////////////////////////////////
//                Workerthread - CWrkWriteThread
//////////////////////////////////////////////////////////////////////

CWrkWriteThread::CWrkWriteThread()
{
	m_pObj = NULL;
}

CWrkWriteThread::~CWrkWriteThread()
{
	;
}


//////////////////////////////////////////////////////////////////////
// Entry
//

void *
CWrkWriteThread::Entry()
{
	pcap_t *fp;
	char errbuf[ PCAP_ERRBUF_SIZE ];
	uint8_t packet[ 512 ];
    
	// Open the adapter 
	if ((fp = pcap_open_live( (const char *)m_pObj->m_interface.mbc_str(),	// name of the device
								65536,		// portion of the packet to capture. It doesn't matter in this case 
								1,			// promiscuous mode (nonzero means promiscuous)
								1000,		// read timeout
								errbuf		// error buffer
							) ) == NULL ) {
		syslog(LOG_ERR,
				"RawEthDrv: Unable to open the adapter or %s is not supported by WinPcap. Err=%s",
				(const char *)m_pObj->m_interface.mb_str(), errbuf );
		return NULL;
	}


	int rv;

	while (!TestDestroy() && !m_pObj->m_bQuit) {
        
        if ( wxSEMA_TIMEOUT == m_pObj->m_semSendQueue.WaitTimeout( 300 ) ) continue;
        
        // Check if there is event(s) to send
        if ( m_pObj->m_sendList.size() ) {

            // Yes there are data to send
            m_pObj->m_mutexSendQueue.Lock();
            vscpEvent *pEvent = m_pObj->m_sendList.front();
            m_pObj->m_sendList.pop_front();
            m_pObj->m_mutexSendQueue.Unlock();

            if (NULL == pEvent) continue;
            
            // Set mac destination to broadcast ff:ff:ff:ff:ff:ff 
			packet[ 0 ] = 0xff;
			packet[ 1 ] = 0xff;
			packet[ 2 ] = 0xff;
			packet[ 3 ] = 0xff;
			packet[ 4 ] = 0xff;
			packet[ 5 ] = 0xff;

			// set mac source to configured value - 6..11
			memcpy(packet + 6, m_pObj->m_localMac, 6);

			// Set the type - always 0x2574 (9598)
			packet[ 12 ] = 0x25;
			packet[ 13 ] = 0x7e;

			// rawEthernet frame version
			packet[ 14 ] = 0x00;

			// Head
			packet[ 15 ] = ( pEvent->head & VSCP_HEADER_PRIORITY_MASK);
			packet[ 16 ] = 0x00;
			packet[ 17 ] = 0x00;
			packet[ 18 ] = 0x00; // LSB

			// VSCP sub source address For this interface it's 0x0000
			packet[ 19 ] = 0x00;
			packet[ 20 ] = 0x00;

			// Timestamp
			uint32_t timestamp = pEvent->timestamp;
			packet[ 21 ] = (timestamp & 0xff000000) >> 24;
			packet[ 22 ] = (timestamp & 0x00ff0000) >> 16;
			packet[ 23 ] = (timestamp & 0x0000ff00) >> 8;
			packet[ 24 ] = (timestamp & 0x000000ff);

			// obid
			uint32_t obid = pEvent->obid;
			packet[ 25 ] = (obid & 0xff000000) >> 24;
			packet[ 26 ] = (obid & 0x00ff0000) >> 16;
			packet[ 27 ] = (obid & 0x0000ff00) >> 8;
			packet[ 28 ] = (obid & 0x000000ff);

			// VSCP Class
			uint16_t vscp_class = pEvent->vscp_class;
			packet[ 29 ] = (vscp_class & 0xff00) >> 8;
			packet[ 30 ] = (vscp_class & 0xff);

			// VSCP Type
			uint16_t vscp_type = pEvent->vscp_type;
			packet[ 31 ] = (vscp_type & 0xff00) >> 8;
			packet[ 32 ] = (vscp_type & 0xff);

			// Size
			packet[ 33 ] = pEvent->sizeData >> 8;
			packet[ 34 ] = pEvent->sizeData & 0xff;

			// VSCP Data
			memcpy(packet + 35, pEvent->pdata, pEvent->sizeData );
			
			//syslog( LOG_INFO,
			//			"RawEthDrv2: Class: %d Type: %d DataSize: %d", pEvent->vscp_class, pEvent->vscp_type, pEvent->sizeData );

			// Send the packet
			if ( 0 != pcap_sendpacket( fp, packet, 35 + pEvent->sizeData ) ) {
				syslog( LOG_ERR,
						"RawEthDrv: Error when sending the packet: %s\n", pcap_geterr( fp ) );
			}

            // Remove the event
            m_pObj->m_mutexSendQueue.Lock();
            vscp_deleteVSCPevent(pEvent);
            m_pObj->m_mutexSendQueue.Unlock();

		} // Event received

	} // work loop   

	// Close the Ethernet interface
	pcap_close(fp);

	// Close the channel
	m_srv.doCmdClose();

	return NULL;
}

//////////////////////////////////////////////////////////////////////
// OnExit
//

void
CWrkWriteThread::OnExit()
{
	;
}


