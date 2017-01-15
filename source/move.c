#include "move.h"

#define TILE 4096
#define SMALLTILE 512

//-------------------------------------------------------------------------------
Sprite Move(Sprite sprite) {
//-------------------------------------------------------------------------------
	hidScanInput();
	u32 kHeld = hidKeysHeld();
	u32 kDown = hidKeysDown();
	//Controls the movement of current sprite (don't use for terrain unless trying to simulate an earthquake!)
	

	//if the button is pressed just once
	if(kDown & KEY_RIGHT) 
		sprite = MoveRight(sprite);
	if(kDown & KEY_LEFT) 
		sprite = MoveLeft(sprite);
	if(kDown & KEY_UP)
		sprite = MoveUp(sprite);
	if(kDown & KEY_DOWN)
		sprite = MoveDown(sprite);




	
	return sprite;
}

//-------------------------------------
Sprite MoveUp(Sprite trans) {
//-------------------------------------
	trans.y -= TILE;	//Moves player in direction specified by 32 (aka 1 tile)
	return trans;
}

//-------------------------------------
Sprite MoveRight(Sprite trans) {
//-------------------------------------
	trans.x += TILE;	//Moves player in direction specified by 32 (aka 1 tile)
	return trans;
}

//-------------------------------------
Sprite MoveDown(Sprite trans) {
//-------------------------------------
	trans.y += TILE;	//Moves player in direction specified by 32 (aka 1 tile)
	return trans;
}

//-------------------------------------
Sprite MoveLeft(Sprite trans) {
//-------------------------------------
	trans.x -= TILE;	//Moves player in direction specified by 32 (aka 1 tile)
	return trans;
}

Sprite SlowMoveUp(Sprite trans) {
	for(int i = 0; i < 8; i++)
		trans.y -= SMALLTILE;
	return trans;
}
