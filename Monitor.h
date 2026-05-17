#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>

void dword_to_ip(DWORD ip, char* buf);
int  fix_port(DWORD port);
void get_process_name(DWORD pid, char* buf);
const char* state_to_string(DWORD state);
void print_connection(MIB_TCPROW_OWNER_PID* row);
void monitor_loop();