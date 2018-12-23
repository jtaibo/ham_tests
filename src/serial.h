#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <windows.h>

class SerialConnection {

public:
	SerialConnection();	// to-do: configuration params
	~SerialConnection();

	void open();
	void close();
	int send(LPCVOID buffer, DWORD size);
	int receive(LPVOID buffer, DWORD size);

private:

	HANDLE hPort;

};

#endif // _SERIAL_H_
