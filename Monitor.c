#include "Monitor.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")

void dword_to_ip(DWORD ip, char* buf) {
	sprintf_s(buf, 16, "%d.%d.%d.%d",
		((ip >> 24) & 0xFF),
		((ip >> 16) & 0xFF),
		((ip >> 8) & 0xFF),
		(ip & 0xFF));
}

int  fix_port(DWORD port) {
	return ntohs((u_short)port);

}
void get_process_name(DWORD pid, char *buf) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (hProcess) {
		DWORD size = 256;
		if (!QueryFullProcessImageNameA(hProcess, 0, buf, &size)) {
			strcpy_s(buf, 256, "Unknown");
		}
		CloseHandle(hProcess);
	} else {
		strcpy_s(buf, 256, "Unknown");
	}
}
const char* state_to_string(DWORD state) {
	switch (state) {
	case MIB_TCP_STATE_CLOSED:      return "CLOSED";
	case MIB_TCP_STATE_LISTEN:      return "LISTEN";
	case MIB_TCP_STATE_SYN_SENT:    return "SYN_SENT";
	case MIB_TCP_STATE_SYN_RCVD:    return "SYN_RECEIVED"; 
	case MIB_TCP_STATE_ESTAB:       return "ESTABLISHED"; 
	case MIB_TCP_STATE_FIN_WAIT1:   return "FIN_WAIT1";
	case MIB_TCP_STATE_FIN_WAIT2:   return "FIN_WAIT2";
	case MIB_TCP_STATE_CLOSE_WAIT:  return "CLOSE_WAIT";
	case MIB_TCP_STATE_CLOSING:     return "CLOSING";
	case MIB_TCP_STATE_LAST_ACK:    return "LAST_ACK";
	case MIB_TCP_STATE_TIME_WAIT:   return "TIME_WAIT";
	default:                        return "UNKNOWN";
	}
}

void print_connection(MIB_TCPROW_OWNER_PID* row) {
	char local_ip[16], remote_ip[16], process_name[256];
	dword_to_ip(row->dwLocalAddr, local_ip);
	dword_to_ip(row->dwRemoteAddr, remote_ip);
	get_process_name(row->dwOwningPid, process_name);

	char* name_only = strrchr(process_name, '\\');
	char* final_name = (name_only != NULL) ? name_only + 1 : process_name;

	printf("PID: %5d | Local: %s:%d | Remote: %s:%d | State: %s | Process: %s\n",
		row->dwOwningPid,
		local_ip, fix_port(row->dwLocalPort),
		remote_ip, fix_port(row->dwRemotePort),
		state_to_string(row->dwState),
		final_name);
}

void monitor_loop() {
	PMIB_TCPTABLE_OWNER_PID tcpTable = NULL;
	DWORD size = 0;
	DWORD result;

	while (1) {

		result = GetExtendedTcpTable(NULL, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
		if (result != ERROR_INSUFFICIENT_BUFFER) {
			printf("Failed to get buffer size. Error: %lu\n", result);
			Sleep(1000); 
			continue;
		}

		tcpTable = (PMIB_TCPTABLE_OWNER_PID)realloc(tcpTable, size);
		if (tcpTable == NULL) {
			printf("Failed to allocate memory.\n");
			Sleep(1000);
			continue;
		}


		result = GetExtendedTcpTable(tcpTable, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
		if (result != NO_ERROR) {
			printf("Failed to get TCP table. Error: %lu\n", result);
			Sleep(1000);
			continue;
		}

		system("cls"); 
		printf("Monitoring TCP Connections (PID Owner):\n\n");

		for (DWORD i = 0; i < tcpTable->dwNumEntries; i++) {
			print_connection(&tcpTable->table[i]);
		}

		Sleep(2000);
	}

	if (tcpTable) {
		free(tcpTable);
	}
}


