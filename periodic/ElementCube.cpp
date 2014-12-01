#include "coders_crux.gen.h" //Contains FB32 palette
#include "ElementCube.h"
#include "Element.h"
#include "periodic.h"

void ElementCube::Init(int cubeId, int initialElementNum)
{
    this->cubeId = cubeId;
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

ElementCube::ElementCube(int cubeId, int initialElementNum)
{
    Init(cubeId, initialElementNum);
}

ElementCube::ElementCube(int cubeId, const char* initialElementSymbol)
{
    Init(cubeId, Element::GetRawElementNum(initialElementSymbol));
}

void ElementCube::GoToNextElement()
{
    currentElementNum++;

    if (currentElementNum >= Element::GetRawElementCount())
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
        LOG("They react!\n");
        this->isDirty = true;
        other->isDirty = true;
    }
}

void ElementCube::ReactWith(ElementCube* other1, ElementCube* other2)
{
    LOG("Attempting to react %s, %s, and %s.\n", this->currentElement.GetName(), other1->currentElement.GetName(), other2->currentElement.GetName());

    if (this->currentElement.ReactWith(&other1->currentElement, &other2->currentElement))
    {
        LOG("They react!\n");
        this->isDirty = true;
        other1->isDirty = true;
        other2->isDirty = true;
    }
}

int ElementCube::GetCubeId()
{
    return cubeId;
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

    // Add space for +/- symbol
    //TODO: Need to handle other charges. (Like 2+) (Needs smaller font for numbers.)
    if (currentElement.GetCharge() != 0)
    {
        stringWidth += 3 + LETTER_SPACING;
    }

    int x = SCREEN_WIDTH / 2 - stringWidth / 2;
    int y = SCREEN_HEIGHT / 2 - stringHeight / 2;

    // Draw the symbol:
    for (int i = 0; i < chars; i++)
    {
        //LOG("Drawing '%c'\n", symbol[i]);
        DrawCharAt(x, y, symbol[i]);
        x += CODERS_CRUX_GLYPH_WIDTH + LETTER_SPACING;
    }

    // Draw the +/- symbol:
    if (currentElement.GetCharge() != 0)
    {
        // Draw the line for the dash
        v.fb32.plot(vec(x + 0, y + 1), CHARGE_COLOR);
        v.fb32.plot(vec(x + 1, y + 1), CHARGE_COLOR);
        v.fb32.plot(vec(x + 2, y + 1), CHARGE_COLOR);

        // Draw the nubs for the plus for positive charge
        if (currentElement.GetCharge() > 0)
        {
            v.fb32.plot(vec(x + 1, y + 0), CHARGE_COLOR);
            v.fb32.plot(vec(x + 1, y + 2), CHARGE_COLOR);
        }
    }

    // Draw covalent bond (basic):
    if (currentElement.GetBondType() == COVALENT)
    {
        for (int i = 0; i < currentElement.GetSharedElectrons() / 2; i++)
        {
            v.fb32.plot(vec(SCREEN_WIDTH - 2, 1 + i * 2), COVALENT_COLOR);
        }
    }

    // Draw potential reaction:
    if (currentElement.GetBondType() == POTENTIAL)
    {
        // Draw border on the cube
        //NOTE: Relies on screen being square.
        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
            v.fb32.plot(vec(i, 0), POTENTIAL_COLOR);
            v.fb32.plot(vec(i, SCREEN_HEIGHT - 1), POTENTIAL_COLOR);
            v.fb32.plot(vec(0, i), POTENTIAL_COLOR);
            v.fb32.plot(vec(SCREEN_WIDTH - 1, i), POTENTIAL_COLOR);
        }
    }

    // Draw electrons:
    DrawLewisDots(stringWidth, stringHeight);

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
        Assert(false);
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

/*
Order of adding electrons according to WolframAlpha:
1. Right
2. Left
3. Top
4. Bottom
Repeat pattern as needed.
*/
enum LewisSides
{
    LRight = 0,
    LLeft = 1,
    LTop = 2,
    LBottom = 3,

    LFirst = LRight,
    LLast = LBottom
};

void ElementCube::DrawLewisDots(int stringWidth, int stringHeight)
{
    for (int s = LFirst; s <= LLast; s++)
    {
        // Calculate the number of electrons on this side
        int e = (currentElement.GetNumOuterElectrons() + 3 - s) / 4;

        int x = SCREEN_WIDTH / 2;
        int y = SCREEN_HEIGHT / 2;

        // Calulate offset to get on the correct side:
        switch (s)
        {
            case LRight:
                x += stringWidth / 2 + 2;
                y += LETTER_DESCENDER_HEIGHT / 2;
                break;
            case LLeft:
                x -= stringWidth / 2 + 2;
                y += LETTER_DESCENDER_HEIGHT / 2;
                break;
            case LTop:
                y -= stringHeight / 2 + 2;
                break;
            case LBottom:
                y += stringHeight / 2 + 2;
                break;
        }
        
        // Calculate offset for electron separation:
        switch (s)
        {
            case LRight:
            case LLeft:
                y -= (e - 1) * 2;
                break;
            case LTop:
            case LBottom:
                x -= (e - 1) * 2;
                break;
        }

        // Draw the electrons:
        for ( ; e > 0; e--)
        {
            v.fb32.plot(vec(x, y), ELECTRON_COLOR);

            switch (s)
            {
            case LRight:
            case LLeft:
                y += 2;
                break;
            case LTop:
            case LBottom:
                x += 2;
                break;
            }
        }
    }
}
