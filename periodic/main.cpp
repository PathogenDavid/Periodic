#include "assets.gen.h"
#include "coders_crux.gen.h"
#include "Element.h"
#include "ElementCube.h"
#include "periodic.h" 

#include <sifteo.h>

using namespace Sifteo;

#define NUM_CUBES 3
#define MAX_REACTION_SIZE 3

static Metadata M = Metadata()
    .title("Periodic Project")
    .package("edu.ksu.periodic", "0.1")
    .icon(Icon)
    .cubeRange(NUM_CUBES)
;

ElementCube cubes[NUM_CUBES] =
{
    ElementCube(0, "H"),
    ElementCube(1, "Be"),
    ElementCube(2, "H")
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

int CountNeighbors(Neighborhood* nh)
{
    int ret = 0;

    for (int i = 0; i < NUM_SIDES; i++)
    {
        if (nh->hasCubeAt((Side)i))
        {
            ret++;
        }
    }

    return ret;
}

bool IsReactant(ElementCube** reactants, int numReactants, ElementCube* elementCube)
{
    for (int i = 0; i < numReactants; i++)
    {
        if (reactants[i] == elementCube)
        {
            return true;
        }
    }

    return false;
}

void FindReactants(ElementCube** reactants, int* numReactants, ElementCube* root)
{
    LOG("Adding %d to reactant collection.\n", root->GetCubeId());
    reactants[*numReactants] = root;
    (*numReactants)++;

    Neighborhood nh(root->GetCubeId());
    for (int i = 0; i < NUM_SIDES; i++)
    {
        if (nh.hasNeighborAt((Side)i))
        {
            ElementCube* neighbor = &cubes[nh.cubeAt((Side)i)];

            // If this neighbor isn't already in the reactant collection, we search it too.
            if (!IsReactant(reactants, *numReactants, neighbor))
            { FindReactants(reactants, numReactants, neighbor); }
        }
    }
}

void ProcessNeighborhood()
{
    // Reset all of the cubes:
    for (int i = 0; i < NUM_CUBES; i++)
    { cubes[i].ResetElement(); }

    //TODO: Currently only handles a single reaction. (IE: You can't use 8 cubes and make two separate reactions.)
    
    // Look or a cube that has neighbors
    int rootCube = -1;
    for (int i = 0; i < NUM_CUBES; i++)
    {
        Neighborhood nh(i);
        int numNeighbors = CountNeighbors(&nh);

        if (numNeighbors == 0)
        {
            continue;
        }

        rootCube = i;
        LOG("Cube #%d with %d neighbors chosen as root of reaction\n", i, numNeighbors);
        break;
    }

    // No reaction
    if (rootCube == -1)
    {
        return;
    }

    // Find all cubes in reaction:
    ElementCube* reactants[MAX_REACTION_SIZE];
    int numReactants = 0;

    FindReactants(reactants, &numReactants, &cubes[rootCube]);
    LOG("Got %d reactants.\n", numReactants);

    if (numReactants == 3)
    {
        reactants[0]->ReactWith(reactants[1], reactants[2]);
    }
    else if (numReactants == 2)
    {
        reactants[0]->ReactWith(reactants[1]);
    }
    else
    {
        // Currently this is an unexpected condition
        ASSERT(false);
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
