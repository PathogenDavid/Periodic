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

        //! Initializes this ElementCube with the specified cube ID and initial element index
        void Init(int cubeId, int initialElementNum);
    public:
        //! Creates a new ElementCube with the specified cube ID and initial element index
        ElementCube(int cubeId, int initialElementNum = 0);
        //! Creates a new ElementCube with the specified cube ID and initial element symbol
        ElementCube(int cubeId, const char* initialElementSymbol);

        //! Changes the current element to the next element in the element database
        void GoToNextElement();
        //! Resets this current element to its natural, basic state
        void ResetElement();
        //! Renders this ElementCube
        void Render();

        //! Reacts this ElementCube with one other ElementCube.
        void ReactWith(ElementCube* other);
        //! Reacts this ElementCube with two other ElementCube instances.
        void ReactWith(ElementCube* other1, ElementCube* other2);

        //! Returns the cube ID associated with this ElementCube.
        int GetCubeId();

        //! Returns the Element associated with this ElementCube.
        Element* GetElement();
    
    private:
        //! Internal supporting function for drawing a bitmap font character at the specifed location
        void DrawCharAt(int x, int y, char c);
        //! Internal supporting function for drawing a bitmap font number at the specified location
        void DrawNumAt(int x, int y, int num, int color);
        //! Internal supporting function for drawing the lewis dot structure dots for this cube.
        //! The dots will be draw about the center of the cube, around the text assosicated with the given text dimensions.
        void DrawLewisDots(int stringWidth, int stringHeight);
};

#endif
