#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//画面基本設定
	ofSetBackgroundAuto(false);
	ofSetFrameRate(30);

	
	ofBackground(255, 255, 255);
	ofEnableSmoothing();

	image1.loadImage("data\\image1.jpg");
	image2.loadImage("data\\image2.jpg");

	imgW = 640;
	imgH = 480;
	image_buff.allocate(imgW, imgH, OF_IMAGE_GRAYSCALE);
	image_mix.allocate(imgW, imgH, OF_IMAGE_COLOR);

	unsigned char *data = image_buff.getPixels();
	for (int y = 0; y < imgH; y++) {
		for (int x = 0; x < imgW; x++) {
			int val = 0;
			data[y * imgW + x] = val;
		}
	}
	image_buff.update();


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	//image1.draw(0, 0);
	//image2.draw(0,0);
	
	unsigned char *data_buff = image_buff.getPixels();

	int circle_r = 30;
	for (int y = 0; y < imgH; y++) {
		for (int x = 0; x < imgW; x++) {

			int circle_x = touchLoc.x-x;
			int circle_y = touchLoc.y-y;

			if((circle_x*circle_x+ circle_y*circle_y<circle_r*circle_r)&&(touchLoc.x!=0)&&(touchLoc.y!=0)){
				int val = 255;
				data_buff[y * imgW + x] = val;
			}
		}
	}
	image_buff.update();
	image_buff.draw(0, 0);


	unsigned char *data_image1 = image1.getPixels();
	unsigned char *data_image2 = image2.getPixels();




}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//ドロー中に
	drawing = true;
	//ひとつ前の座標を記録
	lastTouchLoc = touchLoc;
	//タッチしている場所を記録
	touchLoc.x = x;
	touchLoc.y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//タッチしている場所を記録
	touchLoc.x = x;
	touchLoc.y = y;

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	//ドローを終了
	drawing = false;
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
