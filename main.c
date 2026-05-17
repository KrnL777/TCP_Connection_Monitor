#include "Monitor.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>


int main() {
    monitor_loop();
    return 0;
}