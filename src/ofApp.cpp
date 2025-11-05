#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(34, 34, 34);

	int bufferSize		= 128;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;

	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);

	soundStream.printDeviceList();

	ofSoundStreamSettings settings;

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

	// setup the keyboard mapping
    init_mapped_frequencies();
	init_mappedWhiteKeyIndices();
	init_mappedBlackKeyIndices();
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
void ofApp::init_mappedWhiteKeyIndices(){
	mappedWhiteKeyIndices = {
		{'s', 0},
		{'d', 1},
		{'f', 2},
		{'g', 3},
		{'h', 4},
		{'j', 5},
		{'k', 6}
	};
}

//--------------------------------------------------------------
void ofApp::init_mappedBlackKeyIndices(){
	mappedBlackKeyIndices = {
		{'e', 0},
		{'r', 1},
		{'y', 3}, // skipping 2 due to black keys layout
		{'u', 4},
		{'i', 5}
	};
}
//--------------------------------------------------------------
void ofApp::update()
{

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(225);
    ofDrawBitmapString("SUPER MEGA SYNTHESIZER OF THE DEAD", 50, 50);

    // Setting keyboard properties
    int x_keyboard = 200;
    int y_keyboard = 800;
    int key_width = 40;
    int padding = 4;
    int rounding = 5;

	// Drawing white keys
	ofFill();
	for (const auto& [key, index] : mappedWhiteKeyIndices)
	{
		if (pressedKeys.find(key) != pressedKeys.end())
		{
			ofSetColor(150,150,150);
			ofDrawRectRounded(
				x_keyboard + (index * (key_width + padding)),
				y_keyboard,
				key_width,
				5 * key_width,
				rounding
			);
		}
		else
		{
			ofSetColor(255,255,255);
			ofDrawRectRounded(
				x_keyboard + (index * (key_width + padding)),
				y_keyboard,
				key_width,
				5 * key_width,
				rounding
			);
		}
	}

	// Drawing black keys
	for (const auto& [key, index] : mappedBlackKeyIndices)
	{
		if (pressedKeys.find(key) != pressedKeys.end())
		{
			ofSetColor(85,85,85);
			ofDrawRectRounded(
				x_keyboard + (1.5 * key_width + padding)/2 + (index * (key_width + padding)),
				y_keyboard,
				key_width / 2,
				7 * key_width / 2,
				rounding
			);
		}
		else
		{
			ofSetColor(15,15,15);
			ofDrawRectRounded(
				x_keyboard + (1.5 * key_width + padding)/2 + (index * (key_width + padding)),
				y_keyboard,
				key_width / 2,
				7 * key_width / 2,
				rounding
			);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (mappedFrequency.find(key) == mappedFrequency.end())
		return;
	if (pressedKeys.find(key) != pressedKeys.end())
		return;
    pressedKeys.insert(key);
    freqPhases[key] = 0.0f;
    freqPhaseAdders[key] = (mappedFrequency[key] / (float) sampleRate) * glm::two_pi<float>();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	pressedKeys.erase(key);
}

//--------------------------------------------------------------
// void ofApp::mouseMoved(int x, int y ){

// }

// //--------------------------------------------------------------
// void ofApp::mouseDragged(int x, int y, int button){

// }

// //--------------------------------------------------------------
// void ofApp::mousePressed(int x, int y, int button){

// }

// //--------------------------------------------------------------
// void ofApp::mouseReleased(int x, int y, int button){

// }

// //--------------------------------------------------------------
// void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

// }

// //--------------------------------------------------------------
// void ofApp::mouseEntered(int x, int y){

// }

// //--------------------------------------------------------------
// void ofApp::mouseExited(int x, int y){

// }

// //--------------------------------------------------------------
// void ofApp::windowResized(int w, int h){

// }

// //--------------------------------------------------------------
// void ofApp::gotMessage(ofMessage msg){

// }

// //--------------------------------------------------------------
// void ofApp::dragEvent(ofDragInfo dragInfo){

// }

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