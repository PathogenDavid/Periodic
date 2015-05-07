using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace PeriodicAppCore
{
    public static partial class Periodic
    {
        private static List<Cube> cubes = new List<Cube>();
        private static Queue<Action> pendingEvents = new Queue<Action>();
        private const byte InvalidCubeId = unchecked((byte)-1);
        private static bool neighborhoodsAreStale = false;

        private static int GetCubeId(Cube cube)
        {
            return cubes.IndexOf(cube);
        }

        public static int MaxCubeCount
        {
            get { return GetCubeCount(); }
        }

        internal static void OnTouch(Cube cube)
        {
            if (cube == null)
            { throw new ArgumentNullException("cube"); }

            pendingEvents.Enqueue(() => OnTouch(IntPtr.Zero, (uint)GetCubeId(cube)));
        }

        public static void MarkNeighborhoodsAsStale()
        {
            neighborhoodsAreStale = true;
        }

        private static void Log(string message)
        {
            Debug.Write(message, "Periodic");
        }

        internal static readonly object NeighborhoodMutex = new Object();

        private static void Paint()
        {
            lock (NeighborhoodMutex)
            {
                // Tell the cubes to paint
                foreach (Cube cube in cubes)
                { cube.Paint(); }

                // Dispatch any events that were queued since the last call to Paint. (This is how the Sifteo framework dispatches events.)
                while (pendingEvents.Count > 0)
                {
                    pendingEvents.Dequeue()();
                }

                // Dispatch the neighborhoods stale event
                //HACK: This isn't the same as Sifteo, which isn't the worst, but it is worth noting.
                if (neighborhoodsAreStale)
                {
                    OnNeighborhoodChanged();
                    neighborhoodsAreStale = false;
                }
            }

            // Let cubes handle stuff that should happen when their own neighborhoods change:
            foreach (Cube cube in cubes)
            {
                cube.HandleNeighborhoodChanging();
            }

            //TODO: Rate-limit painting to avoid eating CPU.
        }

        private static byte NeighborHoodCubeAt(byte relativeTo, Side side)
        {
            if (!side.IsValid())
            { throw new ArgumentOutOfRangeException("side", "Side must be a valid side!"); }

            if (relativeTo < 0 || relativeTo >= MaxCubeCount)
            { throw new ArgumentOutOfRangeException("relativeTo", "relativeTo is not a valid cube ID!"); }

            if (relativeTo >= cubes.Count)
            { return InvalidCubeId; }

            Cube relativeToCube = cubes[relativeTo];
            Cube ret = relativeToCube.GetNeighborSinceLastPaint(side);
            return ret == null ? InvalidCubeId : (byte)GetCubeId(ret);
        }

        private static void ClearScreen(byte cube, uint colorIndex)
        {
            if (cube < 0 || cube >= MaxCubeCount)
            { throw new ArgumentOutOfRangeException("cube", "cube is not a valid cube ID!"); }

            if (cube >= cubes.Count)
            { return; }

            cubes[cube].ClearScreen((int)colorIndex);
        }

        private static void DrawPoint(byte cube, uint x, uint y, uint colorIndex)
        {
            if (cube < 0 || cube >= MaxCubeCount)
            { throw new ArgumentOutOfRangeException("cube", "cube is not a valid cube ID!"); }

            if (cube >= cubes.Count)
            { return; }

            cubes[cube].DrawPoint((int)x, (int)y, (int)colorIndex);
        }

        private static void SetPaletteColor(byte cube, uint index, byte r, byte g, byte b)
        {
            if (cube < 0 || cube >= MaxCubeCount)
            { throw new ArgumentOutOfRangeException("cube", "cube is not a valid cube ID!"); }

            if (cube >= cubes.Count)
            { return; }

            cubes[cube].SetPaletteColor((int)index, (int)r, (int)g, (int)b);
        }

        // We have to keep a reference to the delegates we pass into C code or the garbae collector will try to collect them
        private readonly static LogCallback _logCallback;
        private readonly static PaintCallback _paintCallback;
        private readonly static NeighborHoodCubeAtCallback _neighborHoodCubeAtCallback;
        private readonly static ClearScreenCallback _clearScreenCallback;
        private readonly static DrawPointCallback _drawPointCallback;
        private readonly static SetPaletteColorCallback _setPaletteColorCallback;

        static Periodic()
        {
            _logCallback = Log;
            _paintCallback = Paint;
            _neighborHoodCubeAtCallback = NeighborHoodCubeAt;
            _clearScreenCallback = ClearScreen;
            _drawPointCallback = DrawPoint;
            _setPaletteColorCallback = SetPaletteColor;

            InstallCallbacks(_logCallback, _paintCallback, _neighborHoodCubeAtCallback, _clearScreenCallback, _drawPointCallback, _setPaletteColorCallback);
        }

        public static void GiveCube(Cube cube)
        {
            if (cube == null)
            { throw new ArgumentNullException("cube"); }

            if (cubes.Count >= MaxCubeCount)
            { throw new InvalidOperationException("Can't add any more cubes,"); }

            cubes.Add(cube);
        }

        public static event EventHandler Stopped;

        private static Thread thread;

        public static bool IsRunning
        {
            get { return thread != null; }
        }

        public static void Start()
        {
            thread = new Thread(PeriodicThread);
            thread.Start();
        }

        private static void PeriodicThread()
        {
            MainThread();
            thread = null;

            var stopped = Stopped;
            if (stopped != null)
            { stopped(null, new EventArgs()); }
        }
    }
}
