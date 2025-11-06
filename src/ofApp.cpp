#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(34, 34, 34);

	int bufferSize		= 1024;
	sampleRate 			= 44100;
	volume				= 1.0f;

	numOfFrequencies = bufferSize;
	lowerFrequencyBound = 250.0f;
	upperFrequencyBound = 510.0f;

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

	for (const auto& [key, freq] : mappedFrequency) {
		freqPhases[key] = 0.0f;
		freqPhaseAdderMixers[key] = 0.0f;
	}
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
	float delta = (upperFrequencyBound - lowerFrequencyBound) / (numOfFrequencies - 1);
	frequencyAmp.assign(numOfFrequencies, 0.0f);

	vector<float> ampRealPart(numOfFrequencies);
	vector<float> ampImagPart(numOfFrequencies);
	for (int k = 0; k < numOfFrequencies; k++) {
		for (int n = 0; n < numOfFrequencies; n++) {
			ampRealPart[k] += lAudio[n] * cos(- TWO_PI * k * n / numOfFrequencies);
			ampImagPart[k] += lAudio[n] * sin(- TWO_PI * k * n / numOfFrequencies);
		}
		ampRealPart[k] /= numOfFrequencies;
		ampImagPart[k] /= numOfFrequencies;
		frequencyAmp[k] = sqrt(
			ampRealPart[k] * ampRealPart[k] +
			ampImagPart[k] * ampImagPart[k]
		);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(225);
    ofDrawBitmapString("SUPER MEGA SYNTHESIZER OF THE DEAD", 50, 50);

	// Draw the signal amplitude
	ofNoFill();
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 64, 0);

		ofSetColor(225);
		ofDrawBitmapString("Amplitude", 4, 18);

		ofSetLineWidth(1);
		ofDrawRectangle(0, 0, ofGetWidth()-2*32, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);

			ofBeginShape();
			for (unsigned int i = 0; i < lAudio.size(); i++){
				float x =  ofMap(i, 0, lAudio.size(), 0, ofGetWidth()-2*32, false);
				ofVertex(x, 100 -lAudio[i]*180.0f / mappedFrequency.size());
			}
			ofEndShape(false);

		ofPopMatrix();
	ofPopStyle();

	// ----------------------------------------------------------
	ofNoFill();
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 296, 0);

		ofSetColor(225);
		ofDrawBitmapString("Frequencies", 4, 18);

		ofSetLineWidth(1);
		ofDrawRectangle(0, 0, ofGetWidth()-2*32, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);

			ofBeginShape();
			float n0 = n_freq((float) sampleRate / numOfFrequencies);
			float n1 = n_freq((float) sampleRate * (numOfFrequencies - 1) / numOfFrequencies);
			for (int i = 1; i < numOfFrequencies - 1; i++){
				// float x =  ofMap(i, 0, numOfFrequencies, 0, ofGetWidth()-2*32, false);
				float n = n_freq((float) sampleRate * i / numOfFrequencies);
				float x =  ofMap(n, n0, n1, 0, ofGetWidth()-2*32, false);
				ofVertex(x, 190 - frequencyAmp[i]*180.0f / volume);
			}
			ofEndShape(false);

		ofPopMatrix();
	ofPopStyle();

	// ----------------------------------------------------------

    // Setting keyboard properties
    int key_width = 40;
    int padding = 4;
    int rounding = 5;
	int x_keyboard = (float) ofGetWidth()/2 - (float) key_width * mappedWhiteKeyIndices.size() / 2;
    int y_keyboard = 512;

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
	freqPhaseAdders[key] = 0.0f;
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
    for (size_t i = 0; i < buffer.getNumFrames(); i++){
        float sample = 0.0f;
        for (const auto &[key, freq] : mappedFrequency) {
			freqPhaseAdderMixers[key] = 0.95*freqPhaseAdderMixers[key] + 0.05*freqPhaseAdders[key];
			float newPhase = freqPhases[key];

			// Diminish the phase if the key is not pressed
			if (pressedKeys.find(key) == pressedKeys.end()) {
				newPhase *= 0.95;
			}
            freqPhases[key] = fmod(newPhase + freqPhaseAdderMixers[key], glm::two_pi<float>());
            sample += sin(freqPhases[key]);
        }
        lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume;
        rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume;
    }
}