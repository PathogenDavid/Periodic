#include "coders_crux.gen.h" //Contains FB32 palette
#include "ElementCube.h"
#include "Element.h"
#include "periodic.h"

ElementCube::ElementCube(int cubeId, int initialElementNum)
{
    this->cube = CubeID(cubeId);

    currentElementNum = initialElementNum;
    Element::GetRawElement(currentElementNum, &currentElement);
    isDirty = true;

    // Initialize video:
    v.attach(cube);
    v.initMode(FB32);
    v.fb32.fill(BG_COLOR);
    
    // Load the palette:
    for (int j = 0, h = 0; j < PALETTE_COUNT; j++, h += 3)
    {
        v.colormap[j].set(palette[h], palette[h + 1], palette[h + 2]);
    }
}

void ElementCube::GoToNextElement()
{
    currentElementNum++;

    if (currentElementNum > Element::GetRawElementCount())
    { currentElementNum = 0; }

    Element::GetRawElement(currentElementNum, &currentElement);
    isDirty = true;
}

void ElementCube::ResetElement()
{
    currentElement.ResetToBasicState();
    isDirty = true;//TODO: Right now the cubes get marked as dirty when they shouldn't.
}

void ElementCube::ReactWith(ElementCube* other)
{
    LOG("Attempting to react %s with %s.\n", this->currentElement.GetName(), other->currentElement.GetName());

    if (this->currentElement.ReactWith(&other->currentElement))
    {
        LOG("They react!");
        this->isDirty = true;
        other->isDirty = true;
    }
}

void ElementCube::Render()
{
    if (!isDirty)
    {
        return;
    }

    //LOG("Cube %d is dirty! Redrawing.\n", (int)cube);

    // Clear the screen:
    v.fb32.fill(BG_COLOR);

    // Draw the element symbol:
    const char* symbol = currentElement.GetSymbol();
    int chars = strlen(symbol);
    int stringWidth = chars * CODERS_CRUX_GLYPH_WIDTH + (chars - 1) * LETTER_SPACING;
    int stringHeight = CODERS_CRUX_GLYPH_HEIGHT - LETTER_DESCENDER_HEIGHT;

    int x = SCREEN_WIDTH / 2 - stringWidth / 2;
    int y = SCREEN_HEIGHT / 2 - stringHeight / 2;

    // Draw the symbol:
    for (int i = 0; i < chars; i++)
    {
        //LOG("Drawing '%c'\n", symbol[i]);
        DrawCharAt(x, y, symbol[i]);
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
    v.fb32.plot(vec(x, y - yoff), electronColor);
    v.fb32.plot(vec(x, y + yoff), electronColor);
    x += stringWidth + xoff;
    x++;
    v.fb32.plot(vec(x, y - yoff), electronColor);
    v.fb32.plot(vec(x, y + yoff), electronColor);
    x = SCREEN_WIDTH / 2;
    y -= stringHeight / 2 + xoff;
    int zoff = 1 * chars;
    v.fb32.plot(vec(x - zoff, y), electronColor);
    v.fb32.plot(vec(x + zoff, y), electronColor);
    y += stringHeight + xoff;
    y++;
    v.fb32.plot(vec(x - zoff, y), electronColor);
    v.fb32.plot(vec(x + zoff, y), electronColor);

    isDirty = false;
}

void ElementCube::DrawCharAt(int x, int y, char c)
{
    // Convert char to glyph id:
    if (c >= 'a' && c <= 'z')
    {
        c = c - 'a' + 26;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        c -= 'A';
    }
    else
    {
        c = 0;
    }//Invalid character

    //This function seems broken, or I am misunderstanding its purpose.
    //v.fb32.bitmap(vec(x, y), vec(CODERS_CRUX_GLYPH_WIDTH, CODERS_CRUX_GLYPH_HEIGHT), &coders_crux[c * CODERS_CRUX_GLYPH_SIZE], CODERS_CRUX_GLYPH_WIDTH);

    for (int i = 0; i < CODERS_CRUX_GLYPH_HEIGHT; i++)
    {
        for (int j = 0; j < CODERS_CRUX_GLYPH_WIDTH; j++)
        {
            v.fb32.plot(vec(x + j, y + i), coders_crux[j + i * CODERS_CRUX_GLYPH_WIDTH + c * CODERS_CRUX_GLYPH_SIZE]);
        }
    }
}
