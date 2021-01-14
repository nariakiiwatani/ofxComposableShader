#include "ofApp.h"

#include "ofxComposableShaderIntegration_ofxJsonUtils.h"
#include "ofxComposableShaderIntegration_imgui.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofxComposableShader::Settings s;
	s.loadJson(ofLoadJson("settings.json"));
	shader_.setup(s);
	
	gui_.setup();
	
	mesh_ = ofMesh::sphere(100, 16);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWindowSize()/2.f);
	shader_.begin();
	mesh_.drawVertices();
	shader_.end();
	ofPopMatrix();
	
	gui_.begin();
	if(ImGui::Begin("Shader Params")) {
		shader_.gui();
	}
	ImGui::End();
	gui_.end();
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
