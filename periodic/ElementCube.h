#ifndef __ELEMENTCUBE_H__
#define __ELEMENTCUBE_H__

#include "Element.h"

#include <sifteo.h>
using namespace Sifteo;

#define BG_COLOR 7 //White
#define CHARGE_COLOR 8 //Red
#define ELECTRON_COLOR 15 //Pink
#define COVALENT_COLOR_OUTTER 3 // Gray
#define COVALENT_COLOR_INNER 9 // Yellow
#define POTENTIAL_COLOR 9 // Yellow

enum CubeRotation
{
    CubeRotatation0,
    CubeRotatation90,
    CubeRotatation180,
    CubeRotatation270,
    CubeRotatationCount
};

//! ElementCube is responsible for linking an Element to a Sifteo Cube.
//! Its primary role is rendering the element to the screen.
//! Note: Cube events are handled in main.cpp, not here.
class ElementCube
{
    private:
        //! The cube ID associated with this ElementCube.
        int cubeId;
        //! A Sifteo CubeID handle associated with this ElementCube.
        CubeID cube;
        //! The video buffer for this cube's screen
        VideoBuffer v;
        //! True when the video buffer is dirty and should be re-rendered
        bool isDirty;
        //! The current element index used for the currentElement.
        int currentElementNum;
        //! The underlying Element info used for this ElementCube.
        Element currentElement;
        //! The amount this cube's screens hould be rotated.
        unsigned char rotation;

        //! Initializes this ElementCube with the specified cube ID and initial element index
        void Init(int cubeId, int initialElementNum);
    public:
        ElementCube();
        //! Creates a new ElementCube with the specified cube ID and initial element index
        ElementCube(int cubeId, int initialElementNum = 0);
        //! Creates a new ElementCube with the specified cube ID and initial element symbol
        ElementCube(int cubeId, const char* initialElementSymbol);

        //! Changes the current element to the next element in the element database
        void GoToNextElement();
        //! Resets this current element to its natural, basic state
        void Reset();
        //! Renders this ElementCube
        void Render();

        //! Returns the cube ID associated with this ElementCube.
        int GetCubeId();

        //! Returns the Element associated with this ElementCube.
        Element* GetElement();
        
        void SetDirty();// TODO: Make it so we can subscribe to an event when the element is updated?

        //! Rotates the cube by the specified number of degrees, clockwise
        void RotateByClockwise(CubeRotation rotation);
        //! Rotates the cube by the specified number of degrees, counterclockwise
        void RotateByCounterClockwise(CubeRotation rotation);
        //! Sets the rotation of this cube to the value of another cube's rotation
        void RotateTo(ElementCube* otherCube);
        //! Sets the rotation of this cube to a specific value
        void RotateTo(CubeRotation rotation);
        //! Gets the rotation of this cube
        CubeRotation GetRotation();
    private:
        //! Internal supporting function for drawing a bitmap font character at the specified location
        void DrawCharAt(int x, int y, char c);
        //! Internal supporting function for drawing a bitmap font number at the specified location
        void DrawNumAt(int x, int y, int num, int color);
        //! Internal supporting function for drawing the lewis dot structure dots for this cube.
        //! The dots will be draw about the center of the cube, around the text assosicated with the given text dimensions.
        void DrawLewisDots(int stringWidth, int stringHeight);
        //! Internal supporting function for drawing a line for a covalent bond on specified side of this cube
        void DrawCovalentLine(BondSide sides, int stringWidth, int stringHeight, int offset);
        //! Internal supporting function for drawing the lines for covalent bonds on the specified side of this cube
        void DrawCovalentLines(BondSide side, int count, int stringWidth, int stringHeight);

        //! Draws a dot on the screen
        void DrawDot(int x, int y, unsigned int color);
};

#endif
