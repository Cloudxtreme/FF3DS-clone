#include "audio.h"
#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"

u8* buffer;
u32 size;

void audio_load(const char *audio){

	FILE *file = fopen(audio, "rb");
	fseek(file, 0, SEEK_END);
	off_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = linearAlloc(size);
	off_t bytesRead = fread(buffer, 1, size, file);
	fclose(file);
	csndPlaySound(8, SOUND_FORMAT_16BIT | SOUND_REPEAT, 96000, 1, 0, buffer, buffer, size);
	linearFree(buffer);
}
void audio_stop(void){
	csndExecCmds(true);
	CSND_SetPlayState(0x8, 0);
	memset(buffer, 0, size);
	GSPGPU_FlushDataCache(buffer, size);
	linearFree(buffer);
}

void audioLoad() {
	drflac* pFlac = drflac_open_file("/music/Cosmic Thing.flac");
    	
    int32_t* pSampleData = (int32_t*)malloc((size_t)pFlac->totalSampleCount * sizeof(int32_t));
   	drflac_read_s32(pFlac, pFlac->totalSampleCount, pSampleData);

    // At this point pSampleData contains every decoded sample as signed 32-bit PCM.

    drflac_close(pFlac);
}

void audioPlay(int32_t* pSampleData) {
  	srvInit();
	aptInit();
	hidInit();
	bool stop = 0;
	u32 kDown;
	csndInit();//start Audio Lib
	hidScanInput();
	kDown = hidKeysDown();
	audio_load("audio/Start Sound.raw");


	//audio_stop();
	csndExit();

	hidExit();
	aptExit();
	srvExit();
}