using System;
using System.Runtime.InteropServices;

namespace PeriodicAppCore
{
    public static partial class Periodic
    {
        //----------------------------------------------------------------------
        #region External Functions
        //----------------------------------------------------------------------
        private const string periodicDll = "periodic.dll";

        [DllImport(periodicDll, EntryPoint = "main", CallingConvention = CallingConvention.Cdecl)]
        private static extern void MainThread();

        //[DllImport(periodicDll, CallingConvention = CallingConvention.Cdecl)]
        //private static extern void OnNeighborAdd(IntPtr sender, uint firstId, uint firstSide, uint secondId, uint secondSide);

        //[DllImport(periodicDll, CallingConvention = CallingConvention.Cdecl)]
        //private static extern void OnNeighborRemove(IntPtr sender, uint firstId, uint firstSide, uint secondId, uint secondSide);

        [DllImport(periodicDll, CallingConvention = CallingConvention.Cdecl)]
        private static extern void OnNeighborhoodChanged();

        [DllImport(periodicDll, CallingConvention = CallingConvention.Cdecl)]
        private static extern void OnTouch(IntPtr sender, uint cubeId);

        [DllImport(periodicDll, CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetCubeCount();

        [DllImport(periodicDll, CallingConvention = CallingConvention.Cdecl)]
        public static extern void RequestStop();

        [DllImport(periodicDll, CallingConvention = CallingConvention.Cdecl)]
        private static extern void InstallCallbacks
            (
            LogCallback logCallback,
            PaintCallback paintCallback,
            NeighborHoodCubeAtCallback neighborHoodCubeAtCallback,
            ClearScreenCallback clearScreenCallback,
            DrawPointCallback drawPointCallback,
            SetPaletteColorCallback setPaletteColorCallback
            );
        //----------------------------------------------------------------------
        #endregion External Functions
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        #region External Callback Delegates
        //----------------------------------------------------------------------
        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private delegate void LogCallback(string message);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void PaintCallback();

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate byte NeighborHoodCubeAtCallback(byte relativeTo, Side side);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void ClearScreenCallback(byte cube, uint colorIndex);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void DrawPointCallback(byte cube, uint x, uint y, uint colorIndex);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void SetPaletteColorCallback(byte cube, uint index, byte r, byte g, byte b);
        //----------------------------------------------------------------------
        #endregion External Callback Delegates
        //----------------------------------------------------------------------
    }
}
