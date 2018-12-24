#include "audiocapture.h"

#include <io.h>
#include <fcntl.h>

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
	for (int i = 0; i < numDevices; i++)
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


void AudioCapture::open(PaDeviceIndex input_device, PaDeviceIndex output_device)
{
	PaError err;
	PaStreamParameters inputParameters;
	PaStreamParameters outputParameters;

#define NUM_INPUT_CHANNELS 1
#define NUM_OUTPUT_CHANNELS 0
#define PA_SAMPLE_TYPE paFloat32;
#define SAMPLE_RATE 48000
#define FRAMES_PER_BUFFER paFramesPerBufferUnspecified

	/* -- setup input and output -- */
//	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	inputParameters.device = input_device;
	inputParameters.channelCount = NUM_INPUT_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
//	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	outputParameters.device = output_device;
	outputParameters.channelCount = NUM_OUTPUT_CHANNELS;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	/* -- setup stream -- */
	err = Pa_OpenStream(
		&_stream,
		&inputParameters,
		NULL, //&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
//		NULL, /* no callback, use blocking API */
//		NULL); /* no callback, so no callback userData */
		&streamCallback,
		this
		);
	if (err != paNoError) {
		std::cerr << "Error in Pa_OpenStream : " << Pa_GetErrorText(err) << std::endl;
	}

	_dumpFD = _open("C:/Users/jtaibo/Documents/devel/ham_tests/data/dump.raw",
		_O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY,
		_S_IREAD | _S_IWRITE);
	if (_dumpFD < 0) {
		std::cerr << "ERROR opening dump file" << std::endl;
	}
}

void AudioCapture::close()
{
	/* -- don't forget to cleanup! -- */
	PaError err = Pa_CloseStream(_stream);
	if (err != paNoError) {
		std::cerr << "Error in Pa_CloseStream : " << Pa_GetErrorText(err) << std::endl;
	}
	_close(_dumpFD);
}

void AudioCapture::start()
{
	/* -- start stream -- */
	PaError err = Pa_StartStream(_stream);
	if (err != paNoError) {
		std::cerr << "Error in Pa_StartStream : " << Pa_GetErrorText(err) << std::endl;
	}
	std::cout << "Wire on. Will run one minute" << std::endl;
#if 0
	/* -- Here's the loop where we pass data from input to output -- */
	for (int i = 0; i < (60 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i)
	{
		err = Pa_WriteStream(_stream, sampleBlock, FRAMES_PER_BUFFER);
		if (err) goto xrun;
		err = Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
		if (err) goto xrun;
	}
#endif
}

void AudioCapture::stop()
{
	/* -- Now we stop the stream -- */
	PaError err = Pa_StopStream(_stream);
	if (err != paNoError) {
		std::cerr << "Error in Pa_StopStream" << Pa_GetErrorText(err) << std::endl;
	}
}


int AudioCapture::streamCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
//	std::cout << "callback!" << std::endl;
	float *inbuf = (float *)inputBuffer;
	float max = 0;
	float min = 0;
	for (unsigned long i = 0; i < framesPerBuffer; i++) {
//		std::cout << inbuf[i] << std::endl;
		if (inbuf[i] < min)
			min = inbuf[i];
		if (inbuf[i] > max)
			max = inbuf[i];
	}
	std::cout << "min: " << min << std::endl;
	std::cout << "max: " << max << std::endl;

	AudioCapture *ac = (AudioCapture *)userData;
	std::cout << "Writing " << framesPerBuffer << " " << framesPerBuffer * sizeof(float) << " byte(s)" << std::endl;
	int written = _write(ac->_dumpFD, (const void *)inbuf, framesPerBuffer * sizeof(float));
	std::cout << written << " byte(s) written" << std::endl;

	return 0;
}
