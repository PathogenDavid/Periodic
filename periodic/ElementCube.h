#ifndef __ELEMENTCUBE_H__
#define __ELEMENTCUBE_H__

#include "Element.h"

#include <sifteo.h>
using namespace Sifteo;

#define BG_COLOR 7 //White
#define CHARGE_COLOR 8 //Red
#define ELECTRON_COLOR 15 //Pink

class ElementCube
{
    private:
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
    
    private:
        void DrawCharAt(int x, int y, char c);
        void DrawLewisDots(int stringWidth, int stringHeight);
};

#endif
