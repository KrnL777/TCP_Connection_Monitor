# TCP Connection Monitor

A lightweight Windows TCP connection monitoring tool written in C.

This project enumerates active TCP connections in real time using the Windows IP Helper API and displays detailed socket information, including:

- Local Address
- Remote Address
- Local Port
- Remote Port
- Connection State
- Owning Process ID (PID)

---

## Features

- Enumerates active TCP connections
- IPv4 support
- Real-time monitoring
- Displays owning process IDs
- Lightweight native implementation
- Uses the WinAPI (`GetExtendedTcpTable`)

---

## Technologies

- C
- WinAPI
- IP Helper API
- Windows Networking APIs
