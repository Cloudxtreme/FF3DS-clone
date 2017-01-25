/*

	Simple demo of sprites using citro3d, lodepng and immediate mode.

	Citra doesn't yet emulate immediate mode so this is hw only.

*/

#include <3ds.h>
#include <citro3d.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "lodepng.h"
#include "player.h"
#include "move.h"
#include "runtime.h"
#include "menu.h"

// These headers are generated by the build process
#include "vshader_shbin.h"
#include "characters_png.h"

#define CLEAR_COLOR 0xFFFFFFFF

// Used to transfer the final rendered display to the framebuffer
#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

// Used to convert textures to 3DS tiled format
// Note: vertical flip flag set so 0,0 is top left of texture
#define TEXTURE_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define NUM_SPRITES 1
#define MAX_HEIGHT 240
#define MIN_HEIGHT 1
#define MAX_WIDTH 400
#define MIN_WIDTH 1

//simple sprite struct


//Sprite sprites[NUM_SPRITES];
Sprite player;

struct { float left, right, top, bottom; } images[4] = {
	{0.0f, 0.5f, 0.0f, 0.5f},
	{0.5f, 1.0f, 0.0f, 0.5f},
	{0.0f, 0.5f, 0.5f, 1.0f},
	{0.5f, 1.0f, 0.5f, 1.0f},
};

//---------------------------------------------------------------------------------
void drawSprite( int x, int y, int width, int height, int image ) {
//---------------------------------------------------------------------------------

	float left = images[image].left;
	float right = images[image].right;
	float top = images[image].top;
	float bottom = images[image].bottom;

	// Draw a textured quad directly
	C3D_ImmDrawBegin(GPU_TRIANGLES);
		C3D_ImmSendAttrib(x, y, 0.5f, 0.0f); // v0=position
		C3D_ImmSendAttrib( left, top, 0.0f, 0.0f);

		C3D_ImmSendAttrib(x+width, y+height, 0.5f, 0.0f);
		C3D_ImmSendAttrib( right, bottom, 0.0f, 0.0f);

		C3D_ImmSendAttrib(x+width, y, 0.5f, 0.0f);
		C3D_ImmSendAttrib( right, top, 0.0f, 0.0f);

		C3D_ImmSendAttrib(x, y, 0.5f, 0.0f); // v0=position
		C3D_ImmSendAttrib( left, top, 0.0f, 0.0f);

		C3D_ImmSendAttrib(x, y+height, 0.5f, 0.0f);
		C3D_ImmSendAttrib( left, bottom, 0.0f, 0.0f);

		C3D_ImmSendAttrib(x+width, y+height, 0.5f, 0.0f);
		C3D_ImmSendAttrib( right, bottom, 0.0f, 0.0f);


	C3D_ImmDrawEnd();

}


static DVLB_s* vshader_dvlb;
static shaderProgram_s program;
static int uLoc_projection;
static C3D_Mtx projection;

static C3D_Tex spritesheet_tex;

//---------------------------------------------------------------------------------
static void sceneInit(void) {
//---------------------------------------------------------------------------------
	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v2=texcoord

	// Compute the projection matrix
	// Note: we're setting top to 240 here so origin is at top left.
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 240.0, 0.0, 0.0, 1.0, true);

	//spawn point
	player.x = (200 << 8);
	player.y = (120 << 8);
	player.dx = 512;		//Don't change these to anything that isn't divisable by TILE in move.h
	player.dy = 512;
	player.image = 0;

	// Configure buffers
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);

	unsigned char* image;
	unsigned width, height;

	lodepng_decode32(&image, &width, &height, characters_png, characters_png_size);

	u8 *gpusrc = linearAlloc(width*height*4);

	// GX_DisplayTransfer needs input buffer in linear RAM
	u8* src=image; u8 *dst=gpusrc;

	// lodepng outputs big endian rgba so we need to convert
	for(int i = 0; i<width*height; i++) {
		int r = *src++;
		int g = *src++;
		int b = *src++;
		int a = *src++;

		*dst++ = a;
		*dst++ = b;
		*dst++ = g;
		*dst++ = r;
	}

	// ensure data is in physical ram
	GSPGPU_FlushDataCache(gpusrc, width*height*4);

	// Load the texture and bind it to the first texture unit
	C3D_TexInit(&spritesheet_tex, width, height, GPU_RGBA8);

	// Convert image to 3DS tiled texture format
	C3D_SafeDisplayTransfer ((u32*)gpusrc, GX_BUFFER_DIM(width,height), (u32*)spritesheet_tex.data, GX_BUFFER_DIM(width,height), TEXTURE_TRANSFER_FLAGS);
	gspWaitForPPF();

	C3D_TexSetFilter(&spritesheet_tex, GPU_LINEAR, GPU_NEAREST);
	C3D_TexBind(0, &spritesheet_tex);

	free(image);
	linearFree(gpusrc);

	// Configure the first fragment shading substage to just pass through the texture color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, 0, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

	srand(time(NULL));


	// Configure depth test to overwrite pixels with the same depth (needed to draw overlapping sprites)
	C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);
}
//--------------------------------------------------------------------------------
static void boundaries(int c) {
//--------------------------------------------------------------------------------
	if(player.x < (MIN_WIDTH << 8))  {
		player.x = (MAX_WIDTH << 8);
	}
	if(player.x > (MAX_WIDTH << 8)) {
		player.x = (MIN_WIDTH << 8);
	}
		
	if(player.y > (MAX_HEIGHT << 8)) {
		player.y = (MIN_HEIGHT << 8);
	}
	if(player.y < (MIN_HEIGHT << 8)) {
		player.y = (MAX_HEIGHT << 8);
	}

}

//---------------------------------------------------------------------------------
static void sceneRender(void) {
//---------------------------------------------------------------------------------
	int i;
	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);

	for(i = 0; i < NUM_SPRITES; i++) {

		drawSprite( player.x >> 8, player.y >> 8, 32, 32, player.image);
	}

}

//---------------------------------------------------------------------------------
static void sceneExit(void) {
//---------------------------------------------------------------------------------

	// Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------
	//The time the game started
	StTime();

	// Initialize graphics
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	// Initialize the render target
	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
	consoleInit(GFX_BOTTOM, NULL);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// Initialize the scene
	sceneInit();
	bool loop = true;

	//Initialise variables
	PlayerInit();
	u32 kPress;

	// Main loop
	while (loop) {
		
		mainMenu();
		kPress = hidKeysDown();
		Stats();
		if(kPress & KEY_START)
			loop = false;

		player = Move(player);
		boundaries(0);
		UpTime();
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_FrameDrawOn(target);
			sceneRender();
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	sceneExit();

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}
