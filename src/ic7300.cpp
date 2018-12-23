#include "ic7300.h"

#include <iostream>


IC7300::IC7300()
{
	port = std::make_shared<SerialConnection>();
	port->open();
}

IC7300::~IC7300()
{
	port->close();
}


void IC7300::turnOn()
{
	std::cout << "TURN ON" << std::endl;
	unsigned char start_code = 0xfe;
	for (int i = 0; i < 25; i++) {
		DWORD written = 1;
		written = port->send(&start_code, 1);
		std::cout << "written " << written << " byte(s)" << std::endl;
	}

	unsigned char command = 0x18;
	unsigned char subcommand = 0x1;
	unsigned char buf[] = {
		0xfe,
		0xfe,
		0x94,
		0xe0,
		command,
		subcommand,
		0xfd
	};
	DWORD size = 7;
	DWORD written = 0;

	written = port->send(buf, size);
	std::cout << "written " << written << " byte(s)" << std::endl;
}


void IC7300::turnOff()
{
	unsigned char command = 0x18;
	unsigned char subcommand = 0x0;
	unsigned char buf[] = {
		0xfe,
		0xfe,
		0x94,
		0xe0,
		command,
		subcommand,
		0xfd
	};
	DWORD size = 7;
	DWORD written = 0;

	written = port->send(buf, size);
	std::cout << "Written: " << written << std::endl;
}
