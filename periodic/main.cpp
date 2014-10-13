/*
 * Sifteo SDK Example.
 */

#include <sifteo.h>
#include "assets.gen.h"
#include "coders_crux.gen.h"

using namespace Sifteo;

static AssetSlot MainSlot = AssetSlot::allocate()
    .bootstrap(GameAssets);

#define NUM_CUBES 2

static Metadata M = Metadata()
    .title("Periodic Project")
    .package("edu.ksu.periodic", "0.1")
    .icon(Icon)
    .cubeRange(NUM_CUBES)
;

const char* elements1[] = { "H", "Li", "Na", "K", "Rb", "Cs", "Fr", NULL };
const char* elements2[] = { "F", "Cl", "Br", "I", "At", "Uus", NULL };

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

static struct
{
    const CubeID cube;
    VideoBuffer video;
    bool isDirty;
    int currentElement;
    const char** elements;
} cubes[NUM_CUBES] =
{
    { CubeID(0), VideoBuffer(), true, 0, NULL },
    { CubeID(1), VideoBuffer(), true, 0, NULL },
};

static bool isRelease[CUBE_ALLOCATION];//TODO: Investigate if this should be initialized with CubeID.isTouch on startup.

void OnPress(unsigned cubeId);
void OnRelease(unsigned cubeId);
void OnTouch(void* sender, unsigned cubeId);

void RenderCube(unsigned cubeId);

#define BG_COLOR 7 //White

void main()
{
    cubes[0].elements = elements1;
    cubes[1].elements = elements2;

    // Initialize video buffers:
    for (unsigned i = 0; i < NUM_CUBES; i++)
    {
        cubes[i].video.attach(cubes[i].cube);
        cubes[i].video.initMode(FB32);
        cubes[i].video.fb32.fill(BG_COLOR);

        // Load the palette:
        for (int j = 0, h = 0; j < PALETTE_COUNT; j++, h += 3)
        {
            cubes[i].video.colormap[j].set(palette[h], palette[h + 1], palette[h + 2]);
        }
    }

    Events::cubeTouch.set(OnTouch);
    
    TimeStep timeStep;
    while (1)
    {
        for (unsigned i = 0; i < NUM_CUBES; i++)
        {
            RenderCube(i);
        }

        System::paint();
        timeStep.next();
    }
}

void OnPress(unsigned cubeId)
{
}

void OnRelease(unsigned cubeId)
{
    LOG("Going to next elemenet on cube %d.\n", cubeId);

    cubes[cubeId].currentElement++;

    if (cubes[cubeId].elements[cubes[cubeId].currentElement] == NULL)
    {
        cubes[cubeId].currentElement = 0;
    }

    cubes[cubeId].isDirty = true;
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

int strlen(const char* str);

#define LETTER_SPACING 1
#define LETTER_DESCENDER_HEIGHT 2

void DrawCharAt(VideoBuffer* v, int x, int y, char c);

void RenderCube(unsigned cubeId)
{
    if (!cubes[cubeId].isDirty)
    { return; }

    LOG("Cube %d is dirty! Redrawing.\n", cubeId);
    VideoBuffer* v = &cubes[cubeId].video;

    // Clear the screen:
    v->fb32.fill(BG_COLOR);

    // Draw the element symbol:
    const char* symbol = cubes[cubeId].elements[cubes[cubeId].currentElement];
    int chars = strlen(symbol);
    int stringWidth = chars * CODERS_CRUX_GLYPH_WIDTH + (chars - 1) * LETTER_SPACING;
    int stringHeight = CODERS_CRUX_GLYPH_HEIGHT - LETTER_DESCENDER_HEIGHT;

    int x = SCREEN_WIDTH / 2 - stringWidth / 2;
    int y = SCREEN_HEIGHT / 2 - stringHeight / 2;

    // Draw the symbol:
    for (int i = 0; i < chars; i++)
    {
        LOG("Drawing '%c'\n", symbol[i]);
        DrawCharAt(v, x, y, symbol[i]);
        x += CODERS_CRUX_GLYPH_WIDTH + LETTER_SPACING;
    }

    // Draw some dummy electrons:
    // Right now this is super hacky, need to clean up.
    const int electronColor = 15;

    x = SCREEN_WIDTH / 2 - stringWidth / 2;
    y = SCREEN_HEIGHT / 2;
    int xoff = 2;
    int yoff = 2;
    x -= xoff;
    v->fb32.plot(vec(x, y - yoff), electronColor);
    v->fb32.plot(vec(x, y + yoff), electronColor);
    x += stringWidth + xoff;
    x++;
    v->fb32.plot(vec(x, y - yoff), electronColor);
    v->fb32.plot(vec(x, y + yoff), electronColor);
    x = SCREEN_WIDTH / 2;
    y -= stringHeight / 2 + xoff;
    int zoff = 1 * chars;
    v->fb32.plot(vec(x - zoff, y), electronColor);
    v->fb32.plot(vec(x + zoff, y), electronColor);
    y += stringHeight + xoff;
    y++;
    v->fb32.plot(vec(x - zoff, y), electronColor);
    v->fb32.plot(vec(x + zoff, y), electronColor);

    cubes[cubeId].isDirty = false;
}

void DrawCharAt(VideoBuffer* v, int x, int y, char c)
{
    // Convert char to glyph id:
    if (c >= 'a' && c <= 'z')
    { c = c - 'a' + 26; }
    else if (c >= 'A' && c <= 'Z')
    { c -= 'A'; }
    else
    { c = 0; }//Invalid character
    
    //This function seems broken, or I am misunderstanding its purpose.
    //v->fb32.bitmap(vec(x, y), vec(CODERS_CRUX_GLYPH_WIDTH, CODERS_CRUX_GLYPH_HEIGHT), &coders_crux[c * CODERS_CRUX_GLYPH_SIZE], CODERS_CRUX_GLYPH_WIDTH);

    for (int i = 0; i < CODERS_CRUX_GLYPH_HEIGHT; i++)
    {
        for (int j = 0; j < CODERS_CRUX_GLYPH_WIDTH; j++)
        {
            v->fb32.plot(vec(x + j, y + i), coders_crux[j + i * CODERS_CRUX_GLYPH_WIDTH + c * CODERS_CRUX_GLYPH_SIZE]);
        }
    }
}

int strlen(const char* str)
{
    int ret = 0;
    while (str[ret] != '\0')
    { ret++; }
    return ret;
}
