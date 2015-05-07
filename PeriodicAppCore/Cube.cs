using System;
using System.Diagnostics;
using System.Drawing;

namespace PeriodicAppCore
{
    public abstract class Cube
    {
        protected const int paletteSize = 16;
        protected Color[] palette = new Color[paletteSize];
        protected Cube[] neighbors = new Cube[(int)Side.NumSides];

        public Cube GetNeighbor(Side side)
        {
            if (!side.IsValid())
            { throw new ArgumentOutOfRangeException("side", "Side must be a valid side!"); }

            return neighbors[(int)side];
        }

        public void SetNeighbor(Side side, Cube other)
        {
            if (!side.IsValid())
            { throw new ArgumentOutOfRangeException("side", "Side must be a valid side!"); }

            int sideNum = (int)side;
            Side oppositeSide = side.GetOpposite();
            int oppositeSideNum = (int)oppositeSide;

            Cube currentNeighbor = GetNeighbor(side);

            // Don't do anything if we already have this neighbor
            if (currentNeighbor == other)
            { return; }

            // If we had an old neighbor, trigger the appropriate event and remove ourselves as their neighbor
            if (currentNeighbor != null)
            {
                Debug.Assert(currentNeighbor.neighbors[oppositeSideNum] == this);
                Periodic.OnNeighborRemoved(side, this, currentNeighbor);
                currentNeighbor.neighbors[oppositeSideNum] = null;
            }

            // Set the new neighbor, and trigger the appropriate event if relevant.
            neighbors[sideNum] = other;

            if (other == null)
            { return; } // Nothing left to do if the new neighbor is no neighbor.

            Periodic.OnNeighborAdded(side, this, other);

            // If the new neighbor has an existing neighbor in our new spot, we dispatch a removed event
            Cube otherOldNeighbor = other.GetNeighbor(oppositeSide);
            if (otherOldNeighbor != null)
            { Periodic.OnNeighborRemoved(oppositeSide, other, otherOldNeighbor); }

            // Set ourselves as our nieghbor's neighbor
            other.neighbors[oppositeSideNum] = this;
        }

        public void OnTouch()
        {
            Periodic.OnTouch(this);
        }

        protected internal void SetPaletteColor(int colorIndex, Color color)
        {
            if (colorIndex < 0 || colorIndex >= palette.Length)
            { throw new ArgumentOutOfRangeException("colorIndex", "colorIndex is out of the range of allowed palette indices."); }

            palette[colorIndex] = color;
        }

        protected internal void SetPaletteColor(int colorIndex, int r, int g, int b)
        {
            SetPaletteColor(colorIndex, Color.FromArgb(r, g, b));
        }

        public abstract void Paint();
        public abstract void ClearScreen(int colorIndex);
        public abstract void DrawPoint(int x, int y, int colorIndex);
    }
}
