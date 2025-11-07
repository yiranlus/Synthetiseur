#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(34, 34, 34);

	int bufferSize		= 1024;
	sampleRate 			= 44100;
	volume				= 0.5f;
	lambda 				= 3;

	baseFrequency = 440.0f;
	soundType = SineSound;

	numOfFrequencies = bufferSize;
	// lowerFrequencyBound = 250.0f;
	// upperFrequencyBound = 510.0f;
	numOfHarmonics = 3;

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
		freqVolumes[key] = 0.0f;
		freqPhases[key] = 0.0f;
		freqPhaseAdderMixers[key] = 0.0f;
	}
}

//--------------------------------------------------------------
void ofApp::init_mapped_frequencies(){
    mappedFrequency['s'] = freq_n(-9, baseFrequency); // C
    mappedFrequency['e'] = freq_n(-8, baseFrequency); // C#
    mappedFrequency['d'] = freq_n(-7, baseFrequency); // D
    mappedFrequency['r'] = freq_n(-6, baseFrequency); // D#
    mappedFrequency['f'] = freq_n(-5, baseFrequency); // E
    mappedFrequency['g'] = freq_n(-4, baseFrequency); // F
    mappedFrequency['y'] = freq_n(-3, baseFrequency); // F#
    mappedFrequency['h'] = freq_n(-2, baseFrequency); // G
    mappedFrequency['u'] = freq_n(-1, baseFrequency); // G#
    mappedFrequency['j'] = freq_n(0, baseFrequency); // A
    mappedFrequency['i'] = freq_n(1, baseFrequency); // A#
    mappedFrequency['k'] = freq_n(2, baseFrequency); // B
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
	// float delta = (upperFrequencyBound - lowerFrequencyBound) / (numOfFrequencies - 1);
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
void ofApp::draw_amplitude() {
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
				ofVertex(x, 100 -lAudio[i]*180.0f / numOfHarmonics);
			}
			ofEndShape(false);

		ofPopMatrix();
	ofPopStyle();
}

void ofApp::draw_frequencies() {
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
			float n1 = n_freq((float) sampleRate * 0.5f);
			for (int i = 1; i < (numOfFrequencies / 2); i++){
				// float x =  ofMap(i, 0, (numOfFrequencies / 2), 0, ofGetWidth()-2*32, false);
				float n = n_freq((float) sampleRate * i / numOfFrequencies);
				float x = ofMap(n, n0, n1, 0, ofGetWidth()-2*32, false);
				ofVertex(x, 190 - frequencyAmp[i]*180.0f * mappedFrequency.size());
			}
			ofEndShape(false);

		ofPopMatrix();
	ofPopStyle();
}

void ofApp::draw_keyboard() {
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
			ofSetColor(150,150,150);
		else
			ofSetColor(255,255,255);
		ofDrawRectRounded(
			x_keyboard + (index * (key_width + padding)),
			y_keyboard,
			key_width,
			5 * key_width,
			rounding
		);
	}

	// Drawing black keys
	for (const auto& [key, index] : mappedBlackKeyIndices)
	{
		if (pressedKeys.find(key) != pressedKeys.end())
			ofSetColor(85,85,85);
		else
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

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetColor(225);
    ofDrawBitmapString("SUPER MEGA SYNTHESIZER OF THE DEAD", 50, 50);

	draw_amplitude();
	draw_frequencies();
	draw_keyboard();

	ofSetColor(225);
	string reportString =
		"volume: ("+ofToString(volume, 2)+") modify with -/+ keys\n" +
		"diminish level: "+ofToString(lambda)+" modify with 9/0 keys\n" +
		"sound type: "+(
			soundType == SineSound?
				"sine":(
					soundType == TriangleSound?
						"triangle":"rectangle"
				)
		)+"\n            toggle with 1/2/3 keys\n" +
		"#harmonics: "+ofToString(numOfHarmonics)+" change with [ and ] keys\n" +
		"base freq: "+ofToString(baseFrequency)+" change with , and . keys";
	ofDrawBitmapString(reportString, 32, 579);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == '-' || key == '_' ){
		volume -= 0.05;
		volume = std::max(volume, 0.f);
	} else if (key == '+' || key == '=' ){
		volume += 0.05;
		volume = std::min(volume, 1.f);
	} else if (key == '1') { // Change sound type
		soundType = SineSound;
	} else if (key == '2') {
		soundType = TriangleSound;
	} else if (key == '3') {
		soundType = RectangleSound;
	} else if (key == '[') { // Change the number of harmonics
		numOfHarmonics --;
		numOfHarmonics = std::max(numOfHarmonics, 1);
	} else if (key == ']') {
		numOfHarmonics ++;
	} else if (key == ',') {
		baseFrequency = freq_n(-1, baseFrequency);
		init_mapped_frequencies();
	} else if (key == '.') {
		baseFrequency = freq_n(1, baseFrequency);
		init_mapped_frequencies();
	} else if (key == '9') {
		lambda --;
		lambda = std::max(lambda, 0.0f);
	} else if (key == '0') {
		lambda ++;
		lambda = std::min(lambda, 10.0f);
	}

	if (mappedFrequency.find(key) == mappedFrequency.end())
		return;
	if (pressedKeys.find(key) != pressedKeys.end())
		return;

    pressedKeys.insert(key);
    // freqPhases[key] = 0.0f;
	freqVolumes[key] = 1.0f;
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

//--------------------------------------------------------------

float triangle(float phase) {
	if (phase < TWO_PI / 4.0f) {
		return (2.0f / PI) * phase;
	} else if (phase < 3.0f * TWO_PI / 4.0f) {
		return - (2.0f / PI) * (phase - TWO_PI / 2.0f);
	} else {
		return (2.0f / PI) * (phase - TWO_PI);
	}
}

float rectangle(float phase, float margin = 0.1f) {
	if (phase < margin) {
		return 1.0f / margin * phase;
	} else if (phase < TWO_PI / 2.0f - margin) {
		return 1.0f;
	} else if (phase < TWO_PI / 2.0f + margin) {
		return -1.0f / margin * (phase - TWO_PI / 2.0f);
	} else if (phase < TWO_PI - margin) {
		return -1.0f;
	} else {
		return 1.0f / margin * (phase - TWO_PI);
	}
}

void ofApp::audioOut(ofSoundBuffer & buffer){
    for (size_t i = 0; i < buffer.getNumFrames(); i++){
        float sample = 0.0f;
        for (const auto &[key, freq] : mappedFrequency) {
			freqPhaseAdderMixers[key] = 0.95*freqPhaseAdderMixers[key] + 0.05*freqPhaseAdders[key];

			// Diminish the phase if the key is not pressed
			if (pressedKeys.find(key) == pressedKeys.end()) {
				freqVolumes[key] *= exp(-lambda * 1.0f / sampleRate);
			}

            freqPhases[key] = fmod(freqPhases[key] + freqPhaseAdderMixers[key], TWO_PI);

			for (int h = 1; h <= numOfHarmonics; h++) {
				switch (soundType) {
				case SineSound:
					sample += freqVolumes[key] * sin(h * freqPhases[key]) / mappedFrequency.size();
					break;
				case TriangleSound:
					sample += freqVolumes[key] * triangle(h * freqPhases[key]) / mappedFrequency.size() / (numOfHarmonics);
					break;
				case RectangleSound:
					sample += freqVolumes[key] * rectangle(h * freqPhases[key]) / mappedFrequency.size() / (numOfHarmonics * numOfHarmonics * numOfHarmonics);
					break;
				}
			}
        }
        lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume;
        rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume;
    }
}