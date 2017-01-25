#include "menu.h"

void mainMenu() {
	int pos = 0;
	const int totalElem = 2;
	static const char elements[ELEMENTS][32] = {"New Game", "Load Game"};
	
	char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};

	for(int i = 0; i < ELEMENTS; i++) printf("%s\n", elements[i]);

	bool loop = true;

	u32 kDownOld = 0;

	while(loop) {
		hidScanInput();

		u32 kDown = hidKeysDown();

		if(kDown != kDownOld) {
			consoleClear();
			for(int i = 0; i < ELEMENTS; i++) {
				printf("%s\n", elements[i]);
			}
		}
		kDown = kDownOld;
	}


}