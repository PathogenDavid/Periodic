#include "assets.gen.h"
#include "coders_crux.gen.h"
#include "Element.h"
#include "ElementCube.h"
#include "Reaction.h"
#include "periodic.h" 
#include "Trie.h"

#include <sifteo.h>

using namespace Sifteo;

static Metadata M = Metadata()
    .title("Periodic Project")
    .package("edu.ksu.periodic", "0.2")
    .icon(Icon)
    .cubeRange(NUM_CUBES)
;

//! Pointer to the array of the ElementCube instances used in this program. There should be one for every cube in the simulation.
ElementCube* cubes;

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

//!Test method for Trie
void createTrie(void);
void initTrie(Trie* t);
//! Program entry-point, initializes all state and event handlers, and handles the main program loop
void main()
{
    // Initialize ElementCubes:
    // Due to a bug in the Sifteo linker, we can't statically initialize these outside of main.
    // If we do, sometimes they will initialize before the periodic table and will cause crashes trying to access it.
    ElementCube _cubes[NUM_CUBES] =
    {
        ElementCube(0, "H"),
        ElementCube(1, "H"),
        ElementCube(2, "F"),
        //TODO: Make this less terrible and just instantiate all the cubes, but only actually use the ones available.
        /*
        Yes this looks kinda terrible, but it is useful for manual testing.

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
#if NUM_CUBES > 3
        ElementCube(3, "F"),
#if NUM_CUBES > 4
        ElementCube(4, "Li"),
#if NUM_CUBES > 5
        ElementCube(5, "I"),
#if NUM_CUBES > 6
        ElementCube(6, "Li"),
#if NUM_CUBES > 7
        ElementCube(7, "Be"),
#if NUM_CUBES > 8
        ElementCube(8, "Ca"),
#if NUM_CUBES > 9
        ElementCube(9, "H"),
#if NUM_CUBES > 10
        ElementCube(10, "H"),
#if NUM_CUBES > 11
        ElementCube(11, "H")
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
    };
    cubes = _cubes;

    //InitCubes();
    Events::cubeTouch.set(OnTouch);
    Events::neighborAdd.set(OnNeighborAdd);
    Events::neighborRemove.set(OnNeighborRemove);
	
	createTrie();
    
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
void AddNeighbors(int forCube, bool* hasBeenUsed)
{
    Neighborhood nh(forCube);
    for (int i = 0; i < NUM_SIDES; i++)
    {
        // If there is no neighbor on this side, skip
        if (!nh.hasNeighborAt((Side)i))
        { continue; }

        // Get the cube at that side
        int neighborCube = nh.cubeAt((Side)i);

        // Don't doubly process cubes
        if (hasBeenUsed[neighborCube])
        { continue; }

        // Add the new neighbor as a bond, mark it as used, and process its neighbors too
        ElementCube* neighbor = &cubes[neighborCube];
        cubes[forCube].GetElement()->AddBond((BondSide)i, neighbor->GetElement()); // (This will also add this element to the reaction.)
        hasBeenUsed[neighborCube] = true;
        AddNeighbors(neighborCube, hasBeenUsed);
    }
}

void ProcessNeighborhood()
{
    // Reset all of the cubes:
    for (int i = 0; i < NUM_CUBES; i++)
    { cubes[i].ResetElement(); }

    static Reaction reaction; //TODO: Reaction is static so that it is reset the next time we process the neighborhood. (This should be fixed with the TODO below.)
    bool hasBeenUsed[NUM_CUBES];
    PeriodicMemset(hasBeenUsed, 0, sizeof(hasBeenUsed));

    for (int i = 0; i < NUM_CUBES; i++)
    {
        LOG("ProcessNeighborhood, iter = %d\n", i);
        if (hasBeenUsed[i])
        {
            continue;
        }

        Neighborhood nh(i);
        reaction.Reset();

        // Find the entire reaction:
        reaction.Add(cubes[i].GetElement());
        AddNeighbors(i, hasBeenUsed);

        if (reaction.Process())
        {
            //TODO: Right now resetting the reaction destroys state being used by the elements involved in the reaction,
            // so we need to add reference counting to instances of Compound or something.
            return;
        }
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

void createTrie()
{
	Trie* trie = new Trie();
	initTrie(trie);
}


void initTrie(Trie* t)
{
	t->addBond("HH", COVALENT);
	t->addBond("LiH", COVALENT);
	t->addBond("NaH", COVALENT);
	t->addBond("KH", COVALENT);
	t->addBond("CsH", COVALENT);
	t->addBond("FrH", COVALENT);
	t->addBond("FF", COVALENT);
	t->addBond("ClCl", COVALENT);
	t->addBond("BrBr", COVALENT);
	t->addBond("II", COVALENT);
	t->addBond("FCl", COVALENT);
	t->addBond("FBr", COVALENT);
	t->addBond("FI", COVALENT);
	t->addBond("ClBr", COVALENT);
	t->addBond("ClI", COVALENT);
	t->addBond("HF", COVALENT);
	t->addBond("HCl", COVALENT);
	t->addBond("HBr", COVALENT);
	t->addBond("HI", COVALENT);
	t->addBond("HAt", COVALENT);
	t->addBond("LiF", IONIC);
	t->addBond("LiCl", IONIC);
	t->addBond("LiBr", IONIC);
	t->addBond("LiI", IONIC);
	t->addBond("NaF", IONIC);
	t->addBond("NaCl", IONIC);
	t->addBond("NaBr", IONIC);
	t->addBond("NaI", IONIC);
	t->addBond("KF", IONIC);
	t->addBond("KCl", IONIC);
	t->addBond("KBr", IONIC);
	t->addBond("KI", IONIC);
	t->addBond("RbF", IONIC);
	t->addBond("RbCl", IONIC);
	t->addBond("RbBr", IONIC);
	t->addBond("RbI", IONIC);
	t->addBond("CsF", IONIC);
	t->addBond("CsCl", IONIC);
	t->addBond("CsBr", IONIC);
	t->addBond("CsI", IONIC);
	t->addBond("FrF", IONIC);
	t->addBond("FrCl", IONIC);
	t->addBond("FrBr", IONIC);
	t->addBond("FrI", IONIC);
	t->addBond("BeHH", COVALENT);
	t->addBond("MgHH", COVALENT);
	t->addBond("CaHH", IONIC);
	t->addBond("SrHH", IONIC);
	t->addBond("BaHH", IONIC);
	t->addBond("BeFF", COVALENT);
	t->addBond("BeClCl", COVALENT);
	t->addBond("BeBrBr", COVALENT);
	t->addBond("BeII", COVALENT);
	t->addBond("MgFF", COVALENT);
	t->addBond("MgClCl", COVALENT);
	t->addBond("MgBrBr", COVALENT);
	t->addBond("MgII", COVALENT);
	t->addBond("CaFF", COVALENT);
	t->addBond("CaClCl", COVALENT);
	t->addBond("CaBrBr", COVALENT);
	t->addBond("CaII", COVALENT);
	t->addBond("SrFF", COVALENT);
	t->addBond("SrClCl", COVALENT);
	t->addBond("SrBrBr", COVALENT);
	t->addBond("SrII", COVALENT);
	t->addBond("BaFF", COVALENT);
	t->addBond("BaClCl", COVALENT);
	t->addBond("BaBrBr", COVALENT);
	t->addBond("BaII", COVALENT);
	t->addBond("SiOO", COVALENT);
	t->addBond("SO", COVALENT);
	t->addBond("SOO", COVALENT);
	t->addBond("SOOO", COVALENT);
	t->addBond("SSO", COVALENT);
	t->addBond("SSOO", COVALENT);
	t->addBond("HClO", COVALENT);
	t->addBond("HBrO", COVALENT);

}
