#include "ofxBNO055USBStick.h"

// add an include path C:\boost_1_63_0\include\boost - 1_63
// add an library path C:\boost_1_63_0\lib\x86 or C:\boost_1_63_0\lib\x64

//--------------------------------------------------------------
void ofxBNO055USBStick::setup(){
	// sensor settings
	bno055.setup();
	bno055.startThread();

	// draw settings
	box.set(0.5f, 0.8f, 0.2f);
	cam.setTarget(box);
	cam.setNearClip(0.001f);
	cam.setFarClip(1000.f);
	cam.setDistance(1.f);
	light.setParent(cam);
	cylinder.set(0.01f, 0.5f);
	
	// gui settings
	gui.setup("BNO055 USB-Stick");
	gui.add(acceleration.set("acceleration [m/s^2]", ofVec3f(), ofVec3f(-40.f), ofVec3f(40.f)));
	gui.add(magnetometer.set("magnetometer [uT]", ofVec3f(), ofVec3f(-2048.f), ofVec3f(2048.f)));
	gui.add(gyroscope.set("gyroscope [dps]", ofVec3f(), ofVec3f(-2000.f), ofVec3f(2000.f)));
	gui.add(euler_angles.set("euler_angles [deg.]", ofVec3f(), ofVec3f(0.f, -90.f, -180.f), ofVec3f(360.f, 90.f, 180.f)));
	gui.add(temperature.set("temperature [C]", 0., -100., 100.));
	gui.add(calibration_status.set("calibration_status", ofVec4f(), ofVec4f(0), ofVec4f(3)));
}

//--------------------------------------------------------------
void ofxBNO055USBStick::update(){
	output = bno055.getOutput();

	acceleration = output.acceleration;
	magnetometer = output.magnetometer;
	gyroscope = output.gyroscope * 180.f / PI;
	euler_angles = output.euler_angles * 180.f / PI;
	quaternion = output.quaternion;
	linear_acceleration = output.linear_acceleration;
	gravity_vector = output.gravity_vector;
	temperature = output.temperature;
	calibration_status = output.calibration_status;
}

//--------------------------------------------------------------
void ofxBNO055USBStick::draw(){
	ofBackgroundGradient(ofColor::black, ofColor::grey);
	ofEnableLighting();
	ofEnableDepthTest();
	light.enable();

	cam.begin();
	ofPushMatrix();
	ofMatrix4x4 mat;
	mat.setRotate(output.quaternion);
	ofMultMatrix(mat);
	box.drawAxes(0.5f);
	ofPushStyle();
	ofSetColor(ofColor::white, 200);
	box.draw();
	ofPopStyle();
	
	ofQuaternion quat;
	quat.makeRotate(ofVec3f(0.f, 1.f, 0.f), output.gravity_vector);
	mat.setRotate(quat);
	ofMultMatrix(mat);
	ofPushStyle();
	ofSetColor(ofColor::yellow);
	cylinder.draw();
	ofPopStyle();

	ofPopMatrix();
	cam.end();

	ofDisableLighting();
	ofDisableDepthTest();

	gui.draw();
}
