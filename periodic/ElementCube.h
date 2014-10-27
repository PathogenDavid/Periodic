#ifndef __ELEMENTCUBE_H__
#define __ELEMENTCUBE_H__

#include "Element.h"

#include <sifteo.h>
using namespace Sifteo;

#define BG_COLOR 7 //White

class ElementCube
{
    private:
        CubeID cube;
        VideoBuffer v;
        bool isDirty;
        int currentElementNum;
        Element currentElement;
    public:
        ElementCube(int cubeId, int initialElementNum = 0);

        void GoToNextElement();
        void ResetElement();
        void Render();

        void ReactWith(ElementCube* other);
    
    private:
        void DrawCharAt(int x, int y, char c);
};

#endif
