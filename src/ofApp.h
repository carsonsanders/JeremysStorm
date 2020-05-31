#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class Sprite {
public:
	Sprite();

	void draw();
	void update();
	void startAnim();
	void stopAnim();
	void advanceFrame();
	void integrate();
	void addForce(ofVec3f f);

	glm::vec3 pos, cPos;
	ofVec3f force;
	ofVec3f velocity;
	ofVec3f acceleration;
	vector<ofVec3f> forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   birthtime;
	float   age();
	float width, height, voff, hoff;
	float cWidth, cHeight; // for use with collision
	ofImage image;
	bool isBow = false;
	bool moves = false;
	bool animates = true;
	bool useGrav = false;

	int ntiles_x, ntiles_y;
	int nframes;
	int frame = 0;
	int row = 0;
	int col = 0;
	bool bAnimRunning = false;
	float lastTimeRec;
	bool isColliding(Sprite s);

};

class spriteSystem {
public:
	vector<Sprite> sprites;
	void addSprite(Sprite s);
	void draw();
	void update();
	void checkCollision(spriteSystem);
};



class Emitter {
public:
	void emit();
	void update();
	void draw();
	spriteSystem system;
	ofImage spriteImage;

	bool animates = true;
	bool mushroom = false;
	bool arrow = false;
	bool eye = false;
	ofVec3f velocity;
	glm::vec3 pos;
	vector<Sprite> sprites;
	bool gameOver = false;
	

};

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

		ofImage background;
		ofImage backgroundMountain;
		ofImage foregroundMountain;
		ofImage backgroundTrees;
		ofImage foregroundTrees;

		ofImage foregroundTrees2;
		ofImage backgroundTrees2;
		ofImage foregroundMountain2;
		ofImage backgroundMountain2;

		ofImage horseRun;
		ofImage horseJump;

		ofImage arrowImage;

		ofImage mushroom;
		ofImage eye;

		ofVec3f treePosition;
		ofVec3f treePosition2;
		ofVec3f bgTreePosition;
		ofVec3f bgTreePosition2;
		ofVec3f mountainPosition;
		ofVec3f mountainPosition2;
		ofVec3f bgMountainPosition;
		ofVec3f bgMountainPosition2;

		ofVec3f bowPosition;
		float bowRot;

		ofxPanel gui;
		ofxFloatSlider speed;

		Sprite player;
		Sprite bow;
		Sprite arrow;

		spriteSystem mushroomSystem;
		spriteSystem arrowSystem;
		spriteSystem eyeSystem;

		Emitter mushroomEmitter;
		Emitter arrowEmitter;
		Emitter eyeEmitter;

		int score = 0;
		int enemyRate = 5;
		bool justSpawned = false;
		bool phase2;
		
		ofSoundPlayer drawBow;
		ofSoundPlayer fireBow;
		ofSoundPlayer music;

		bool gameOver = false;

};
