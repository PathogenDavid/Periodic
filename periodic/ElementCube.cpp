#include "coders_crux.gen.h" // Contains FB32 palette
#include "number_font.h"
#include "ElementCube.h"
#include "Element.h"
#include "periodic.h"

// Uncomment to draw info for debugging rotation logic:
//#define DEBUG_ROTATION_LOGIC

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

void ElementCube::Initialize(int cubeId, int initialElementNum)
{
    this->cubeId = cubeId;
    this->cube = CubeID(cubeId);

    currentElementNum = initialElementNum;
    Element::GetRawElement(currentElementNum, &currentElement);
    rotation = CubeRotatation0;
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

void ElementCube::Initialize(int cubeId,const char* initialElementSymbol)
{
    Initialize(cubeId, Element::GetRawElementNum(initialElementSymbol));
}

ElementCube::ElementCube()
{
}

ElementCube::ElementCube(int cubeId, int initialElementNum)
{
    Initialize(cubeId, initialElementNum);
}

ElementCube::ElementCube(int cubeId, const char* initialElementSymbol)
{
    Initialize(cubeId, Element::GetRawElementNum(initialElementSymbol));
}

void ElementCube::GoToNextElement()
{
    currentElementNum++;

    if (currentElementNum >= Element::GetRawElementCount())
    { currentElementNum = 0; }

    Element::GetRawElement(currentElementNum, &currentElement);
    isDirty = true;
}

void ElementCube::Reset()
{
    currentElement.ResetToBasicState();
    rotation = CubeRotatation0;
    isDirty = true;//TODO: Right now the cubes get marked as dirty when they shouldn't.
}

int ElementCube::GetCubeId()
{
    return cubeId;
}

Element* ElementCube::GetElement()
{
    return &currentElement;
}

void ElementCube::SetDirty()
{
    isDirty = true;
}

void ElementCube::RotateByClockwise(CubeRotation rotation)
{
    this->rotation += rotation;
    this->rotation %= CubeRotatationCount;
    SetDirty();
}

void ElementCube::RotateByCounterClockwise(CubeRotation rotation)
{
    this->rotation -= rotation;
    this->rotation %= CubeRotatationCount;
    SetDirty();
}

void ElementCube::RotateTo(ElementCube* otherCube)
{
    this->rotation = otherCube->rotation;
    SetDirty();
}

void ElementCube::RotateTo(CubeRotation rotation)
{
    this->rotation = rotation;
    SetDirty();
}

CubeRotation ElementCube::GetRotation()
{
    return (CubeRotation)rotation;
}

void ElementCube::DrawDot(int x, int y, unsigned int color)
{
    if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
    {
        LOG("WARN: ElementCube:%d tried to draw %d at %d, %d!\n", cubeId, color, x, y);
        return;
    }

    switch (rotation)
    {
    case CubeRotatation0:
        v.fb32.plot(vec(x, y), color);  //no rotation
        break;
    case CubeRotatation90:
        v.fb32.plot(vec(SCREEN_WIDTH - y - 1, x), color);  //clockwise 90
        break;
    case CubeRotatation180:
        v.fb32.plot(vec(SCREEN_WIDTH - x - 1, SCREEN_HEIGHT - y - 1), color);  // clockwise 180
        break;
    case CubeRotatation270:
        v.fb32.plot(vec(y, SCREEN_HEIGHT - x - 1), color);   //clockwise 270
        break;
    default:
        AssertAlways(); // This should never happen
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

    // Add space for +/- symbol
    if (currentElement.GetCharge() != 0)
    {
        stringWidth += 3 + LETTER_SPACING;

        // Add space for number
        if (abs(currentElement.GetCharge()) > 1)
        {
            stringWidth += NUMBER_FONT_GLYPH_WIDTH + LETTER_SPACING;
        }
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
    if (currentElement.GetCharge() != 0 && currentElement.HasBondType(BondType_Ionic))
    {   
        if (abs(currentElement.GetCharge()) > 1)
        {
            // Draw the number
            DrawNumAt(x, y - 1, currentElement.GetCharge(), CHARGE_COLOR);
            // Draw the line for the dash
            DrawDot(x + 3 + LETTER_SPACING + 0, y + 1, CHARGE_COLOR);
            DrawDot(x + 3 + LETTER_SPACING + 1, y + 1, CHARGE_COLOR);
            DrawDot(x + 3 + LETTER_SPACING + 2, y + 1, CHARGE_COLOR);
            // Draw the nubs for the plus for positive charge
            if (currentElement.GetCharge() > 0)
            {
                DrawDot(x + 3 + LETTER_SPACING + 1, y + 0, CHARGE_COLOR);
                DrawDot(x + 3 + LETTER_SPACING + 1, y + 2, CHARGE_COLOR);
            }
        }
        else{ //charge == +1 or -1
            // Draw the line for the dash
            DrawDot(x + 0, y + 1, CHARGE_COLOR);
            DrawDot(x + 1, y + 1, CHARGE_COLOR);
            DrawDot(x + 2, y + 1, CHARGE_COLOR);
            // Draw the nubs for the plus for positive charge
            if (currentElement.GetCharge() > 0)
            {
                DrawDot(x + 1, y + 0, CHARGE_COLOR);
                DrawDot(x + 1, y + 2, CHARGE_COLOR);
            }
        }
    }

    // Draw covalent bond lines:
    for (int i = 0; i < BondSide_Count; i++)
    {
        BondSide side = (BondSide)i;
        if (currentElement.GetBondTypeFor(side) == BondType_Covalent)
        { 
			DrawCovalentLines(side, currentElement.GetBondDataFor(side), stringWidth, stringHeight); 
			isCovalent = true;
			covalentSide = i;
		}
    }

    // Draw potential reaction:
    if (currentElement.HasBondType(BondType_Potential))
    {
        // Draw border on the cube
        //NOTE: Relies on screen being square.
        for (int i = 0; i < SCREEN_WIDTH; i++)
        {
            DrawDot(i, 0, POTENTIAL_COLOR);
            DrawDot(i, 1, POTENTIAL_COLOR);
            DrawDot(i, SCREEN_HEIGHT - 1, POTENTIAL_COLOR);
            DrawDot(i, SCREEN_HEIGHT - 2, POTENTIAL_COLOR);
            DrawDot(0, i, POTENTIAL_COLOR);
            DrawDot(1, i, POTENTIAL_COLOR);
            DrawDot(SCREEN_WIDTH - 1, i, POTENTIAL_COLOR);
            DrawDot(SCREEN_WIDTH - 2, i, POTENTIAL_COLOR);
        }
    }

    // Draw electrons:
    DrawLewisDots(stringWidth, stringHeight);

    // Draw stuff for assisting debugging rotation logic
    #ifdef DEBUG_ROTATION_LOGIC
    for (int i = 2; i < SCREEN_WIDTH; i++)
    {
        v.fb32.plot(vec(i, 0), 15);
        v.fb32.plot(vec(i, 1), 15);
    }

    v.fb32.plot(vec(0, 1), 0);
    v.fb32.plot(vec(1, 1), 0);
    v.fb32.plot(vec(1, 0), 0);
    // Red    = 0   degrees
    // Lime   = 90  degrees
    // Cyan   = 180 degrees
    // Purple = 270 degrees
    v.fb32.plot(vec(0, 0), 8 + rotation * 2);
    #endif

    isDirty = false;
	isCovalent = false;
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
        AssertAlways(); // Invalid character
    }

    // Draw the glyph:
    for (int i = 0; i < CODERS_CRUX_GLYPH_HEIGHT; i++)
    {
        for (int j = 0; j < CODERS_CRUX_GLYPH_WIDTH; j++)
        {
            DrawDot(x + j, y + i, coders_crux[j + i * CODERS_CRUX_GLYPH_WIDTH + c * CODERS_CRUX_GLYPH_SIZE]);
        }
    }
}

void ElementCube::DrawNumAt(int x, int y, int num, int color)
{
    if (num < 0 || num >= 10)
    {
        num = 10; // Draw the bad number symbol
    }

    for (int i = 0; i < NUMBER_FONT_GLYPH_HEIGHT; i++)
    {
        for (int j = 0; j < NUMBER_FONT_GLYPH_WIDTH; j++)
        {
            if (number_font[j + i * NUMBER_FONT_GLYPH_WIDTH + num * NUMBER_FONT_GLYPH_SIZE])
            {
                DrawDot(x + j, y + i, color);
            }
        }
    }
}

void ElementCube::DrawCovalentLines(BondSide side, int count, int stringWidth, int stringHeight)
{
    // The default data value for bonds is 0, so we want to change it to 1 if nobody set it:
    if (count < 1)
    { count = 1; }

    const int seperation = 2;
    int offset = -count / 2 * seperation;
    for (int i = 0; i < count; i++)
    {
        DrawCovalentLine(side, stringWidth, stringHeight, offset);
        offset += seperation;
    }
}

void ElementCube::DrawCovalentLine(BondSide side, int stringWidth, int stringHeight, int offset)
{
    const int whitspace = 4;
    Assert(side >= 0 && side < BondSide_Count);
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT / 2;
    int direction = 1;

    if (side == BondSide_Top || side == BondSide_Left)
    { direction = -1; }

    if (side == BondSide_Right || side == BondSide_Left)
    {
        x += direction * (stringWidth / 2 + whitspace);
        y += offset;

        for (; x >= 0 && x < SCREEN_WIDTH; x += direction)
        {
            DrawDot(x, y, CHARGE_COLOR);
        }
    }
    else if (side == BondSide_Top || side == BondSide_Bottom)
    {
        x += offset;
        y += direction * (stringHeight / 2 + whitspace);

        for (; y >= 0 && y < SCREEN_HEIGHT; y += direction)
        {
            DrawDot(x, y, CHARGE_COLOR);
        }
    }
}

void ElementCube::DrawLewisDots(int stringWidth, int stringHeight)
{
	if (currentElement.GetCharge() != 0 && currentElement.GetNumOuterElectrons() == 8)
	{
		return;
	}
	int numOuterElectrons = currentElement.GetNumOuterElectrons() - currentElement.GetSharedElectrons();
	LOG("\n\tnumOuterElectrons = %d", numOuterElectrons);

	//right, left, top, bottom
	//int position[4];
	//If we need to draw covalent lines, then we enter
	if (isCovalent)
	{
		//right, left, top, bottom
		int position[4] = { 0, 0, 0, 0 };
		LOG("\n\tCovalentSide = %d", covalentSide);
		if (covalentSide == 1)
		{
			switch (numOuterElectrons)
			{
			case 1:
				position[2] = 1;
				break;
			case 2:
				position[2] = 1;
				position[3] = 1;
				break;
			case 3:
				position[0] = 1;
				position[2] = 1;
				position[3] = 1;
				break;
			case 4:
				position[0] = 1;
				position[2] = 2;
				position[3] = 1;
				break;
			case 5:
				position[0] = 1;
				position[2] = 2;
				position[3] = 2;
				break;
			case 6:
				position[0] = 2;
				position[2] = 2;
				position[3] = 2;
				break;
			case 7:
				position[0] = 2;
				position[1] = 1;
				position[2] = 2;
				position[3] = 2;
				break;
			default:
				break;
			}
		}
		else if (covalentSide == 3)
		{
			switch (numOuterElectrons)
			{
			case 1:
				position[2] = 1;
				break;
			case 2:
				position[2] = 1;
				position[3] = 1;
				break;
			case 3:
				position[1] = 1;
				position[2] = 1;
				position[3] = 1;
				break;
			case 4:
				position[1] = 1;
				position[2] = 2;
				position[3] = 1;
				break;
			case 5:
				position[1] = 1;
				position[2] = 2;
				position[3] = 2;
				break;
			case 6:
				position[1] = 2;
				position[2] = 2;
				position[3] = 2;
				break;
			case 7:
				position[0] = 1;
				position[1] = 2;
				position[2] = 2;
				position[3] = 2;
				break;
			default:
				break;
			}
		}

		for (int s = LFirst; s <= LLast; s++)
		{
			int e = position[s];
			LOG("\n\ts = %d", s);
			LOG("\n\te = %d", e);
			if (e == 0)
			{
				continue;
			}
			int x = SCREEN_WIDTH / 2;
			int y = SCREEN_HEIGHT / 2;

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
				x++; // Looks more natural one pixel to the right
				y -= stringHeight / 2 + 2;
				break;
			case LBottom:
				x++; // Looks more natural one pixel to the right
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
			for (; e > 0; e--)
			{
				DrawDot(x, y, ELECTRON_COLOR);
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
	//Otherwise we'll draw them normally
	else
	{
		for (int s = LFirst; s <= LLast; s++)
		{

			int e = (numOuterElectrons + 3 - s) / 4;

			int x = SCREEN_WIDTH / 2;
			int y = SCREEN_HEIGHT / 2;
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
				x++; // Looks more natural one pixel to the right
				y -= stringHeight / 2 + 2;
				break;
			case LBottom:
				x++; // Looks more natural one pixel to the right
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
			for (; e > 0; e--)
			{
				DrawDot(x, y, ELECTRON_COLOR);

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

}

/*
void ElementCube::DrawLewisDots(int stringWidth, int stringHeight)
{
    //TODO: This is a bit of a hack. We need to properly set the outer electron count to 0 when the orbital is filled.
    if (currentElement.GetCharge() != 0 && currentElement.GetNumOuterElectrons() == 8)
    {
        return;
    }
    int numOuterElectrons = currentElement.GetNumOuterElectrons() - currentElement.GetSharedElectrons();
    for (int s = LFirst; s <= LLast; s++)
    {
        // Calculate the number of electrons on this side
        int e = (numOuterElectrons + 3 - s) / 4;

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
                x++; // Looks more natural one pixel to the right
                y -= stringHeight / 2 + 2;
                break;
            case LBottom:
                x++; // Looks more natural one pixel to the right
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
            DrawDot(x, y, ELECTRON_COLOR);

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
*/
