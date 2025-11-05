#pragma once

#include <vector>
#include <set>
#include <map>
using std::vector;
using std::set;
using std::map;

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void draw_keys();
		// void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		// void mouseMoved(int x, int y ) override;
		// void mouseDragged(int x, int y, int button) override;
		// void mousePressed(int x, int y, int button) override;
		// void mouseReleased(int x, int y, int button) override;
		// void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		// void mouseEntered(int x, int y) override;
		// void mouseExited(int x, int y) override;
		// void windowResized(int w, int h) override;
		// void dragEvent(ofDragInfo dragInfo) override;
		// void gotMessage(ofMessage msg) override;
		void init_mapped_frequencies();
		void init_mappedWhiteKeyIndices();
		void init_mappedBlackKeyIndices();
		void audioOut(ofSoundBuffer & buffer) override;
		void drawSound();
		
	private:
		ofSoundStream soundStream;
		int sampleRate;
		float phase;
		float phaseAdder;
		float phaseAdderTarget;

		set<int> pressedKeys;
		map<int, float> mappedFrequency;
		map<int, float> freqPhases;
		map<int, float> freqPhaseAdders;
		map<int, int> mappedWhiteKeyIndices;
		map<int, int> mappedBlackKeyIndices;
		
		vector<float> lAudio;
		vector<float> rAudio;
		float volume;
};
