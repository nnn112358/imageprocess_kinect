#pragma once
#include "ofxKinectCommonBridge.h"

#include "ofMain.h"
#include "ofxFluid.h"

#include "ofxGui.h"

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
		
		void viewSkeletons();

		ofxKinectCommonBridge kinect;

		ofImage kinectImage;
		ofImage depthImage;
		unsigned char * 	video_buff;
		ofTexture			videoTexture;

		unsigned char * 	depth_buff;
		unsigned char * 	depth_Inv;
		ofTexture			depthTexture;

		unsigned char * 	video_cut;
		ofTexture			videoTexture_cut;


		ofxFluid fluid;
		//ofxFluid fluid2;

		int     width, height;
		int     depth_width, depth_height;
		ofVec2f oldM;
		ofVec2f oldM2;
		ofVec2f oldM3;
		ofVec2f oldM4;
		ofVec2f oldM5;
		ofVec2f oldM6;

		ofxPanel gui;
		bool bHide;
		double dissipation_p1;

		ofxFloatSlider p1, p2, p3, p4, p5, p6,p7,p8,p9,p10,p11,p12;
		ofxFloatSlider rr2, gg2, bb2;
		ofxFloatSlider rr,gg,bb;
		ofVec3f head, lHand, rHand;
		
		ofxFloatColorSlider lHand_color;
		ofxFloatColorSlider  rHand_color;
		ofxFloatColorSlider  Head_color;
		ofxFloatColorSlider  Hip_color;
		ofxFloatColorSlider  rFoot_color;
		ofxFloatColorSlider  lFoot_color;


		ofVec3f Hip, lFoot, rFoot;

		bool hasSkeleton;
		ofParameter<bool> depth_draw;
		ofParameter<bool> Skeleton_draw;

		ofParameter<bool> head_draw;
		ofParameter<bool> Hip_draw;
		ofParameter<bool> lFoot_draw;
		ofParameter<bool> rFoot_draw;
		ofParameter<bool> lHand_draw;
		ofParameter<bool> rHand_draw;
		
		ofParameter<bool>	doFullScreen;
		void				setFullScreen(bool& _value) { ofSetFullscreen(_value); }
		ofxLabel screenSize;

		double window_ratio;

		ofImage     capture_img;
		bool     capture_flg;

};
