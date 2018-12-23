#ifndef _IC7300_H_
#define _IC7300_H_

#include "serial.h"

#include <memory>

class IC7300 {

public:

	IC7300();
	~IC7300();

	void turnOn();
	void turnOff();

private:

	std::shared_ptr<SerialConnection> port;

};

#endif //_IC7300_H_
