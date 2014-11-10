#include "assets.gen.h"
#include "coders_crux.gen.h"
#include "Element.h"
#include "ElementCube.h"
#include "periodic.h" 

#include <sifteo.h>

using namespace Sifteo;

#define NUM_CUBES 2

static Metadata M = Metadata()
    .title("Periodic Project")
    .package("edu.ksu.periodic", "0.1")
    .icon(Icon)
    .cubeRange(NUM_CUBES)
;

ElementCube cubes[NUM_CUBES] =
{
    ElementCube(0, "Li"),
    ElementCube(1, "F")
};

static bool isRelease[CUBE_ALLOCATION];//TODO: Investigate if this should be initialized with CubeID.isTouch on startup.

void ProcessNeighborhood();

void OnPress(unsigned cubeId);
void OnRelease(unsigned cubeId);
void OnTouch(void* sender, unsigned cubeId);

void OnNeighborAdd(void* sender, unsigned firstId, unsigned firstSide, unsigned secondId, unsigned secondSide);
void OnNeighborRemove(void* sender, unsigned firstId, unsigned firstSide, unsigned secondId, unsigned secondSide);

void main()
{
    Events::cubeTouch.set(OnTouch);
    Events::neighborAdd.set(OnNeighborAdd);
    Events::neighborRemove.set(OnNeighborRemove);
    
    TimeStep timeStep;
    while (1)
    {
        for (unsigned i = 0; i < NUM_CUBES; i++)
        {
            cubes[i].Render();
        }

        System::paint();
        timeStep.next();
    }
}

void ProcessNeighborhood()
{
    // Reset all of the cubes:
    for (int i = 0; i < NUM_CUBES; i++)
    { cubes[i].ResetElement(); }

    // For each cube, process interaction events for all cubes with an id > than its.
    //TODO: This does not allow for any order of interaction. (As in, a lower ID cube can steal a reaction from a higher one if both touch the same cube.)
    for (int i = 0; i < NUM_CUBES; i++)
    {
        Neighborhood nh(i);
        for (int j = i + 1; j < NUM_CUBES; j++)
        {
            if (nh.sideOf(j) == NO_SIDE) { continue; }
            cubes[i].ReactWith(&cubes[j]);
        }
    }
}

void OnPress(unsigned cubeId)
{
}

void OnRelease(unsigned cubeId)
{
    LOG("Going to next elemenet on cube %d.\n", cubeId);

    // Move the tapped cube to the next element
    cubes[cubeId].GoToNextElement();

    ProcessNeighborhood();
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

void OnNeighborAdd(void* sender, unsigned firstId, unsigned firstSide, unsigned secondId, unsigned secondSide)
{
    ProcessNeighborhood();
}

void OnNeighborRemove(void* sender, unsigned firstId, unsigned firstSide, unsigned secondId, unsigned secondSide)
{
    ProcessNeighborhood();
}
