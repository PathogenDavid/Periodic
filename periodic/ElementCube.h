#ifndef __ELEMENTCUBE_H__
#define __ELEMENTCUBE_H__

#include "Element.h"

#include <sifteo.h>
using namespace Sifteo;

#define BG_COLOR 7 //White
#define CHARGE_COLOR 8 //Red
#define ELECTRON_COLOR 15 //Pink
#define COVALENT_COLOR 3 // Gray
#define POTENTIAL_COLOR 9 // Yellow

class ElementCube
{
    private:
        int cubeId;
        CubeID cube;
        VideoBuffer v;
        bool isDirty;
        int currentElementNum;
        Element currentElement;

        void Init(int cubeId, int initialElementNum);
    public:
        ElementCube(int cubeId, int initialElementNum = 0);
        ElementCube(int cubeId, const char* initialElementSymbol);

        void GoToNextElement();
        void ResetElement();
        void Render();

        void ReactWith(ElementCube* other);
        void ReactWith(ElementCube* other1, ElementCube* other2);

        int GetCubeId();
    
    private:
        void DrawCharAt(int x, int y, char c);
        void DrawLewisDots(int stringWidth, int stringHeight);
};

#endif
