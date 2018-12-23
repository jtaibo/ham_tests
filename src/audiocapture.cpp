#include "audiocapture.h"

#include <iostream>

AudioCapture::AudioCapture()
{
	std::cout << "Constructing AudioCapture" << std::endl;
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
		throw -1;
	}
	std::cout << "Poraudio version: " << Pa_GetVersionText() << std::endl;
}

AudioCapture::~AudioCapture()
{
	std::cout << "Destructing AudioCapture" << std::endl;
	PaError err = Pa_Terminate();
	if (err != paNoError) {
		std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
		return;
	}
}

void AudioCapture::enumerateDevices()
{
	PaError err;
	int numDevices;
	numDevices = Pa_GetDeviceCount();
	if (numDevices < 0)
	{
		std::cout << "ERROR: Pa_CountDevices returned " << numDevices << std::endl;
		err = numDevices;
		return;
	}

	const   PaDeviceInfo *deviceInfo;
	for (unsigned int i = 0; i < numDevices; i++)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		std::cout << "Device #" << i << std::endl;
		std::cout << "  structVersion            : " << deviceInfo->structVersion << std::endl;
		std::cout << "  name                     : " << deviceInfo->name << std::endl;
		std::cout << "  hostApi                  : " << deviceInfo->hostApi << std::endl;
		std::cout << "  maxInputChannels         : " << deviceInfo->maxInputChannels << std::endl;
		std::cout << "  maxOutputChannels        : " << deviceInfo->maxOutputChannels << std::endl;
		std::cout << "  defaultLowInputLatency   : " << deviceInfo->defaultLowInputLatency << std::endl;
		std::cout << "  defaultLowOutputLatency  : " << deviceInfo->defaultLowOutputLatency << std::endl;
		std::cout << "  defaultHighInputLatency  : " << deviceInfo->defaultHighInputLatency << std::endl;
		std::cout << "  defaultHighOutputLatency : " << deviceInfo->defaultHighOutputLatency << std::endl;
		std::cout << "  defaultSampleRate        : " << deviceInfo->defaultSampleRate << std::endl;
		std::cout << "=====================================================================" << std::endl;
		//...
	}

}