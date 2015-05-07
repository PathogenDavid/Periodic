using System;
using System.Diagnostics;
using System.Drawing;

namespace PeriodicAppCore
{
    public abstract class Cube
    {
        public const int Width = 32;
        public const int Height = 32;
        protected const int paletteSize = 16;
        protected Color[] palette = new Color[paletteSize];
        protected Cube[] neighbors = new Cube[(int)Side.NumSides];

        protected virtual void OnNeighborsChanged()
        {
        }

        public Cube GetNeighbor(Side side)
        {
            if (!side.IsValid())
            { throw new ArgumentOutOfRangeException("side", "Side must be a valid side!"); }

            return neighbors[(int)side];
        }

        private void SetNeighbor(Side side, Cube other, bool skipAddEvent)
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
                currentNeighbor.OnNeighborsChanged();
            }

            // Set the new neighbor, and trigger the appropriate event if relevant.
            neighbors[sideNum] = other;
            OnNeighborsChanged();

            // Nothing left to do if the new neighbor is no neighbor.
            if (other == null)
            { return; }

            if (!skipAddEvent)
            { Periodic.OnNeighborAdded(side, this, other); }

            // Set the neighbor backwards for the new neighbor
            other.SetNeighbor(oppositeSide, this, true);
        }

        public void SetNeighbor(Side side, Cube other)
        {
            SetNeighbor(side, other, false);
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
