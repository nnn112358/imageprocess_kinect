#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

		ofPoint touchLoc; //���݃^�b�`���Ă���ꏊ
		ofPoint lastTouchLoc; //�ЂƂO�̃^�b�`���Ă���ꏊ
		bool drawing; //�h���[�����ǂ���
		float interpolate; //�⊮�̊���

		int imgW ;
		int imgH ;
		ofImage image1;
		ofImage image2;
		ofImage image_buff;
		ofImage image_mix;

};
