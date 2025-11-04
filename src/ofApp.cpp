#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup()
{
ofBackground(50, 50, 50);

int bufferSize		= 512;
sampleRate 			= 44100;
phase 				= 0;
phaseAdder 			= 0.0f;
phaseAdderTarget 	= 0.0f;
volume				= 0.1f;
bNoise 				= false;

hit_white_keys = {0,0,0,0,0,0,0};
hit_black_keys = {0,0,0,0,0};

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
    for (int i = 0; i < 7; i++)
    {
		if (hit_white_keys[i]==0)
		{
			ofSetColor(255,255,255);
			ofDrawRectRounded(
				x_keyboard + (i * (key_width + padding)), 
				y_keyboard, 
				key_width, 
				5 * key_width, 
				rounding
			);
		}
		else
		{
			ofSetColor(150,150,150);
			ofDrawRectRounded(
				x_keyboard + (i * (key_width + padding)), 
				y_keyboard, 
				key_width, 
				5 * key_width, 
				rounding
			);
		}
        
    }
    
    // Drawing black keys
    for (int i = 0; i < 6; i++)
    {
        if (i != 2)
        {
			int j = i;
			if (j>2)
			{
				j = j-1;
			}
			if (hit_black_keys[j]==0)
			{
				ofSetColor(15,15,15);
				ofDrawRectRounded(
					x_keyboard + (1.5 * key_width + padding)/2 + (i * (key_width + padding)),
					y_keyboard, 
					key_width / 2, 
					7 * key_width / 2,
					rounding
				);
			}
			else
			{
				ofSetColor(85,85,85);
				ofDrawRectRounded(
					x_keyboard + (1.5 * key_width + padding)/2 + (i * (key_width + padding)),
					y_keyboard, 
					key_width / 2, 
					7 * key_width / 2,
					rounding
				);
			}
            
        }
    }


}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// White keys
	if (key == 's')
	{
		hit_white_keys[0] = 1;
	}
	if (key == 'd')
	{
		hit_white_keys[1] = 1;
	}
	if (key == 'f')
	{
		hit_white_keys[2] = 1;
	}
	if (key == 'g')
	{
		hit_white_keys[3] = 1;
	}
	if (key == 'h')
	{
		hit_white_keys[4] = 1;
	}
	if (key == 'j')
	{
		hit_white_keys[5] = 1;
	}
	if (key == 'k')
	{
		hit_white_keys[6] = 1;
	}

	// Black keys
	if (key == 'e')
	{
		hit_black_keys[0] = 1;
	}
	if (key == 'r')
	{
		hit_black_keys[1] = 1;
	}
	if (key == 'y')
	{
		hit_black_keys[2] = 1;
	}
	if (key == 'u')
	{
		hit_black_keys[3] = 1;
	}
	if (key == 'i')
	{
		hit_black_keys[4] = 1;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (key == 's')
	{
		hit_white_keys[0] = 0;
	}
	if (key == 'd')
	{
		hit_white_keys[1] = 0;
	}
	if (key == 'f')
	{
		hit_white_keys[2] = 0;
	}
	if (key == 'g')
	{
		hit_white_keys[3] = 0;
	}
	if (key == 'h')
	{
		hit_white_keys[4] = 0;
	}
	if (key == 'j')
	{
		hit_white_keys[5] = 0;
	}
	if (key == 'k')
	{
		hit_white_keys[6] = 0;
	}

	// Black keys
	if (key == 'e')
	{
		hit_black_keys[0] = 0;
	}
	if (key == 'r')
	{
		hit_black_keys[1] = 0;
	}
	if (key == 'y')
	{
		hit_black_keys[2] = 0;
	}
	if (key == 'u')
	{
		hit_black_keys[3] = 0;
	}
	if (key == 'i')
	{
		hit_black_keys[4] = 0;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
