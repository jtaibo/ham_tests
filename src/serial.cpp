#include "serial.h"

#include <string>
#include <iostream>

std::string port = "COM3";

SerialConnection::SerialConnection()
{
	std::cout << "Constructing SerialConnection" << std::endl;
}

SerialConnection::~SerialConnection()
{
	std::cout << "Destructing SerialConnection" << std::endl;
}

/**
 *	@todo Manage exceptions
 */
void SerialConnection::open()
{
	DCB dcb;

	hPort = CreateFile(
		port.c_str(),							// file name
		GENERIC_READ | GENERIC_WRITE,	// desired access
		0,								// shared mode
		NULL,							// security attributes
		OPEN_EXISTING,					// creation disposition
		0,								// flags and attributes
		NULL							// template file
	);

	if (hPort == INVALID_HANDLE_VALUE) {
		std::cout << "Error opening com port. Error code: " << GetLastError() << std::endl;
		throw -1;
	}

	if (!GetCommState(hPort, &dcb)) {
		std::cout << "GetCommState error" << std::endl;
		throw -1;
	}
	dcb.BaudRate = CBR_19200; //19200 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = TWOSTOPBITS; //2 stop
	if (!SetCommState(hPort, &dcb)) {
		std::cout << "SetCommState error" << std::endl;
		throw -1;
	}
}

void SerialConnection::close()
{
	CloseHandle(hPort);		// Close the handle
}

int SerialConnection::send(LPCVOID buffer, DWORD size)
{
	DWORD written = -1;
	WriteFile(hPort, buffer, size, &written, NULL);
	return written;
}

int SerialConnection::receive(LPVOID buffer, DWORD size)
{
	DWORD read = -1;
	ReadFile(hPort, buffer, size, &read, NULL);
	return read;
}
