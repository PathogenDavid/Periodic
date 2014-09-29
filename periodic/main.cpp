/*
 * Sifteo SDK Example.
 */

#include <sifteo.h>
#include "assets.gen.h"
using namespace Sifteo;

static AssetSlot MainSlot = AssetSlot::allocate()
    .bootstrap(GameAssets);

static Metadata M = Metadata()
    .title("Periodic Project")
    .package("edu.ksu.periodic", "0.1")
    .icon(Icon)
    .cubeRange(2)
;

const CubeID displayCube(0);
static VideoBuffer video;
const CubeID infoCube(1);
static VideoBuffer infoVideo;

static bool isRelease[CUBE_ALLOCATION];//TODO: Investigate if this should be initialized with CubeID.isTouch on startup.

typedef enum
{
    EDT_WINDOWED_SPRITES,
    EDT_FB_COLOR,
    EDT_FB_BW,
    EDT_FULL_CLEAR,
    EDT_COUNT
} E_DISPLAY_TESTS;

int currentTest;
bool forceSuperClearOnNextLoop;

void OnPress(unsigned cubeId);
void OnRelease(unsigned cubeId);
void OnTouch(void* sender, unsigned cubeId);

void InitCurrentTest();
void DrawCurrentTest(float delta);

void ClearCursor();
void DrawCursor();

void main()
{
    video.attach(displayCube);
    infoVideo.attach(infoCube);

    Events::cubeTouch.set(OnTouch);
    
    //Draw menu on info cube:
    infoVideo.initMode(BG0_ROM);
    infoVideo.bg0rom.text(vec(1, 0), "Windowed Sprites");
    infoVideo.bg0rom.text(vec(1, 1), "FB Mode 32");
    infoVideo.bg0rom.text(vec(1, 2), "FB Mode 64");
    infoVideo.bg0rom.text(vec(1, 3), "Screen flicker");
    DrawCursor();
    InitCurrentTest();
    
    TimeStep timeStep;
    while (1)
    {
        if (forceSuperClearOnNextLoop)
        {
            System::finish();
            video.setDefaultWindow();
            System::paintUnlimited();
            video.setDefaultWindow();
            System::finish();

            forceSuperClearOnNextLoop = false;
        }

        DrawCurrentTest(timeStep.delta());
        timeStep.next();
    }
}

void OnPress(unsigned cubeId)
{
}

void OnRelease(unsigned cubeId)
{
    ClearCursor();
    currentTest++;
    if (currentTest >= EDT_COUNT)
    { currentTest = 0; }
    DrawCursor();
    InitCurrentTest();
    forceSuperClearOnNextLoop = true;
}

void OnTouch(void* sender, unsigned cubeId)
{
    if (isRelease[cubeId])
    {
        OnRelease(cubeId);
    }
    else
    {
        OnPress(cubeId);
    }

    isRelease[cubeId] = !isRelease[cubeId];//Next touch event on this cube will be a release event
}

void ClearCursor()
{
    infoVideo.bg0rom.text(vec(0, currentTest), " ");
}

void DrawCursor()
{
    infoVideo.bg0rom.text(vec(0, currentTest), "*");
}

//Common variables/constants:
float time;
#define NUM_SPRITES 16
float SpriteX[NUM_SPRITES];

//EDT_WINDOWED_SPRITES variables/constants:
#define NUM_ACTUAL_SPRITES 8
const UInt2 SpriteSize = vec(8, 8);

//EDT_FB_COLOR variables/constants:
const UInt2 SpriteSizeFB32 = vec(2,2);

//EDT_FB_BW variables/constants:
const UInt2 SpriteSizeFB64 = vec(4,4);

//EDT_FULL_CLEAR
bool everyOtherFrame;

void InitCurrentTest()
{
    if (currentTest == EDT_WINDOWED_SPRITES)
    {
        video.initMode(BG0_SPR_BG1);
        video.bg0.fill(vec(0, 0), vec(16, 16), BlackBg);
        video.bg1.eraseMask();

        return;
    }

    if (currentTest == EDT_FB_COLOR)
    {
        video.initMode(FB32);
        video.colormap.setEGA();
        video.fb32.fill(0);

        return;//Remove me for fun glitches
    }

    if (currentTest == EDT_FB_BW)
    {
        video.initMode(FB64);
        video.colormap.setMono(0x0, 0xFFFFFFFF);
        video.fb64.fill(0);

        return;//Remove me for fun glitches
    }

    if (currentTest == EDT_FULL_CLEAR)
    {
        video.initMode(SOLID_MODE);
        return;
    }

    video.initMode(BG0_ROM);
    video.bg0rom.text(vec(0, 0), "//TODO: IMPLEMENT ME");
    video.setDefaultWindow();
}

void UpdateSprite(int i)
{
    SpriteX[i] = sin((float)i + time * 2.f) * 56.f + 60.f;
}

void UpdateSprites()
{
    //Update the "sprites":
    for (int i = 0; i < NUM_SPRITES; i++)
    {
        UpdateSprite(i);
    }
}

void DrawWindowedSprites(float delta)
{
    UpdateSprites();

    //Draw the sprites, one bank at a time:
    for (int j = 0; j < NUM_SPRITES; j += NUM_ACTUAL_SPRITES)
    {
        //Set the Window:
        video.setWindow(j * 8, (j + NUM_ACTUAL_SPRITES) * 8);

        //Set the image and position for the current sprites:
        for (int i = 0; i < NUM_ACTUAL_SPRITES; i++)
        {
            video.sprites[i].setImage(Rainbow, j + i);
            video.sprites[i].move(SpriteX[j + i], (j + i) * 8);
        }

        System::paint();
        System::finish();//Fixes tearing on sprites
    }
}

void DrawCurrentTest(float delta)
{
    time += delta;

    if (currentTest == EDT_WINDOWED_SPRITES)
    {
        DrawWindowedSprites(delta);
        return;
    }

    if (currentTest == EDT_FB_COLOR)
    {
        for (int i = 0; i < NUM_SPRITES; i++)
        {
            //Clear the old one:
            UInt2 pos = vec((unsigned)(SpriteX[i] / 4.f), i * SpriteSizeFB32.y);
            video.fb32.fill(pos, SpriteSizeFB32, 0);

            //Draw the new one:
            UpdateSprite(i);
            pos.x = SpriteX[i] / 4.f;
            video.fb32.fill(pos, SpriteSizeFB32, i % 15 + 1);//Keep color index between 1 - 15 (#0 is the background)
        }

        System::paint();
        System::finish();
        return;
    }

    if (currentTest == EDT_FB_BW)
    {
        for (int i = 0; i < NUM_SPRITES; i++)
        {
            //Clear the old one:
            UInt2 pos = vec((unsigned)(SpriteX[i] / 2.f), i * SpriteSizeFB64.y);
            video.fb64.fill(pos, SpriteSizeFB64, 0);

            //Draw the new one:
            UpdateSprite(i);
            pos.x = SpriteX[i] / 2.f;
            video.fb64.fill(pos, SpriteSizeFB64, 1);
        }

        System::paint();
        System::finish();
        return;
    }

    if (currentTest == EDT_FULL_CLEAR)
    {
        video.colormap[0] = RGB565::fromRGB(everyOtherFrame ? 0xFF0000 : 0x0000FF);
        everyOtherFrame = !everyOtherFrame;
        System::paintUnlimited();
        return;
    }

    System::paint();
}
