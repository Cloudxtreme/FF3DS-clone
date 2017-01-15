#include "player.h"


const int maxHp = 500;
const int maxMp = 80;
char name[] = "Dary";		//debug code
int hp = 500;
int mp = 80;


void PlayerInit() {

}

//-------------------------------------------------------------------------------
void Stats() {
//-------------------------------------------------------------------------------

	puts("\x1B[2J");	//clears screen
	printf("\x1b[37mPlayer name: %s\n\x1b", name);
	printf("\x1b[32mHP: %i\x1b\t\t \x1b[33mMP: %i\x1b\n", hp, mp);
			//display healthbar		


}
//-------------------------------------------------------------------------------
void PlayerDmg(int dmg) {
//-------------------------------------------------------------------------------
	hp -= dmg;
}

