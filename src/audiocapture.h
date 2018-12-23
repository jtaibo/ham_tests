#ifndef _AUDIOCAPTURE_H_
#define _AUDIOCAPTURE_H_

#include <portaudio.h>

class AudioCapture {
public:

	AudioCapture();
	~AudioCapture();

	void enumerateDevices();

private:

	//

};

#endif // _AUDIOCAPTURE_H_
