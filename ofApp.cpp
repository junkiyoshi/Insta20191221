#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetLineWidth(0.25);
	ofEnableDepthTest();

	ofColor specular_color, diffuse_color;
	ofSetSmoothLighting(true);
	for (int i = 0; i < 255; i += 32) {

		specular_color.setHsb(i, 255, 255);
		diffuse_color.setHsb(i, 255, 128);

		auto light = ofLight();
		light.setSpecularColor(specular_color);
		light.setDiffuseColor(diffuse_color);
		light.enable();

		this->light_list.push_back(light);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	for (auto& light : this->light_list) {

		auto location = glm::vec3(ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.008), 0, 1, -300, 300),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.008), 0, 1, -300, 300),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.008), 0, 1, -300, 300));
		light.setPosition(glm::normalize(location) * 350);
	}

	this->mesh.clear();

	int span = 8;
	int scale = 25;
	int index;
	for (int v = 0; v < 360; v += span) {

		for (int u = -170; u < 250; u += span) {

			index = this->mesh.getNumVertices();

			this->mesh.addVertex(this->make_apple_point(u - span * 0.5, v - span * 0.5) * scale);
			this->mesh.addVertex(this->make_apple_point(u - span * 0.5, v + span * 0.5) * scale);
			this->mesh.addVertex(this->make_apple_point(u + span * 0.5, v + span * 0.5) * scale);

			this->mesh.addIndex(index); this->mesh.addIndex(index + 1); this->mesh.addIndex(index + 2);

			index = this->mesh.getNumVertices();

			this->mesh.addVertex(this->make_apple_point(u - span * 0.5, v - span * 0.5) * scale);
			this->mesh.addVertex(this->make_apple_point(u + span * 0.5, v - span * 0.5) * scale);
			this->mesh.addVertex(this->make_apple_point(u + span * 0.5, v + span * 0.5) * scale);

			this->mesh.addIndex(index); this->mesh.addIndex(index + 1); this->mesh.addIndex(index + 2);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (auto& light : this->light_list) {

		ofSetColor(light.getSpecularColor());
		ofDrawSphere(light.getPosition(), 5);
	}

	ofSetColor(255);
	this->mesh.draw();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_apple_point(float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち リンゴ局面 apple surface P.33 

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	float x = (4 + 3.8 * cos(u)) * cos(v);
	float y = (4 + 3.8 * cos(u)) * sin(v);
	float z = -5 * log10(1 - 0.315 * u) + 5 * sin(u) + 2 * cos(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}