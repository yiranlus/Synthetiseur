#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(34, 34, 34);

	int bufferSize		= 128;
	sampleRate 			= 44100;
	volume				= 0.1f;

	sigma = 0.1f;
	numOfFrequencies = 256;
	lowerFrequencyBound = 250.0f;
	upperFrequencyBound = 500.0f;
	frequencyAmplitudes.assign(numOfFrequencies, 0.0f);
	frequencyPhases.assign(numOfFrequencies, 0.0f);

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
	init_frequencyAdders();
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

void ofApp::init_frequencyAdders() {
	frequencyPhaseAdders.assign(numOfFrequencies, 0.0f);
	for (int i = 0; i < numOfFrequencies; i++) {
		float n = ofMap(i, 0, numOfFrequencies, n_freq(lowerFrequencyBound), n_freq(upperFrequencyBound), true);
		float frequency = freq_n(n);
		frequencyPhaseAdders[i] = (frequency / (float)sampleRate) * glm::two_pi<float>();
	}
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
	frequencyAmplitudes.assign(numOfFrequencies, 0.0f);
	for (const auto &key: pressedKeys) {
		activate_frequency(mappedFrequency[key]);
	}
	if (!pressedKeys.empty()) {
		normalize_amplitudes();
	}
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

		ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 350, 0);

		ofSetColor(225);
		ofDrawBitmapString("Frequency", 4, 18);

		ofSetLineWidth(1);
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);

			ofBeginShape();
			for (int i = 0; i < numOfFrequencies; i++) {
				float x =  ofMap(i, 0, numOfFrequencies, 0, 900, true);
				ofVertex(x, 100 - frequencyAmplitudes[i]*100);
			}
			ofEndShape(false);

		ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (mappedFrequency.find(key) == mappedFrequency.end())
		return;
	if (pressedKeys.find(key) != pressedKeys.end())
		return;
    pressedKeys.insert(key);
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
    for (size_t i = 0; i < buffer.getNumFrames(); i++){
        float sample = 0.0f;
		for (int i = 0; i < numOfFrequencies; i++) {
			frequencyPhases[i] = fmod(frequencyPhases[i] + frequencyPhaseAdders[i], glm::two_pi<float>());
			sample += frequencyAmplitudes[i] * sin(frequencyPhases[i]) / pressedKeys.size();
		}
        lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume;
        rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume;
    }
}

void ofApp::activate_frequency(float frequency) {
	int n = n_freq(frequency);
	float sigma2 = sigma*sigma;
	float coeff = 1/sqrt(2*TWO_PI*sigma2);

	for (int i = 0; i < numOfFrequencies; i++) {
		float n_i = ofMap(i, 0, numOfFrequencies, n_freq(lowerFrequencyBound), n_freq(upperFrequencyBound), true);
		frequencyAmplitudes[i] += coeff * exp(-(n_i - n)*(n_i - n) / (2 * sigma2));
	}
}

void ofApp::normalize_amplitudes() {
	float maxAmplitude = 0.0f;
	for (int i = 0; i < numOfFrequencies; i++) {
		if (frequencyAmplitudes[i] > maxAmplitude) {
			maxAmplitude = frequencyAmplitudes[i];
		}
	}
	// ofLog(OF_LOG_NOTICE) << "maxAmplitude: " << maxAmplitude;
	for (int i = 0; i < numOfFrequencies; i++) {
		frequencyAmplitudes[i] /= maxAmplitude;
	}
}