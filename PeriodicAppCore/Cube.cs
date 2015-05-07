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
        protected Cube[] lastNeighbors = new Cube[(int)Side.NumSides];

        protected virtual void OnNeighborsChanged()
        {
        }

        public Cube GetNeighborSinceLastPaint(Side side)
        {
            if (!side.IsValid())
            { throw new ArgumentOutOfRangeException("side", "Side must be a valid side!"); }

            return lastNeighbors[(int)side];
        }

        public Cube GetNeighbor(Side side)
        {
            if (!side.IsValid())
            { throw new ArgumentOutOfRangeException("side", "Side must be a valid side!"); }

            return neighbors[(int)side];
        }

        public void SetNeighbor(Side side, Cube other)
        {
            lock (Periodic.NeighborhoodMutex)
            {
                if (!side.IsValid())
                { throw new ArgumentOutOfRangeException("side", "Side must be a valid side!"); }

                int sideNum = (int)side;
                Side oppositeSide = side.GetOpposite();
                int oppositeSideNum = (int)oppositeSide;

                Cube oldNeighbor = GetNeighbor(side);

                if (oldNeighbor == other)
                { return; }

                // Remove ourselves from our old neighbor
                if (oldNeighbor != null)
                {
                    Debug.Assert(oldNeighbor.neighbors[oppositeSideNum] == this);
                    oldNeighbor.neighbors[oppositeSideNum] = null;
                }

                // Set our new neighbor:
                neighbors[sideNum] = other;

                // Set us on our new neighbor, and remove the old neighbor if the new neighbor had one:
                if (other != null)
                {
                    // Remove the new neighbor from the old neighbor
                    if (other.neighbors[oppositeSideNum] != null)
                    { other.neighbors[oppositeSideNum].neighbors[sideNum] = null; }

                    // Set us as a neighbor of the new neighbor
                    other.neighbors[oppositeSideNum] = this;
                }
            }
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

        bool neighborhoodIsStale = false;

        public virtual void Paint()
        {
            // Back up the neighbors array so that the cubes get the state that is accurate to the events they've received rather than
            // the live version. We also use this to call OnNeighborsChanged and inform Periodic that the neighborhoods have changed.
            neighborhoodIsStale = false;
            for (int i = 0; i < neighbors.Length; i++)
            {
                if (lastNeighbors[i] != neighbors[i])
                {
                    lastNeighbors[i] = neighbors[i];
                    neighborhoodIsStale = true;
                }
            }

            if (neighborhoodIsStale)
            {
                Periodic.MarkNeighborhoodsAsStale();
            }
        }

        //HACK: This is kinda weird to get around issues with deadlocking with Invoke in OnNeighborsChanged.
        public void HandleNeighborhoodChanging()
        {
            if (neighborhoodIsStale)
            {
                neighborhoodIsStale = false;
                OnNeighborsChanged();
            }
        }

        public abstract void ClearScreen(int colorIndex);
        public abstract void DrawPoint(int x, int y, int colorIndex);
    }
}
