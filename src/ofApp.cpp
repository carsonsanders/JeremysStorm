#include "ofApp.h"

//-------------------------------------Sprite Functions---------------------------------------------------------
Sprite::Sprite() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	force.set(0, 0, 0);
	lifespan = 500000;
	birthtime = 0;
	damping = .99;
	mass = 1;
}

void Sprite::startAnim() {

	frame = 0;
	bAnimRunning = true;
	lastTimeRec = ofGetSystemTimeMillis();
}

void Sprite::stopAnim() {
	bAnimRunning = false;
}

void Sprite::update() {
	if (!bAnimRunning) return;

	float curTime = ofGetSystemTimeMillis();
	if ((curTime - lastTimeRec) > 50) {
		advanceFrame();
		lastTimeRec = curTime;
	}

	if (moves) {
		integrate();
	}
}


void Sprite::advanceFrame() {
	if (frame == (nframes - 1)) {
		if (!isBow) {
			// cycle back to first frame
			col = 0;
			row = 0;
			frame = 0;
		}
	}
	else {
		frame++;
		if (col == ntiles_x - 1) col = 0; else col++;
		row = frame / ntiles_x;
	}
}
void Sprite::draw() {
	if (animates) {
		image.drawSubsection(pos.x, pos.y, width, height, col*width + hoff, row*height + voff, width, height);
	}
	else {
		image.draw(pos);
	}
}

void Sprite::integrate() {

	// check for 0 framerate to avoid divide errors
	//
	float framerate = ofGetFrameRate();

	// avoid accumulating forces on startup before frame is drawn
	//
	if (framerate < 5.0)
	{
		force.set(0, 0, 0);
		return;
	}

	// interval for this step
	//
	float dt = 1.0 / framerate;

	// update position based on velocity
	//
	pos += (velocity * dt);

	//update force to include all forces acting on the particle
	for (int i = 0; i < forces.size(); i++) {
		force += forces[i];
	}

	if (useGrav) {
	//Gravity Force
		if (pos.y > 0) {
			force.y += 10 * mass;
		}

		//Turbulence Force, Kicks in a little above ground
		if (pos.y > 0.5) {
			force.x += ofRandom(-0.5, 0.5);
			force.y += ofRandom(-0.5, 0.5);
			force.z += ofRandom(-0.5, 0.5);
		}
	}
	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the particle
	accel += (force * (1.0 / mass));
	velocity += accel * dt;


	// add a little damping for good measure
	//
	//velocity *= damping;

	// clear forces on particle (they get re-added each step)
	//
	force.set(0, 0, 0);
}

void Sprite::addForce(ofVec3f t) {
	forces.push_back(t);
}

bool Sprite::isColliding(Sprite s) {

	float xDistance = (s.pos.x - pos.x);
	float yDistance = (s.pos.y - pos.y);
	ofVec3f Distance = ofVec3f(xDistance, yDistance, 0);

	float yContactDistance = (s.cHeight / 2 + cHeight / 2) ;
	float xContactDistance = (s.cWidth / 2 + cWidth / 2) ;

	//cout << xContactDistance << " " << yContactDistance << " ";
	//cout << " xContactD: " << Distance.x << "ycontactD:" << Distance.y;

	if ( abs(Distance.x) < xContactDistance && abs(Distance.y)  < yContactDistance) {
		return true;
	}
	else return false;
}
//------------------------------------------Sprite System Functions-----------------------------------------------------
//  Add a Sprite to the Sprite System
//
void spriteSystem::addSprite(Sprite s) {
	sprites.push_back(s);
}



//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void spriteSystem::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	
	//Kill out of bounds sprites
	for (int j = 0; j < sprites.size(); j++) {
		if (sprites[j].pos.x > 1024 || sprites[j].pos.x < 0 || sprites[j].pos.y > 768 || sprites[j].pos.y < 0) {
			sprites[j].lifespan = 0;
		}
	}
	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		
		if (s->lifespan == 0) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].update();
	}
}

//  Render all the sprites
//
void spriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}

void spriteSystem::checkCollision(spriteSystem s) {
	for (int i = 0; i < sprites.size(); i++) {
		for (int j = 0; j < s.sprites.size(); j++) {
			if (sprites[i].isColliding(s.sprites[j])) {
				sprites[i].lifespan = 0;
				s.sprites[j].lifespan = 0;
				//s.sprites[j].lifespan = 0;
			}
		}
	}
}
//------------------------------------------Emitter Functions-----------------------------------------------------------
void Emitter::emit() {
	Sprite sprite;
	sprite.velocity.set(velocity);
	sprite.acceleration.set(200);
	sprite.image = spriteImage;
	sprite.moves = true;
	sprite.pos = pos;
	sprite.animates = animates;
	if (arrow) sprite.useGrav = true;
	if (arrow) sprite.cWidth = 10;
	if (arrow) sprite.cHeight = 2;
	if (arrow) sprite.cPos = pos;

	
	if (mushroom) {
		sprite.acceleration.set(0);
		sprite.ntiles_x = 4;
		sprite.ntiles_y = 3;
		sprite.nframes = 8;
		sprite.height = 150;
		sprite.width = 150;
		sprite.cHeight = 150;
		sprite.cWidth = 150;
		sprite.cPos.x = pos.x + 50;
		sprite.cPos.y = pos.y + 50;
		sprite.voff = 0;
		sprite.hoff = 0;
		
	}

	if (eye) {
		sprite.acceleration.set(0);
		sprite.ntiles_x = 4;
		sprite.ntiles_y = 2;
		sprite.nframes = 8;
		sprite.height = 150;
		sprite.width = 150;
		sprite.cHeight = 150;
		sprite.cWidth = 150;
		sprite.voff = 0;
		sprite.hoff = 0;
	}
	
	sprite.startAnim();

	system.addSprite(sprite);
}

void Emitter::update() {

	// Changes animation to attack if near player for mushroom dudes
	if (mushroom) {
		for (int i = 0; i < system.sprites.size(); i++) {
			if (system.sprites[i].pos.x <= 188) {
				system.sprites[i].voff = 300;
			}
			if (system.sprites[i].pos.x <= 150) {
				gameOver = true;
			}
			
		}
	}
	if (eye) {
		for (int i = 0; i < system.sprites.size(); i++) {
			if (system.sprites[i].pos.x <= 190) {
				system.sprites[i].voff = 300;
			}
			if (system.sprites[i].pos.x <= 150) {
				gameOver = true;
			}
		}
	}

	system.update();


}

void Emitter::draw() {
	
	system.draw();
}


//--------------------------------------------------------------
void ofApp::setup(){

	//Load Background Images
	background.load("images/backgroundresized.png");
	backgroundMountain.load("images/backgroundMountainresized.png");
	backgroundMountain2.load("images/backgroundMountainresized.png");
	foregroundMountain.load("images/foregroundMountainresized.png");
	foregroundMountain2.load("images/foregroundMountainresized.png");
	backgroundTrees.load("images/backgroundTreesresized.png");
	backgroundTrees2.load("images/backgroundTreesresized.png");
	foregroundTrees.load("images/foregroundTreesresized.png");
	foregroundTrees2.load("images/foregroundTreesresized.png");

	

	//Set foreground tree positions
	treePosition = ofVec3f(0, 0, 0);
	treePosition2 = ofVec3f(1024, 0, 0);
	bgTreePosition = ofVec3f(0, 0, 0);
	bgTreePosition2 = ofVec3f(1024, 0, 0);
	mountainPosition = ofVec3f(0, 0, 2);
	mountainPosition2 = ofVec3f(1024, 0, 0);
	bgMountainPosition = ofVec3f(0, 0, 0);
	bgMountainPosition2 = ofVec3f(1024, 0, 0);

	//gui setup
	gui.setup();
	gui.add(speed.setup("speed", 3, 1, 100));

	//Player Sprite setup
	horseRun.load("images/horserunonly.png");
	horseJump.load("images/HorseJump.png");
	player.image = horseRun;
	player.pos = glm::vec3(100, 705, 0);
	player.width = 80;
	player.height = 64;
	player.ntiles_x = 2;
	player.ntiles_y = 3;
	player.voff = 0;
	player.hoff = 0;
	player.nframes = 6;
	player.startAnim();

	//Bow sprite setup
	bow.image.load("images/boww.png");
	bow.isBow = true;
	bow.pos = glm::vec3(132, 703, 0);
	bow.width = 35;
	bow.height = 45;
	bow.ntiles_x = 6;
	bow.ntiles_y = 4;
	bow.nframes = 11;
	//bow.startAnim();

	
	//Bow emitter Setup
	arrowImage.load("images/arrow.png");
	arrowImage.resize(52, 18);
	arrowEmitter.spriteImage = arrowImage;
	arrowEmitter.pos = glm::vec3(132, 710, 0);
	arrowEmitter.animates = false;
	arrowEmitter.system = arrowSystem;
	arrowEmitter.arrow = true;


	//Mushroom enemy emitter setup
	mushroom.load("images/mushroommonster.png");
	mushroomEmitter.spriteImage = mushroom;
	mushroomEmitter.pos = glm::vec3(850, 665, 0);
	mushroomEmitter.animates = true;
	mushroomEmitter.mushroom = true;
	mushroomEmitter.velocity = ofVec3f(-225, 0, 0);
	mushroomEmitter.system = mushroomSystem;

	//Eye enemy emitter setup
	eye.load("images/eyemonster.png");
	eyeEmitter.spriteImage = eye;
	eyeEmitter.pos = glm::vec3(900, 300, 0);
	eyeEmitter.animates = true;
	eyeEmitter.eye = true;
	eyeEmitter.velocity = ofVec3f(-225, 90, 0);
	eyeEmitter.system = eyeSystem;

	drawBow.loadSound("sounds/drawbow.wav");
	fireBow.loadSound("sounds/firebow.wav");
	music.loadSound("sounds/music.mp3");
	music.play();
	
	



	

}

//--------------------------------------------------------------
void ofApp::update(){

	//updates foreground tree positions based on speed
	if (treePosition.x <= -1024) treePosition.x = treePosition2.x + 1024;
	treePosition.x -= speed;
	if (treePosition2.x <= -1024) treePosition2.x = treePosition.x + 1024;
	treePosition2.x-= speed;

	//Same for background trees
	if (bgTreePosition.x <= -1024) bgTreePosition.x = bgTreePosition2.x + 1024;
	else bgTreePosition.x -= speed/2;
	if (bgTreePosition2.x <= -1024) bgTreePosition2.x = bgTreePosition.x + 1024;
	else bgTreePosition2.x -= speed/2;

	//Same for foreground mountains
	if (mountainPosition.x <= -1024) mountainPosition.x = mountainPosition2.x + 1024;
	else mountainPosition.x -= speed / 6;
	if (mountainPosition2.x <= -1024) mountainPosition2.x = mountainPosition.x + 1024;
	else mountainPosition2.x -= speed / 6;

	//..and background mountains
	if (bgMountainPosition.x <= -1024) bgMountainPosition.x = bgMountainPosition2.x + 1024;
	else bgMountainPosition.x -= speed / 12;
	if (bgMountainPosition2.x <= -1024) bgMountainPosition2.x = bgMountainPosition.x + 1024;
	else bgMountainPosition2.x -= speed / 12;

	mushroomEmitter.system.checkCollision(arrowEmitter.system);
	arrowEmitter.system.checkCollision(mushroomEmitter.system);

	eyeEmitter.system.checkCollision(arrowEmitter.system);
	arrowEmitter.system.checkCollision(eyeEmitter.system);

	if (player.frame >= 15) {
		player.image = horseRun;
		player.width = 80;
		player.height = 64;
		player.ntiles_x = 2;
		player.ntiles_y = 3;
		player.nframes = 6;
		player.pos = glm::vec3(100, 705, 0);

		player.frame = 0;

	}
	player.update();

	if (bow.frame >= 23) {
		bow.stopAnim();
	}
	bow.update();

	float xDistance = ofGetMouseX() - bow.pos.x;
	float yDistance = ofGetMouseY() - bow.pos.y;

	bowRot = atan2(yDistance, xDistance) * 180 / PI;

	arrowEmitter.update();
	mushroomEmitter.update();
	eyeEmitter.update();
	float time = ofGetElapsedTimef();
	int roundedTime = round(time);
	
	if (roundedTime % enemyRate == 0) {
		
		if (!justSpawned) {
			mushroomEmitter.emit();
		}

		if (!justSpawned && phase2 && (roundedTime % enemyRate * 2 == 0)) {
			eyeEmitter.emit();
		}
		

		if ( (roundedTime % (enemyRate * 4) == 0) && !justSpawned)  {
			eyeEmitter.emit();
			
		}

		if ((roundedTime % (enemyRate * 10) == 0) && !justSpawned) {
			if (enemyRate > 2) {
				enemyRate--;
				cout << enemyRate;
			}
			else {
				phase2 = true;
			}
		}

		

		justSpawned = true;
	}
	else justSpawned = false;

	if (mushroomEmitter.gameOver || eyeEmitter.gameOver) {
		gameOver = true;
	}
	


	
}

//--------------------------------------------------------------
void ofApp::draw(){
	//Draw background
	background.draw(ofVec3f(0,0,0));
	backgroundMountain.draw(bgMountainPosition);
	backgroundMountain2.draw(bgMountainPosition2);
	foregroundMountain.draw(mountainPosition);
	foregroundMountain2.draw(mountainPosition2);
	backgroundTrees.draw(bgTreePosition);
	backgroundTrees2.draw(bgTreePosition2);
	foregroundTrees.draw(treePosition);
	foregroundTrees2.draw(treePosition2);

	//Draw player
	if (!gameOver) {
		player.draw();
	}

	//draw gui
	//gui.draw();

	
	//draw bow angled toward mouse
	bow.image.setAnchorPoint(bow.pos.x,bow.pos.y + 22.5);

	ofPushMatrix();
	ofTranslate(bow.pos.x, bow.pos.y + 22.5);
	ofRotate(bowRot);
	if (!gameOver) bow.draw();
	ofPopMatrix();
	
	//Draw sprites from emitters
	arrowEmitter.draw();
	mushroomEmitter.draw();
	eyeEmitter.draw();

	if (gameOver) {
		ofDrawBitmapString("YOU DIED. GAME OVER.", 512, 384);
	}


	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch (key) {
	case ' ': {
		player.image = horseJump;
		player.ntiles_x = 4;
		player.ntiles_y = 4;
		player.nframes = 16;
		player.width = 80;
		player.height = 82;
		player.pos.x = 100;
		player.pos.y = 687;
		player.frame = 0;
	}

	

	
	}
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
	bow.frame = 0;
	bow.nframes = 11;
	bow.startAnim();
	if(!gameOver) drawBow.play();
	


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	

	ofVec3f aimVec = ofVec3f(mouseX, mouseY, 0) - arrowEmitter.pos;
	arrowEmitter.velocity = aimVec;
	if (bow.frame == 10 && !gameOver) {
		arrowEmitter.emit();
		fireBow.play();
	}

	bow.frame = 11;
	bow.nframes = 24;

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
