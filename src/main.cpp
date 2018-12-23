#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include <iostream>

#include "ic7300.h"
#include "audiocapture.h"


int main(int argc, char *argv[])
{
	std::cout << "Hello!" << std::endl;

#if 1
	std::shared_ptr<AudioCapture> audio = std::make_shared<AudioCapture>();

	audio->enumerateDevices();
#endif

#if 0
	std::cout << "Opening port... ";
	std::shared_ptr<IC7300> xcvr = std::make_shared<IC7300>();
	std::cout << "OK" << std::endl;

	Sleep(1000);
	xcvr->turnOn();
	Sleep(3000);
	xcvr->turnOff();
	Sleep(1000);
#endif

	return 0;
}
