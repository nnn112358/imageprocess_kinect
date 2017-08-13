#include "ofApp.h"
#include "read_file.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(0, 0, 0);

	width = 640*1.0;
	height = 480 * 1.0;

	depth_width = 640;
	depth_height = 480;
	ofFloatColor minFloatColor = ofFloatColor(0, 0, 0, 1);
	ofFloatColor maxFloatColor = ofFloatColor(1, 1, 1, 1);

	gui.setup();
	gui.add(screenSize.setup("screen size", ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight())));
	gui.add(doFullScreen.set("fullscreen (F)", false));
	doFullScreen.addListener(this, &ofApp::setFullScreen);
	gui.add(p1.setup("dissipation", 0.99, 0.90, 1.0));
	gui.add(p2.setup("vel-dissipation", 0.80, 0.50, 1.0));
	gui.add(p3.setup("L_Force", 5.0, 1.0, 30.0));
	gui.add(p4.setup("R_Force", 5.0, 1.0, 30.0));
	gui.add(p9.setup("C_Force", 5.0, 1.0, 30.0));
	gui.add(p10.setup("velocity", 10, 00, 100));


	gui.add(lHand_draw.set("lHand_draw", true));
	//gui.add(rr.setup("L_Color_R", 1.0, 0.0, 1.0));
	//gui.add(gg.setup("L_Color_G", 0.50, 0.0, 1.0));
	//gui.add(bb.setup("L_Color_B", 0.0, 0.0, 1.0));
	gui.add(lHand_color.setup("lHand_color", minFloatColor, minFloatColor, maxFloatColor));

	gui.add(rHand_draw.set("rHand_draw", true));
	//gui.add(rr2.setup("R_Color_R", 0.0, 0.0, 1.0));
	//gui.add(gg2.setup("R_Color_G", 0.50, 0.0, 1.0));
	//gui.add(bb2.setup("R_Color_B", 1.0, 0.0, 1.0));
	gui.add(rHand_color.setup("rHand_color", minFloatColor, minFloatColor, maxFloatColor));

	gui.add(head_draw.set("head_draw", false));
	gui.add(Head_color.setup("Head_color", minFloatColor, minFloatColor, maxFloatColor));
	gui.add(Hip_draw.set("Hip_draw", false));
	gui.add(Hip_color.setup("Hip_color", minFloatColor, minFloatColor, maxFloatColor));

	gui.add(lFoot_draw.set("lFoot_draw", false));
	gui.add(lFoot_color.setup("lFoot_color", minFloatColor, minFloatColor, maxFloatColor));

	gui.add(rFoot_draw.set("rFoot_draw", false));
	gui.add(rFoot_color.setup("rFoot_color", minFloatColor, minFloatColor, maxFloatColor));

	gui.add(depth_draw.set("depth_draw", false));
	gui.add(Skeleton_draw.set("Skeleton_draw", false));

	gui.add(p5.setup("depth_dis", 125.0, 0.0, 255.0));

	gui.add(p6.setup("depth_ave", 0.2, 0.0, 1.0));
	//	gui.add(p7.setup("px", 0, -50, +50));
	//	gui.add(p8.setup("py", 0, -50, +50));




	gui.loadFromFile("settings.xml");

	kinect.initSensor(0);
	//kinect.initIRStream(640, 480);
	kinect.initColorStream(width, height, true);
	kinect.initDepthStream(depth_width, depth_height, true);
	kinect.initSkeletonStream(false);
	//simple start
	kinect.start();

	//ofDisableAlphaBlending(); //Kinect alpha channel is default 0;
	ofEnableAlphaBlending();

	ofSetCircleResolution(64);

	float winW_fluid = 1920;
	float winH_fluid = 1080;

	fluid.allocate(winW_fluid, winH_fluid, 0.5);
	fluid.dissipation = p1;
	fluid.velocityDissipation = p2;
	fluid.setGravity(ofVec2f(0.0, 0.00));

	//ofSetWindowShape(width, height);

	video_buff = new unsigned char[width*height * 3]; //反転映像用の変数
	videoTexture.allocate(width, height, GL_RGB);    //反転映像用のテクスチャを配置
	//videoTexture.setAnchorPercent(0.5, 0.5);

	depth_Inv = new unsigned char[width*height]; //反転映像用の変数
	depth_buff = new unsigned char[width*height]; //反転映像用の変数
	depthTexture.allocate(width, height, GL_LUMINANCE);    //反転映像用のテクスチャを配置

	video_cut = new unsigned char[width*height * 3]; //反転映像用の変数
	videoTexture_cut.allocate(width, height, GL_RGB);    //反転映像用のテクスチャを配置

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int w = width;
			int h = height;
			video_cut[y * 3 * w + x * 3 + 0] = 0;
			video_cut[y * 3 * w + x * 3 + 1] = 0;
			video_cut[y * 3 * w + x * 3 + 2] = 0;
		}
	}
	hasSkeleton = false;
	window_ratio = 1.0;

}

//--------------------------------------------------------------
void ofApp::update() {
	kinect.update();

	kinectImage = kinect.getColorPixelsRef();
	depthImage = kinect.getDepthPixelsRef();
	//ofShortPixels rawDepthPix = getRawDepthPixelsRef(); ///< raw 11 bit values

	unsigned char *pixels = kinectImage.getPixels();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int w = width;
			int h = height;

			//それぞれのピクセルのr, g, bを抽出
			video_buff[y * 3 * w + x * 3] = pixels[y * 4 * w + x * 4 + 2];			//赤
			video_buff[y * 3 * w + x * 3 + 1] = pixels[y * 4 * w + x * 4 + 1];	//緑
			video_buff[y * 3 * w + x * 3 + 2] = pixels[y * 4 * w + x * 4 + 0];	//青
		}
	}
	videoTexture.loadData(video_buff, width, height, GL_RGB);

	unsigned char *d_pixels = depthImage.getPixels();

	for (int i = 0; i < depth_height*depth_width; i++) {
		if ((d_pixels[i] > 0) && (d_pixels[i] < 255))	depth_buff[i] = d_pixels[i] * p6 + depth_buff[i] * (1.0 - p6);
		//		else depth_buff[i] = 0;
	}

	for (int i = 0; i < depth_height*depth_width; i++) {
		depth_Inv[i] = 255 - depth_buff[i];
		//	if (depth_Inv[i] < p5)		depth_Inv[i] = 0;
		//	if (depth_Inv[i] > 254)		depth_Inv[i] = 0;
	}


	depthTexture.loadData(depth_Inv, depth_width, depth_height, GL_LUMINANCE);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int w = width;
			int h = height;
			//それぞれのピクセルのr, g, bを抽出
			int dw = depth_width;
			int dh = depth_height;
			int dx = x*dw / w;
			int dy = y*dh / h;

			int d_pixel = depth_Inv[dy * dw + dx];
			long cx;
			long cy;

			NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL, x, y, 0, &cx, &cy);
			long cx2 = cx;
			long cy2 = cy;
			if ((d_pixel != 0) && (cx > 0) && (cx < width) && (cy > 0) && (cy < height) && (cx2 > 0) && (cx2 < width) && (cy2 > 0) && (cy2 < height)) {
				video_cut[cy2 * 3 * w + cx2 * 3 + 0] = video_buff[cy * 3 * w + cx * 3 + 0];
				video_cut[cy2 * 3 * w + cx2 * 3 + 1] = video_buff[cy * 3 * w + cx * 3 + 1];
				video_cut[cy2 * 3 * w + cx2 * 3 + 2] = video_buff[cy * 3 * w + cx * 3 + 2];
			}
			else if ((cx > 0) && (cx < width) && (cy > 0) && (cy < height) && (cx2 > 0) && (cx2 < width) && (cy2 > 0) && (cy2 < height)) {
				video_cut[cy2 * 3 * w + cx2 * 3 + 0] = 0;
				video_cut[cy2 * 3 * w + cx2 * 3 + 1] = 0;
				video_cut[cy2 * 3 * w + cx2 * 3 + 2] = 0;
			}

		}
	}
	videoTexture_cut.loadData(video_cut, width, height, GL_RGB);


	// Adding temporal Force

	if (kinect.isNewSkeleton()) {
		for (int i = 0; i < kinect.getSkeletons().size(); i++)
		{
			if (kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HEAD) != kinect.getSkeletons().at(i).end())
			{

				// just get the first one
				SkeletonBone HeadBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HEAD)->second;
				SkeletonBone lHandBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HAND_LEFT)->second;
				SkeletonBone rHandBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HAND_RIGHT)->second;

				SkeletonBone HipBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HIP_CENTER)->second;
				SkeletonBone rFoorBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_FOOT_RIGHT)->second;
				SkeletonBone lFootBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_FOOT_LEFT)->second;

				ofVec3f hb(HeadBone.getScreenPosition().x, HeadBone.getScreenPosition().y, 0);
				head = head.getInterpolated(hb, 0.5);
				head.z = ofInterpolateCosine(head.z, HeadBone.getStartPosition().x, 0.5) + 0.1;

				ofVec3f lhb(lHandBone.getScreenPosition().x, lHandBone.getScreenPosition().y, 0);
				lHand = lHand.getInterpolated(lhb, 0.5);
				lHand.z = ofInterpolateCosine(lHand.z, lHandBone.getStartPosition().x, 0.5);

				ofVec3f rhb(rHandBone.getScreenPosition().x, rHandBone.getScreenPosition().y, 0);
				rHand = rHand.getInterpolated(rhb, 0.5);
				rHand.z = ofInterpolateCosine(rHand.z, rHandBone.getStartPosition().x, 0.5);

				ofVec3f hhb(HipBone.getScreenPosition().x, HipBone.getScreenPosition().y, 0);
				Hip = Hip.getInterpolated(hhb, 0.5);
				Hip.z = ofInterpolateCosine(Hip.z, HipBone.getStartPosition().x, 0.5);

				ofVec3f lfhb(lFootBone.getScreenPosition().x, lFootBone.getScreenPosition().y, 0);
				lFoot = lFoot.getInterpolated(lfhb, 0.5);
				lFoot.z = ofInterpolateCosine(lFoot.z, lFootBone.getStartPosition().x, 0.5);

				ofVec3f rfhb(rFoorBone.getScreenPosition().x, rFoorBone.getScreenPosition().y, 0);
				rFoot = rFoot.getInterpolated(rfhb, 0.5);
				rFoot.z = ofInterpolateCosine(rFoot.z, rFoorBone.getStartPosition().x, 0.5);


				hasSkeleton = true;
			}
		}
	}

	fluid.dissipation = p1;
	fluid.velocityDissipation = p2;


	double forceL = p3;
	double forceR = p4;
	double forceC = p9;


	if (rHand_draw == true) {
		ofPoint m = ofPoint(lHand.x*(double)width*window_ratio / (double)640.0, lHand.y*(double)height*window_ratio / (double)480.0);
		ofPoint d = (m - oldM)*p10;
		oldM = m;
		//  Update
		fluid.addTemporalForce(m, d, lHand_color, (double)forceL);
	}
	
	if (lHand_draw == true) {
		ofPoint m2 = ofPoint(rHand.x*(double)width*window_ratio / (double)640.0, rHand.y*(double)height*window_ratio / (double)480.0);
		ofPoint d2 = (m2 - oldM2)*p10;
		oldM2 = m2;
		fluid.addTemporalForce(m2, d2, rHand_color, (double)forceR);
	}

	if (lFoot_draw == true) {
		ofPoint m = ofPoint(lFoot.x*(double)width*window_ratio / (double)640.0, lFoot.y*(double)height*window_ratio / (double)480.0);
		ofPoint d = (m - oldM3)*p10;
		oldM3 = m;
		fluid.addTemporalForce(m, d, lFoot_color, (double)forceL);
	}
	if (rFoot_draw == true) {
		ofPoint m = ofPoint(rFoot.x*(double)width*window_ratio / (double)640.0, rFoot.y*(double)height*window_ratio / (double)480.0);
		ofPoint d = (m - oldM4)*p10;
		oldM4 = m;
		fluid.addTemporalForce(m, d, rFoot_color, (double)forceR);
	}
	if (head_draw == true) {
		ofPoint m = ofPoint(head.x*(double)width*window_ratio / (double)640.0, head.y*(double)height*window_ratio / (double)480.0);
		ofPoint d = (m - oldM5)*p10;
		oldM5 = m;
		fluid.addTemporalForce(m, d, Head_color, (double)forceC);
	}
	if (Hip_draw == true) {
		ofPoint m = ofPoint(Hip.x*(double)width*window_ratio / (double)640.0, Hip.y*(double)height*window_ratio / (double)480.0);
		ofPoint d = (m - oldM6)*p10;
		oldM6 = m;
		fluid.addTemporalForce(m, d, Hip_color, (double)forceC);
	}

	fluid.update();

	ofSetWindowTitle(ofToString(ofGetFrameRate()));

}

//--------------------------------------------------------------
void ofApp::viewSkeletons() {

	ofPushStyle();
	ofSetColor(255, 0, 0);
	ofSetLineWidth(3.0f);
	auto skeletons = kinect.getSkeletons();

	for (auto & skeleton : skeletons) {
		for (auto & bone : skeleton) {
			switch (bone.second.getTrackingState()) {
			case SkeletonBone::Inferred:
				ofSetColor(0, 0, 255);
				break;
			case SkeletonBone::Tracked:
				ofSetColor(0, 255, 0);
				break;
			case SkeletonBone::NotTracked:
				ofSetColor(255, 0, 0);
				break;
			}

			auto index = bone.second.getStartJoint();
			auto connectedTo = skeleton.find((_NUI_SKELETON_POSITION_INDEX)index);
			if (connectedTo != skeleton.end()) {
			//	if (index != 1) {
					ofLine(connectedTo->second.getScreenPosition()*window_ratio, bone.second.getScreenPosition()*window_ratio);
			//	}
			}

			ofCircle(bone.second.getScreenPosition()*window_ratio, 10.0f);
		}
	}
	ofPopStyle();
}


void ofApp::draw() {

	//ofEnableAlphaBlending();

	//vidGrabber.draw(20, 20);
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	//kinectImage.draw(0, 0);
	//kinect.draw(0, 0);

	//  get size
	float winW = ofGetWidth(),
		winH = ofGetHeight(),
		imgW = videoTexture.getWidth(),
		imgH = videoTexture.getHeight();
	//  fit it
	float ratioW = winW / imgW,
		ratioH = winH / imgH,
		ratioMin;
	if (ratioW < ratioH)
		ratioMin = ratioW;
	else
		ratioMin = ratioH;
	window_ratio = ratioMin;

	videoTexture.draw(0, 0, ratioMin*imgW, ratioMin*imgH);


	//videoTexture.draw(0, 0, width, height);
	//videoTexture_cut.draw(width, height, width, height);
	//videoTexture_cut.draw(0, 0, width, height);
	//videoTexture.draw(width, height, width, height);
	//depthTexture.draw(width, 0, depth_width, depth_height);

	if (depth_draw == true)
		depthTexture.draw(winW - depth_width, winH - depth_height, depth_width, depth_height);
	//depthImage.draw(winW - depth_width, winH- depth_height, depth_width, depth_height);

//	kinect.drawDepth(width, 0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	//ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	ofPushStyle();
	fluid.draw();
	ofPopStyle();



	if (capture_flg==true){
		capture_img.grabScreen(0, 0, ratioMin*imgW, ratioMin*imgH);
		capture_img.save("screenshot.png");

		char fileNameStr[1024];
		string date = ofGetTimestampString();//タイムスタンプをファイル名にする
		sprintf(fileNameStr, "%s.png", date.c_str());
		capture_img.save(fileNameStr, OF_IMAGE_QUALITY_BEST);
		capture_flg = false;

		ofSetColor(255, 255, 255);
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		Sleep(100);
	
	}

	if(Skeleton_draw){
		viewSkeletons();
	}
	if (!bHide) {
		gui.draw();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'h') {
		bHide = !bHide;
	}
	else if (key == 's') {
		gui.saveToFile("settings.xml");
	}
	else if (key == 'l') {
		gui.loadFromFile("settings.xml");
	}

	else if (key == 'r') {
		dissipation_p1 = p1;
		p1=0.0;
	}

	else if (key == 'f') {
		doFullScreen.set(!doFullScreen.get());
	}

	if (key == 'x') {
		capture_flg = true;
		Sleep(100);
	

	}


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'r') {
		 p1= dissipation_p1;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
