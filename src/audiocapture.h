#ifndef _AUDIOCAPTURE_H_
#define _AUDIOCAPTURE_H_

#include <portaudio.h>

class AudioCapture {
public:

	AudioCapture();
	~AudioCapture();

	void enumerateDevices();

	void open(PaDeviceIndex input_device, PaDeviceIndex output_device);
	void close();
	void start();
	void stop();

private:

	static int streamCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

	PaStream *_stream;

	int _dumpFD;
};

#endif // _AUDIOCAPTURE_H_
