#include "assets.gen.h"
#include "coders_crux.gen.h"
#include "Element.h"
#include "ElementCube.h"
#include "Reaction.h"
#include "periodic.h" 
#include "Set.h"

#include <sifteo.h>

using namespace Sifteo;

static Metadata M = Metadata()
    .title("Periodic Project")
    .package("edu.ksu.periodic", "0.2")
    .icon(Icon)
    .cubeRange(NUM_CUBES)
;

//! ElementCube instances used in this program. There should be one for every cube in the simulation.
ElementCube cubes[NUM_CUBES];

/*
Some reactions you can make with this set:
H-H    :: Covalent
H-F    :: Covalent
H-Li   :: Covalent
H-Be   :: Potential
F-Li   :: Ionic
F-Be   :: Potential
Li-I   :: Ionic
F-Be-F :: Covalent
F-Ca-F :: Ionic
H-Be-H :: Covalent
*/
const char* defaultCubeSymbols[] =
{
    "H",
    "H",
    "F",
    "F",
    "Li",
    "I",
    "Ca",
    "Be",
    "Ca",
    "H",
    "H",
    "H"
};

//! Processes the entire Sifteo Cube neighborhood and handles any reactions present in it
void ProcessNeighborhood();

//! Called when a specific cube is pressed (as in, touched after not being touched.)
void OnPress(unsigned cubeId);
//! Called when a specific cube is released (as in, not touched after being touched.)
void OnRelease(unsigned cubeId);
//! Raw Sifteo event handler for OnTocuh, you probably want to use OnPress and OnRelease instead.
void OnTouch(void* sender, unsigned cubeId);

//! Raw Sifteo event handler used to process cubes touching
void OnNeighborAdd(void* sender, unsigned firstId, unsigned firstSide, unsigned secondId, unsigned secondSide);
//! Raw Sifteo event handler used to process cubes untouching
void OnNeighborRemove(void* sender, unsigned firstId, unsigned firstSide, unsigned secondId, unsigned secondSide);

//! Program entry-point, initializes all state and event handlers, and handles the main program loop
void main()
{
    LOG("Enterting main...\n");

    // Initialize ElementCubes:
    // Due to a bug in the Sifteo linker, we can't statically initialize these at all.
    // If we do, sometimes they will initialize before the periodic table and will cause crashes trying to access it.
    for (int i = 0; i < NUM_CUBES; i++)
    {
        cubes[i].Initialize(i, defaultCubeSymbols[i]);
    }

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

////////////////////////////////////////////////////////////////////////////////
// Reaction Building and Processing
////////////////////////////////////////////////////////////////////////////////
Side GetOppositeSideFor(int relativeTo, int otherCube)
{
    Neighborhood nh(otherCube);
    for (int i = 0; i < NUM_SIDES; i++)
    {
        // Check if this side has the cube we are relative to:
        if (nh.cubeAt((Side)i) == relativeTo)
        {
            return (Side)i;
        }
    }

    Assert(false); // If we made it this far, the two cubes given aren't actually touching!
    return NO_SIDE;
}

void SetCubeRotation(ElementCube* neighbor, int firstCubeSide, int secondCubeSide)
{
    int rotationAmount = secondCubeSide + firstCubeSide + 2;
    if (secondCubeSide == LEFT || secondCubeSide == RIGHT)
    { rotationAmount += 2; }
    rotationAmount %= (int)CubeRotatationCount;

    neighbor->RotateByClockwise((CubeRotation)rotationAmount);
}

void AddNeighbors(int forCube, bool* hasBeenUsed)
{
    Neighborhood nh(forCube);
    for (int i = 0; i < NUM_SIDES; i++)
    {
        // If there is no neighbor on this side, skip
        if (!nh.hasCubeAt((Side)i))
        { continue; }

        // Get the cube at that side
        int neighborCube = nh.cubeAt((Side)i);
        ElementCube* neighbor = &cubes[neighborCube];

        // Don't doubly process cubes
        if (hasBeenUsed[neighborCube])
        { continue; }

        // "Rotate" the side we detected to the orientation of the cube so we set the side of the bond correctly.
        int bondSide = (i + cubes[forCube].GetRotation()) % NUM_SIDES;

        // Add the new neighbor as a bond, mark it as used, and process its neighbors too
        cubes[forCube].GetElement()->AddBond((BondSide)bondSide, neighbor->GetElement()); // (This will also add this element to the reaction.)

        // Figure out the rotation needed for the cube:
        neighbor->RotateTo(&cubes[forCube]); // Cube should always start with parent's rotation amount.
        SetCubeRotation(neighbor, i, (int)GetOppositeSideFor(forCube, neighborCube));

        hasBeenUsed[neighborCube] = true;
        AddNeighbors(neighborCube, hasBeenUsed);
    }
}

Set<Reaction, MAX_REACTIONS> reactions;
void ProcessNeighborhood()
{
    // Reset all of the cubes:
    for (int i = 0; i < NUM_CUBES; i++)
    { cubes[i].Reset(); }

    // Destroy old reactions
    for (int i = 0; i < reactions.Count(); i++)
    { delete reactions[i]; }
    reactions.Clear();

    bool hasBeenUsed[NUM_CUBES];
    PeriodicMemset(hasBeenUsed, 0, sizeof(hasBeenUsed));

    for (int i = 0; i < NUM_CUBES; i++)
    {
        if (hasBeenUsed[i])
        { continue; }

        // Abort if the reaction list's space (and therefore the Reaction ObjectPool) is depleted:
        if (reactions.Count() >= reactions.Capacity())
        {
            LOG("WARNING: Some cubes are going unprocessed because we've depleted the Reaction ObjectPool!\n");
            break;
        }

        Neighborhood nh(i);
        Reaction* reaction = new Reaction();

        // Find the entire reaction:
        reaction->Add(cubes[i].GetElement());
        hasBeenUsed[i] = true;
        AddNeighbors(i, hasBeenUsed);

        // Process the reaction, and save it to our list of active reactions if it resulted in any compounds:
        if (reaction->Process())
        { reactions.Add(reaction); }
        else
        { delete reaction; }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Sifteo Events
////////////////////////////////////////////////////////////////////////////////
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

//! Internal accounting for OnTouch, used to separate presses from releases.
static bool isRelease[CUBE_ALLOCATION];//TODO: Investigate if this should be initialized with CubeID.isTouch on startup.

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
