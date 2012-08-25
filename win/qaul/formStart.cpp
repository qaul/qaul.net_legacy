/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "StdAfx.h"
#include "formStart.h"
#include "clix.h" // Marshaling managed strings to c++ strings

#include <stdio.h>
#include <stdlib.h>
#include <wlanapi.h>
#include <TlHelp32.h>
#include <objbase.h>
#include <wtypes.h>
#include <iphlpapi.h>
#include <shlobj.h>//for knownFolder
#include <winerror.h> //for HRESULT


#using <System.Xml.Dll>
using namespace System::Diagnostics;
using namespace System::Runtime::InteropServices;


// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "comdlg32.lib") // needed for GetOpenFileName() function
#pragma comment(lib, "Shell32.lib")  // needed for ShellExecute() function
#pragma comment(lib, "comsuppw")     // needed for known folder id´s

namespace qaul
{
void formStart::InitializeQaul(void)
{
    qaulStartCounter = 0;
	qaulTestError = false;

	// configure firewall
	ConfigureFirewall();

	// initialize qaullib
	Qaullib_Init((char*)(void*)Marshal::StringToHGlobalAnsi(qaulResourcePath));
	Debug::WriteLine(L"Qaullib_Init initialized");

	// set path to download folder
	HRESULT hr;
	LPWSTR qaulDownloadPath = NULL;
	hr = SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &qaulDownloadPath);
	if(SUCCEEDED(hr))
	{
		std::string qaulDownloadPathString("\?");
		cvtLPW2stdstring(qaulDownloadPathString, qaulDownloadPath, CP_ACP);
		Qaullib_SetConfDownloadFolder(qaulDownloadPathString.c_str());
	}

	// start webserver
	if(!Qaullib_WebserverStart()) ErrorShow(L"error starting webserver");

	// init wifi config values
	if(!isXP) dwMaxClient = 2; //  for windows vista and higher  
	else dwMaxClient = 1;      //  for windows XP  
    dwCurVersion = 0;
	//pin_ptr<DWORD> pdwCurVersion = &dwCurVersion;
	//pdwCurVersion = &dwCurVersion;
    dwResult = 0;

	// intitalize Backgroundworker
	// set callback functions for background worker
	this->backgroundWorkerStart = gcnew System::ComponentModel::BackgroundWorker;
    this->backgroundWorkerStart->WorkerReportsProgress = false;
    this->backgroundWorkerStart->WorkerSupportsCancellation = false;
	this->backgroundWorkerStart->DoWork += gcnew DoWorkEventHandler( this, &formStart::backgroundWorkerStart_DoWork );
    this->backgroundWorkerStart->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &formStart::backgroundWorkerStart_RunWorkerCompleted );
    //backgroundWorkerStart->ProgressChanged += gcnew ProgressChangedEventHandler( this, &formStart::backgroundWorkerStart_ProgressChanged );

}

void formStart::ExitQaul(void)
{
	Qaullib_Exit();
	Debug::WriteLine(L"Qaullib_Exit called");
	// kill port forward
	StopPortforward();
	// kill olsrd
	StopOlsr();
	// remove added firewall configuration
	UnconfigureFirewall();
}

void formStart::QaulStarting(void)
{
	// show web view
	if(qaulStartCounter == 0)
	{
		// load chat-interface
		Debug::WriteLine(L"StartHide");
		StartHide();
		Debug::WriteLine(L"ChatShow");
		ChatShow();

		// start with user configuration
		Qaullib_ConfigStart();

		qaulStartCounter = 10;
	}

#ifdef ARS_EDITION
	if(qaulStartCounter == 10)
	{
		if(Qaullib_TimedCheckAppEvent() == QAUL_EVENT_QUIT)
			qaulStartCounter = 9;
	}
#endif

	// check authorization rights
	if(qaulStartCounter == 10)
	{
		Debug::WriteLine(L"QaulStarting: Check authorization");
		// TODO: check for rights and 
		qaulStartCounter = 20;
	}
	
	// configure wifi
	if(qaulStartCounter >= 20 && qaulStartCounter <= 30)
	{
		Debug::WriteLine(L"QaulStarting: configure wifi");
		
		if(qaulStartCounter == 20)
		{
			if(WifiFindInterface()) qaulStartCounter = 21;
			else ErrorShow(L"No wifi interface found.");
		}

		if(qaulStartCounter == 21)
		{
			if(WifiSetProfile()) qaulStartCounter = 22;
			else ErrorShow(L"Error: Wasn't able to set wifi profile");
		}

		if(qaulStartCounter == 22)
		{
			if(WifiConnectProfile()) qaulStartCounter = 23;
			else ErrorShow(L"Error: Could not connect wifi profile");
		}
		
		if(qaulStartCounter == 23)
		{
			if(WifiSetIp()) qaulStartCounter = 29;
			else ErrorShow(L"Error: Could not set IP");
		}
	}

	// wait until it is connected
	if(qaulStartCounter == 29) 
		this->backgroundWorkerStart->RunWorkerAsync( 5000 );

	// check if user name is set
	if(qaulStartCounter == 30)
	{
		Debug::WriteLine(L"QaulStarting: wait until user name is set");
		if(!Qaullib_ExistsUsername())
		{
			// wait until username is set
			qaulStartCounter = 29;
			this->backgroundWorkerStart->RunWorkerAsync( 500 );
		}
		else
		{
			qaulStartCounter = 40;
		}
	}

	// start Routing
	// start olsrd
	if(qaulStartCounter == 40)
	{
		Debug::WriteLine(L"start: 40");
		if(!StartOlsr()) ErrorShow(L"olsrd startup failed");
		else qaulStartCounter = 44;
	}
	
	// wait until olsrd started
	if(qaulStartCounter == 44) this->backgroundWorkerStart->RunWorkerAsync( 1000 );

	// connect ipc
	if(qaulStartCounter == 45)
	{
		Debug::WriteLine(L"start: 45");
		int err = Qaullib_IpcConnect();
		if(err != 1) 
		{
			Debug::WriteLine(System::String::Format("Qaullib_IpcConnect failed with error: {0} ",err)); 
			qaulStartCounter = 44;
			this->backgroundWorkerStart->RunWorkerAsync( 1000 );
		}
		else
		{
			// start the timer
			ipcTimer->Tick += gcnew EventHandler(CheckIpcMessage);
			ipcTimer->Interval = 10;
			ipcTimer->Start();

			ipcTimer2->Tick += gcnew EventHandler(CheckIpcTopology);
			ipcTimer2->Interval = 3000;
			ipcTimer2->Start();

			ipcTimer3->Tick += gcnew EventHandler(CheckAppEvents);
			ipcTimer3->Interval = 100;
			ipcTimer3->Start();

			qaulStartCounter = 50;
		}
	}

	// start Webservices
	if(qaulStartCounter == 50)
	{
		Debug::WriteLine(L"start: 50");
		// start VoIP
		Qaullib_SetConfVoIP();

		if(!Qaullib_UDP_StartServer())
			Debug::WriteLine(L"error starting UDP server");
		// start captive portal
		if(!Qaullib_CaptiveStart()) 
			Debug::WriteLine(L"error starting captive portal");
		// start port forward
		if(!StartPortforward()) 
			Debug::WriteLine(L"error starting port forward");
		qaulStartCounter = 54;
	}

	if(qaulStartCounter == 54) 
	{
		Debug::WriteLine(L"start: 54");
		this->backgroundWorkerStart->RunWorkerAsync( 500 );
	}

	// finished
	if(qaulStartCounter == 55)
	{
		Debug::WriteLine(L"QaulStarting: finished");
		Qaullib_ConfigurationFinished();
		qaulStartCounter = 60;
	}
	
}

/**
 * configure wifi
 */
bool formStart::WifiFindInterface(void)
{
	HANDLE hClient = NULL;
    int iRet = 0;
    
    WCHAR GuidString[40] = {0};
	char buf[40] = "";
	char*charGuid = buf;
     
    int i;

    // variables used for WlanEnumInterfaces 
    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO pIfInfo = NULL;

    pin_ptr<DWORD> pdwCurVersion = &dwCurVersion;
    dwResult = WlanOpenHandle(dwMaxClient, NULL, pdwCurVersion, &hClient); 
    if (dwResult != ERROR_SUCCESS)  {
		Debug::WriteLine(System::String::Format("WlanOpenHandle failed with error: {0} ",dwResult));
        // FormatMessage can be used to find out why the function failed
        return false;
    }
    
    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList); 
    if (dwResult != ERROR_SUCCESS)  {
		Debug::WriteLine(System::String::Format("WlanEnumInterfaces failed with error: {0}", dwResult));
        // FormatMessage can be used to find out why the function failed
        return false;
    }
    else {
		Debug::WriteLine(System::String::Format("Num Entries: {0}", pIfList->dwNumberOfItems));
		Debug::WriteLine(System::String::Format("Current Index: {0}", pIfList->dwIndex));
        for (i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
            pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i];
			Debug::WriteLine(System::String::Format("Interface Index[{0}]: {1}", i, i));
            iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 39); 
            // For c rather than C++ source code, the above line needs to be
            // iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 39); 
            if (iRet == 0)
				Debug::WriteLine("StringFromGUID2 failed");
            else {
				sprintf_s(buf,"%ws",GuidString);
				System::String^ str = gcnew String(buf);
				Debug::WriteLine(str);
            }    
			char buf2[500] = "";
			sprintf_s(buf2,"Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
			System::String^ str2 = gcnew String(buf2);
			Debug::WriteLine(str2);
			Debug::WriteLine(System::String::Format("  Interface State[{0}]: ", i));
            switch (pIfInfo->isState) {
            case wlan_interface_state_not_ready:
				Debug::WriteLine("Not ready");
                break;
            case wlan_interface_state_connected:
				Debug::WriteLine("Connected");
                break;
            case wlan_interface_state_ad_hoc_network_formed:
				Debug::WriteLine("First node in an ad hoc network");
                break;
            case wlan_interface_state_disconnecting:
				Debug::WriteLine("Disconnecting");
                break;
            case wlan_interface_state_disconnected:
				Debug::WriteLine("Not connected");
                break;
            case wlan_interface_state_associating:
				Debug::WriteLine("Attempting to associate with a network");
                break;
            case wlan_interface_state_discovering:
				Debug::WriteLine("Auto configuration is discovering settings for the network");
                break;
            case wlan_interface_state_authenticating:
				Debug::WriteLine("In process of authenticating");
                break;
            default:
				Debug::WriteLine("Unknown state");
                break;
            }


			// -----------------------------------------------------------
			// search Network Interface Index
			PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
			DWORD AdapterInfoSize = 0;
			GetAdaptersInfo(NULL, &AdapterInfoSize);
			pAdapterInfo = (PIP_ADAPTER_INFO) GlobalAlloc(GPTR, AdapterInfoSize);
			
			dwResult = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize);
			if (dwResult == ERROR_SUCCESS) Debug::WriteLine(L"GetAdaptersInfo Success");
			else Debug::WriteLine(System::String::Format("GetAdaptersInfo Error  {0} ",(int)dwResult));
			
			pAdapt = pAdapterInfo;
			while(pAdapt)
			{
				// compare interface description
				System::String^ guid1 = gcnew String(pAdapt->AdapterName);
				System::String^ guid2 = gcnew String(charGuid);
				Debug::WriteLine(System::String::Format("compare names: {0}, {1}", guid1, guid2));
				
				if(strcmp(pAdapt->AdapterName,charGuid)==0)
				{
					Debug::WriteLine(System::String::Format("GUID Match, Index: {0}", (int)pAdapt->Index));

					netInterface->InterfaceGuid = pIfInfo->InterfaceGuid;
					netInterface->InterfaceIndex = pAdapt->Index;
					netInterface->InterfaceFound = true;
				}
				if(netInterface->InterfaceFound)break;
				pAdapt = pAdapt->Next;
			}
        }
    }
	return netInterface->InterfaceFound;
}

bool formStart::WifiSetProfile(void)
{
	bool success = false;
	// open Wlan Handle
	HANDLE hClient2 = NULL;
	pin_ptr<DWORD> pdwCurVersion = &dwCurVersion;
	dwResult = WlanOpenHandle(dwMaxClient, NULL, pdwCurVersion, &hClient2); 
	if (dwResult != ERROR_SUCCESS) 
	{
		Debug::WriteLine(System::String::Format("WlanOpenHandle failed with error: {0} ",(int)dwResult));
		return false;
	}

	// set Profile
	// cli: 
	// netsh wlan add profile filename=PATH interface=INTERFACENAME user=curent
	DWORD dwFlags;
	DWORD dwErrorCode = 0;
	if(!isXP) dwFlags = 0x00000002; // Windows Vista and newer (WLAN_PROFILE_USER)
	else dwFlags = 0;               // Windows XP
	LPCWSTR lpcwstrProfile = _T(qaulWifiProfile);

	dwResult = WlanSetProfile(
					hClient2,
					&netInterface->InterfaceGuid,
					dwFlags,
					lpcwstrProfile,
					NULL,
					true,
					NULL,
					&dwErrorCode
				);
			
	if (dwResult == ERROR_SUCCESS) success = true;
	else Debug::WriteLine(System::String::Format("WlanSetProfile Error {0}, {1}",(int)dwResult, (int)dwErrorCode));

	WlanCloseHandle(hClient2, NULL);
	return success;
}

bool formStart::WifiConnectProfile(void)
{
	bool success = false;
	// connect with Profile
	WLAN_CONNECTION_PARAMETERS sWLANConnParam;
	LPCWSTR lpsXMLProfileName = _T("qaul.net");

	// open Wlan Handle
	HANDLE hClient3 = NULL;
	pin_ptr<DWORD> pdwCurVersion = &dwCurVersion;
	dwResult = WlanOpenHandle(dwMaxClient, NULL, pdwCurVersion, &hClient3); 
	if (dwResult != ERROR_SUCCESS) 
	{
		Debug::WriteLine(System::String::Format("WlanOpenHandle failed with error: {0} ", dwResult));
		return false;
	}

	// set connection parameters
	memset(&sWLANConnParam, 0, sizeof(WLAN_CONNECTION_PARAMETERS));
	sWLANConnParam.wlanConnectionMode = wlan_connection_mode_profile;
	sWLANConnParam.strProfile = lpsXMLProfileName;
	sWLANConnParam.pDot11Ssid = NULL;                          // set SSID qaul.net here
	sWLANConnParam.pDesiredBssidList = NULL;                   // set BSSID here (must be null for windows XP)
	sWLANConnParam.dot11BssType = dot11_BSS_type_independent;  // adhoc network
	sWLANConnParam.dwFlags = 0;

	dwResult = WlanConnect(hClient3,
		&netInterface->InterfaceGuid,
		&sWLANConnParam,
		NULL);
	if (dwResult == ERROR_SUCCESS) success = true;
	else Debug::WriteLine(System::String::Format("WlanConnect Error {0}",(int)dwResult));

	WlanCloseHandle(hClient3, NULL);
	return success;
}

bool formStart::WifiSetIp(void)
{
	Debug::WriteLine(L"configure ip");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	System::String^ ip = Marshal::PtrToStringAnsi((IntPtr) (char *) Qaullib_GetIP());
	pin_ptr<const TCHAR> ipTchar = PtrToStringChars(ip);
	TCHAR cCmdBuf[5000];
	_stprintf(cCmdBuf,_T("netsh interface ip set address %i static %s 255.0.0.0 0.0.0.0 1"),
							(int)netInterface->InterfaceIndex,
							ipTchar
							);

	System::String^ strCmd = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("{0}", strCmd));

	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("ip configuration error: {0}",GetLastError()));
		return false;
	}
	else
	{
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, INFINITE );

		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}

	Debug::WriteLine(L"ip configured");
	return true;
}

/**
 * start olsr
 */
bool formStart::StartOlsr(void)
{
	// start olsrd
	// Interfaces: Olsrd recognizes the interfaces from its index
	//             in hexadecimal notation:
	//             12 -> if0c
	Debug::WriteLine(System::String::Format("Adapter Index: {0} ",netInterface->InterfaceIndex));

	TCHAR cCmdBuf[5000];
	pin_ptr<const TCHAR> tcResourcePath = PtrToStringChars(qaulResourcePath);
	_stprintf(cCmdBuf,_T("\"%s\\olsrd.exe\" -i if%02x -f \"%s\\olsrd_app.conf\" -d 0"), 
							tcResourcePath,
							netInterface->InterfaceIndex,
							tcResourcePath);

	System::String^ strCmd = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("olsrd cmd: {0}", strCmd));


	int i;
	for(i = 0; i < sizeof(STARTUPINFO); i++)
		((char *)&netInterface->StartInfo)[i] = 0;
	netInterface->StartInfo.cb = sizeof(STARTUPINFO);

	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &netInterface->StartInfo, &netInterface->ProcInfo)) {
		//printf("Olsrd Startup Error: %d\n", GetLastError());
		Debug::WriteLine(System::String::Format("Olsrd Startup Error: {0}",GetLastError()));
		return false;
	}

	// TODO: check if olsrd is running
	return true;
}

/**
 * stop Olsr
 */
bool formStart::StopOlsr(void)
{
	// kill all running olsrd instances
	Debug::WriteLine(L"kill olsrd");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	Debug::WriteLine(L"create cmd");

	TCHAR cCmdBuf[5000];
	_stprintf(cCmdBuf,_T("Taskkill /IM olsrd.exe /F"));

	Debug::WriteLine(L"create process");

	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("Olsrd Kill Error 2: {0}",GetLastError()));
		return false;
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, INFINITE );

		Debug::WriteLine(L"close handle");
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	Debug::WriteLine(L"olsrd killed");

	return true;
}

/**
 * control portforward
 */
// --------------------------------------------------
// start port forward
bool formStart::StartPortforward(void)
{
	Debug::WriteLine(L"start port forwarding");

	STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	TCHAR cCmdBuf[5000];
	pin_ptr<const TCHAR> tcResourcePath = PtrToStringChars(qaulResourcePath);
	_stprintf(cCmdBuf,_T("\"%s\\socat.exe\" \"TCP4-Listen:80,fork,reuseaddr\" \"TCP4:localhost:8081\""),tcResourcePath);
	System::String^ strCmd = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("port forward cmd: {0}", strCmd));

	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("Port forward Startup Error: {0}",GetLastError()));
		return false;
	}

	return true;
}

// --------------------------------------------------
// stop port forward
bool formStart::StopPortforward(void)
{
	Debug::WriteLine(L"kill port forwarding");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	TCHAR cCmdBuf[5000];
	pin_ptr<const TCHAR> tcResourcePath = PtrToStringChars(qaulResourcePath);
	_stprintf(cCmdBuf,_T("Taskkill /IM socat.exe /F"));

	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("Port Forward Kill Error 2: {0}",GetLastError()));
		return false;
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, INFINITE );

		Debug::WriteLine(L"close handle");
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	Debug::WriteLine(L"port forward killed");

	return true;
}

/**
 * configure the windows firewall
 */
bool formStart::ConfigureFirewall(void)
{
	Debug::WriteLine(L"configure firewall");

	STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	TCHAR cCmdBuf[5000];
	pin_ptr<const TCHAR> tcResourcePath = PtrToStringChars(qaulResourcePath);

	_stprintf(cCmdBuf,_T("netsh advfirewall firewall add rule name=\"qaul\" dir=in action=allow enable=yes program=\"%s\\qaul.exe\""),tcResourcePath);
	System::String^ strCmd5 = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("add firewall rule: {0}", strCmd5));
	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("firewall configuration error: {0}",GetLastError()));
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, 1000 );
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );	
	}

	_stprintf(cCmdBuf,_T("netsh advfirewall firewall add rule name=\"qaul\" dir=out action=allow enable=yes program=\"%s\\qaul.exe\""),tcResourcePath);
	System::String^ strCmd6 = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("add firewall rule: {0}", strCmd6));
	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("firewall configuration error: {0}",GetLastError()));
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, 1000 );
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );	
	}

	_stprintf(cCmdBuf,_T("netsh advfirewall firewall add rule name=\"qaul\" dir=in action=allow enable=yes program=\"%s\\socat.exe\""),tcResourcePath);
	System::String^ strCmd = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("add firewall rule: {0}", strCmd));
	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("firewall configuration error: {0}",GetLastError()));
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, 1000 );
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );	
	}

	_stprintf(cCmdBuf,_T("netsh advfirewall firewall add rule name=\"qaul\" dir=out action=allow enable=yes program=\"%s\\socat.exe\""),tcResourcePath);
	System::String^ strCmd2 = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("add firewall rule: {0}", strCmd2));
	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("firewall configuration error: {0}",GetLastError()));
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, 1000 );
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );	
	}

	_stprintf(cCmdBuf,_T("netsh advfirewall firewall add rule name=\"qaul\" dir=in action=allow enable=yes program=\"%s\\olsrd.exe\""),tcResourcePath);
	System::String^ strCmd3 = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("add firewall rule: {0}", strCmd3));
	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("firewall configuration error: {0}",GetLastError()));
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, 1000 );
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );	
	}

	_stprintf(cCmdBuf,_T("netsh advfirewall firewall add rule name=\"qaul\" dir=out action=allow enable=yes program=\"%s\\olsrd.exe\""),tcResourcePath);
	System::String^ strCmd4 = gcnew System::String(cCmdBuf);
	Debug::WriteLine(System::String::Format("add firewall rule: {0}", strCmd4));
	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("firewall configuration error: {0}",GetLastError()));
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, 1000 );
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );	
	}

	return true;
}

/**
 * remove the formerly set rules
 */
bool formStart::UnconfigureFirewall(void)
{
	Debug::WriteLine(L"remove firewall configuration");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	TCHAR cCmdBuf[5000];
	pin_ptr<const TCHAR> tcResourcePath = PtrToStringChars(qaulResourcePath);
	_stprintf(cCmdBuf,_T("netsh advfirewall firewall delete rule name=\"qaul\""));

	if(!CreateProcess(NULL, cCmdBuf, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		Debug::WriteLine(System::String::Format("Error firewall unconfiguration: {0}",GetLastError()));
		return false;
	}
	else
	{
		Debug::WriteLine(L"wait for process");
		// Wait until child process exits.
		WaitForSingleObject( pi.hProcess, INFINITE );

		Debug::WriteLine(L"close handle");
		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	Debug::WriteLine(L"firewall unconfigured");

	return true;
}

/**
 * timer
 */
void formStart::CheckIpcMessage( Object^ myObject, EventArgs^ myEventArgs)
{
	//Debug::WriteLine(L"Qaullib_IpcReceive");
	Qaullib_TimedSocketReceive();
}

void formStart::CheckIpcTopology( Object^ myObject, EventArgs^ myEventArgs)
{
	//Debug::WriteLine(L"Qaullib_IpcSendCom");
	Qaullib_IpcSendCom(1);
	//Debug::WriteLine(L"Qaullib_IpcCheckNonames");
	Qaullib_TimedDownload();
}

void formStart::CheckAppEvents( Object^ myObject, EventArgs^ myEventArgs)
{
	int appEvent = Qaullib_TimedCheckAppEvent();
	if(appEvent > 0)
	{
		// show file picker
		if(appEvent == QAUL_EVENT_CHOOSEFILE)
		{
			Debug::WriteLine(L"AppEvent add file");

			// via system.windows.forms.openfiledialog
			// http://msdn.microsoft.com/de-de/library/system.windows.forms.openfiledialog(v=vs.80).aspx
			// http://www.daniweb.com/software-development/cpp/threads/159753
			OPENFILENAME ofn;
			wchar_t fileName[MAX_PATH +1];
			ZeroMemory(&ofn, sizeof(ofn));
 
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = sizeof(fileName);
			//ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = L"";
 
			if (GetOpenFileName(&ofn))
			{
				// http://www.daniweb.com/software-development/cpp/threads/155420
				std::string fileNameString("\?");
				cvtLPW2stdstring(fileNameString, fileName, CP_ACP);
				const char* fileNameChar = fileNameString.c_str();
				Qaullib_FilePicked(2, fileNameString.c_str());
			}
		}
		// open file
		else if(appEvent == QAUL_EVENT_OPENFILE)
		{
			Debug::WriteLine(L"AppEvent open file");
			// open file with default application
			const char* openPath = Qaullib_GetAppEventOpenPath();
			int len = strlen(openPath) + 1;
			wchar_t *openPathW = new wchar_t[len];
			memset(openPathW, 0, len);
			MultiByteToWideChar(CP_ACP, NULL, openPath, -1, openPathW, len);
			ShellExecute(NULL, L"open", openPathW, NULL, NULL, SW_SHOWNORMAL);
		}
		else if(appEvent == QAUL_EVENT_OPENURL)
		{
			Debug::WriteLine(L"AppEvent open URL");
			// open file with default application
			const char* openURL = Qaullib_GetAppEventOpenURL();
			int len = strlen(openURL) + 1;
			wchar_t *openURLW = new wchar_t[len];
			memset(openURLW, 0, len);
			MultiByteToWideChar(CP_ACP, NULL, openURL, -1, openURLW, len);
			ShellExecute(NULL, L"open", openURLW, NULL, NULL, SW_SHOWNORMAL);
		}
		else if(appEvent == QAUL_EVENT_QUIT)
		{
			Application::Exit();
		}
		else if(appEvent == QAUL_EVENT_NOTIFY || appEvent == QAUL_EVENT_RING)
		{
			Beep(750, 300);
		}
	}
}

bool formStart::cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage)
{
    bool res = false;
    char* p = 0;
    int bsz;
 
    bsz = WideCharToMultiByte(codepage,
        0,
        pw,-1,
        0,0,
        0,0);
    if (bsz > 0) {
        p = new char[bsz];
        int rc = WideCharToMultiByte(codepage,
            0,
            pw,-1,
            p,bsz,
            0,0);
        if (rc != 0) {
            p[bsz-1] = 0;
            s = p;
            res = true;
        }
    }
    delete [] p;
    return res;
}

/**
 * background worker
 */
// start background worker
void formStart::backgroundWorkerStart_DoWork( Object^ sender, DoWorkEventArgs^ e )
{
	Sleep( safe_cast<Int32>(e->Argument) );
}
// end of background worker
void formStart::backgroundWorkerStart_RunWorkerCompleted( Object^ /*sender*/, RunWorkerCompletedEventArgs^ e )
{
	qaulStartCounter++;
	QaulStarting();
}

}
