#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(34, 34, 34);

	int bufferSize		= 128;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	// bNoise 				= false;

	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);

	soundStream.printDeviceList();

	ofSoundStreamSettings settings;

	// if you want to set the device id to be different than the default:
	//
	//	auto devices = soundStream.getDeviceList();
	//	settings.setOutDevice(devices[3]);

	// you can also get devices for an specific api:
	//
	//	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
	//	settings.setOutDevice(devices[0]);

	// or get the default device for an specific api:
	//
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name:
	//
	//	auto devices = soundStream.getMatchingDevices("default");
	//	if(!devices.empty()){
	//		settings.setOutDevice(devices[0]);
	//	}


	// Latest linux versions default to the HDMI output
	// this usually fixes that. Also check the list of available
	// devices if sound doesn't work

	//settings.setApi(ofSoundDevice::MS_ASIO);
	//settings.setApi(ofSoundDevice::MS_WASAPI);
	//settings.setApi(ofSoundDevice::MS_DS);

	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setOutDevice(devices[0]);
	}

	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

	// on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
	// use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior
	// to loading a file.

    // setup the keyboard mapping
    init_mapped_frequencies();
}

//--------------------------------------------------------------
void ofApp::init_mapped_frequencies(){
    mappedFrequency['s'] = 261.63f; // C
    mappedFrequency['e'] = 277.18f; // C#
    mappedFrequency['d'] = 293.66f; // D
    mappedFrequency['r'] = 311.13f; // D#
    mappedFrequency['f'] = 329.63f; // E
    mappedFrequency['g'] = 349.23f; // F
    mappedFrequency['y'] = 369.99f; // F#
    mappedFrequency['h'] = 392.00f; // G
    mappedFrequency['u'] = 415.30f; // G#
    mappedFrequency['j'] = 440.00f; // A
    mappedFrequency['i'] = 466.16f; // A#
    mappedFrequency['k'] = 493.88f; // B
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (mappedFrequency.find(key) == mappedFrequency.end())
        return;
    pressedKeys.insert(key);
    freqPhases[key] = 0.0f;
    freqPhaseAdders[key] = (mappedFrequency[key] / (float) sampleRate) * glm::two_pi<float>();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    pressedKeys.erase(key);
    freqPhases.erase(key);

    if (pressedKeys.empty()) {
        phase = 0.0f;
        phaseAdderTarget = 0.0f;
    }
}

void ofApp::audioOut(ofSoundBuffer & buffer){
	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-glm::two_pi<float>() like this:

    for (size_t i = 0; i < buffer.getNumFrames(); i++){
        float sample = 0.0f;
        for (const auto &key: pressedKeys) {
            freqPhases[key] = fmod(freqPhases[key] + freqPhaseAdders[key], glm::two_pi<float>());
            sample += sin(freqPhases[key]) / pressedKeys.size();
        }
        lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume;
        rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume;
    }
}