#include "ofMain.h"
#include "ofApp.h"


/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <cstring>
#include "Leap.h"

using namespace Leap;

ofApp app;

class SampleListener : public Listener {
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);

private:
};

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

bool volumeChange = false;
int palmY = 0;
int palmX = 0;
int swipeYstart = 0;
int volumeDiff = 0;

bool prevVolChange = false;
bool curVolChange = false;
Frame previousFrame;

bool prevSpeedChange = false;
bool curSpeedChange = false;
int speedID = 0;

bool pinch = false;

int swipeID = 0;

int volumeID = 0;

void SampleListener::onInit(const Controller& controller) {
	/*std::cout << "Initialized" << std::endl;*/
}

void SampleListener::onConnect(const Controller& controller) {
	/*std::cout << "Connected" << std::endl;*/
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	/*std::cout << "Disconnected" << std::endl;*/
}

void SampleListener::onExit(const Controller& controller) {
	/*std::cout << "Exited" << std::endl;*/
}

void SampleListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	/*std::cout << "Frame id: " << frame.id()
		<< ", timestamp: " << frame.timestamp()
		<< ", hands: " << frame.hands().count()
		<< ", extended fingers: " << frame.fingers().extended().count()
		<< ", tools: " << frame.tools().count()
		<< ", gestures: " << frame.gestures().count() << std::endl;*/

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		/*std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
			<< ", palm position: " << hand.palmPosition() << std::endl;*/
		palmY = hand.palmPosition()[1];
		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		// Calculate the hand's pitch, roll, and yaw angles
		/*std::cout << std::string(2, ' ') << "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
			<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
			<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;*/

		// Get the Arm bone
		Arm arm = hand.arm();
		/*std::cout << std::string(2, ' ') << "Arm direction: " << arm.direction()
			<< " wrist position: " << arm.wristPosition()
			<< " elbow position: " << arm.elbowPosition() << std::endl;*/

		// Get fingers
		const FingerList fingers = hand.fingers();
		//float strength = hand.grabStrength();
		//if (fingers[0].isExtended() && !fingers[1].isExtended() && !fingers[2].isExtended() && !fingers[3].isExtended() && !fingers[4].isExtended() && strength == 1.0)
		//{
		//	//std::cout<< "yes." << std::endl;
		//	prevVolChange = curVolChange;
		//	curVolChange = true;
		//	if (curVolChange != prevVolChange){
		//		// std::cout<< "prev frame id. " << frame.id() << std::endl;
		//		previousFrame = frame;
		//		// std::cout<< "frame id. " << previousFrame.id() << std::endl;
		//	}
		//	else{
		//		Vector rotationAxis = frame.rotationAxis(previousFrame);
		//		float rotationAnglez = frame.rotationAngle(previousFrame, Leap::Vector::zAxis());
		//		// std::cout << "previous frame: " << previousFrame.id() << std::endl;
		//		 std::cout << "rotation Axis: " << rotationAxis << " rotation Angle: z " << rotationAnglez << std::endl;

		//	}
		//}
		//else{
		//	curVolChange = false;
		//}

		//Below fist-thumb thing
		float pinchStrength = hand.pinchStrength();
		if (!fingers[0].isExtended() && fingers[1].isExtended() && !fingers[2].isExtended() && !fingers[3].isExtended() && fingers[4].isExtended() && pinchStrength == 1.0){
			//std::cout << "WE PINCHING " << std:: endl;
			pinch = true;
		}
		else pinch = false;


		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			/*std::cout << std::string(4, ' ') << fingerNames[finger.type()]
				<< " finger, id: " << finger.id()
				<< ", length: " << finger.length()
				<< "mm, width: " << finger.width() << std::endl;*/

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				/*std::cout << std::string(6, ' ') << boneNames[boneType]
					<< " bone, start: " << bone.prevJoint()
					<< ", end: " << bone.nextJoint()
					<< ", direction: " << bone.direction() << std::endl;*/
			}
		}
	}

	// Get tools
	const ToolList tools = frame.tools();
	for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
		const Tool tool = *tl;
		/*std::cout << std::string(2, ' ') << "Tool, id: " << tool.id()
			<< ", position: " << tool.tipPosition()
			<< ", direction: " << tool.direction() << std::endl;*/
	}

	// Get gestures
	const GestureList gestures = frame.gestures();
	for (int g = 0; g < gestures.count(); ++g) {
		Gesture gesture = gestures[g];

		switch (gesture.type()) {
		case Gesture::TYPE_CIRCLE:
		{
			CircleGesture circle = gesture;
			std::string clockwiseness;

			//same code as you sent me, replace that block of code with this one
			if (frame.fingers().extended().count() >= 5 /*&& (int)(circle.progress()*10)%5==0*/){
				bool isLeft = circle.pointable().hand().isLeft();
				// if(!circle.pointable().hand().isLeft()){

				if (!isLeft){
					prevSpeedChange = curSpeedChange;
					curSpeedChange = true;
				}
				else {
					prevVolChange = curVolChange;
					curVolChange = true;
				}
				if (stateNames[gesture.state()] == "STATE_START"){
					if (!isLeft)
						speedID = gesture.id();

					else
						volumeID = gesture.id();
				}

				else if (stateNames[gesture.state()] == "STATE_UPDATE"){

					if (circle.pointable().direction().angleTo(circle.normal()) <= PI / 2) {
						clockwiseness = "clockwise";
						if (circle.normal().angleTo(circle.normal().down()) <= PI / 4){
							if (!isLeft && speedID == gesture.id()){
								std::cout << "right circle cw" << std::endl;
								app.speedUp(circle.radius());
							}

							else if (volumeID == gesture.id()){
								std::cout << "left circle cw" << std::endl;
								app.xFade2();
								//left hand circle clockwise, "increase" volume
							}
						}
					}
					else {
						clockwiseness = "counterclockwise";
						if (circle.normal().angleTo(circle.normal().up()) <= PI / 4){
							if (!isLeft && speedID == gesture.id()){
								std::cout << "right circle ccw" << std::endl;
								app.speedDown(circle.radius());
							}
							else if (volumeID == gesture.id()){
								std::cout << "left circle ccw" << std::endl;
								app.xFade1();
								//left hand circle counterclockwise, "decrease" volume
							}
						}
					}
				}
				else if (stateNames[gesture.state()] == "STATE_END"){
					curSpeedChange = false;
					curVolChange = false;
				}

			}
			// Calculate angle swept since last frame
			float sweptAngle = 0;
			if (circle.state() != Gesture::STATE_START) {
				CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
				sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
			}
			/*std::cout << std::string(2, ' ')
				<< "Circle id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", progress: " << circle.progress()
				<< ", radius: " << circle.radius()
				<< ", angle " << sweptAngle * RAD_TO_DEG
				<< ", " << clockwiseness << std::endl;*/
			break;
		}
		case Gesture::TYPE_SWIPE:
		{
			SwipeGesture swipe = gesture;
			if (frame.fingers().extended().count() == 1){

				if (stateNames[gesture.state()] == "STATE_START"){
					swipeID = gesture.id();
					//swipe.pointable().hand().finger(tap.pointable().id()).type();
				}

				else if (stateNames[gesture.state()] == "STATE_UPDATE"){

				}
				else
					if (stateNames[gesture.state()] == "STATE_END"){
					if (std::abs(swipe.direction()[0]) > std::abs(swipe.direction()[1]) && swipeID == gesture.id()/*&& swipe.pointable().hand().finger(swipe.pointable().id()).type() == 1*/){


						if (swipe.direction()[0] < 0.0){
							//swiping left
							app.changeSong1();
							std::cout << "swiped left" << gesture.id() << std::endl;
						}
						else{
							//swiping right
							std::cout << "swiped right" << gesture.id() << std::endl;
							app.changeSong2();
						}

					}
					}

			}
			/*if (frame.fingers().extended().count() == 2){
				std::cout << "Two fingers swiping, volume" << std::endl;

				if (stateNames[gesture.state()] == "STATE_START"){
					swipeYstart = palmY;
					volumeChange = true;
					std::cout << "Swipe Y start :" << swipeYstart << palmY<< std::endl;
				}

				else if (stateNames[gesture.state()] == "STATE_END"){
					volumeDiff = palmY - swipeYstart;
					volumeChange = false;
					std::cout << "Diff: " << volumeDiff << std::endl;
				}
			}*/
			/*std::cout << std::string(2, ' ')
				<< "Swipe id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", direction: " << swipe.direction()
				<< ", speed: " << swipe.speed() << std::endl;*/
			break;
		}
		case Gesture::TYPE_KEY_TAP:
		{
			KeyTapGesture tap = gesture;
			
			//in Key Tap Gesture
			if (pinch == true){
				std::cout << "reset speed to one" << std::endl;
				app.resetSpeed();
				// reset speed to one or whatever you want this to do
			}

			else {
				cout << tap.pointable().hand().finger(tap.pointable().id()).type() << endl;
				//tap.pointable().hand().finger(tap.pointable().id).type();
				int num = tap.pointable().hand().finger(tap.pointable().id()).type();
				app.tapBeat(num);
			}
			/*std::cout << std::string(2, ' ')
				<< "Key Tap id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", position: " << tap.position()
				<< ", direction: " << tap.direction() << std::endl;*/
			break;
		}
		case Gesture::TYPE_SCREEN_TAP:
		{
			ScreenTapGesture screentap = gesture;
			/*std::cout << std::string(2, ' ')
				<< "Screen Tap id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", position: " << screentap.position()
				<< ", direction: " << screentap.direction() << std::endl;*/
			break;
		}
		default:
			/*std::cout << std::string(2, ' ') << "Unknown gesture type." << std::endl;*/
			break;
		}
	}

	if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
		/*std::cout << std::endl;*/
	}

}

void SampleListener::onFocusGained(const Controller& controller) {
	/*std::cout << "Focus Gained" << std::endl;*/
}

void SampleListener::onFocusLost(const Controller& controller) {
	/*std::cout << "Focus Lost" << std::endl;*/
}

void SampleListener::onDeviceChange(const Controller& controller) {
	/*std::cout << "Device Changed" << std::endl;*/
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		/*std::cout << "id: " << devices[i].toString() << std::endl;*/
		/*std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;*/
	}
}

void SampleListener::onServiceConnect(const Controller& controller) {
	/*std::cout << "Service Connected" << std::endl;*/
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	/*std::cout << "Service Disconnected" << std::endl;*/
}

int main(int argc, char** argv) {


	// Create a sample listener and controller
	SampleListener listener;
	Controller controller;

	// Have the sample listener receive events from the controller
	controller.addListener(listener);
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
	app = ofApp();
	ofRunApp(&app);
	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
	
	// Keep this process running until Enter is pressed
	/*std::cout << "Press Enter to quit..." << std::endl;*/
	std::cin.get();

	// Remove the sample listener when done
	controller.removeListener(listener);

	return 0;
}
//========================================================================

	

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	

