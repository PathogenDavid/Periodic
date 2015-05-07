using PeriodicAppCore;
using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

namespace PeriodicAppWinForms
{
    public class WinFormsCube : Cube, IDisposable
    {
        public CubeForm Form { get; private set; }

        private Bitmap[] buffers = new Bitmap[2];
        private int backBufferIndex = 0;
        private bool backBufferIsDirty = true;

        private Bitmap BackBuffer
        {
            get { return buffers[backBufferIndex]; }
        }

        /// <summary>
        /// If changes were made, makes the current back buffer the new front buffer and selects a new back buffer.
        /// </summary>
        /// <remarks>
        /// We have to do this to avoid editing the Bitmap object while it is being drawn, as doing so results in an InvlaidOperationException.
        /// It'd be more proper to use a mutex for the front buffer, but I didn't want to experiment with modifying the form's paint event and such.
        /// </remarks>
        private void Swap()
        {
            if (!backBufferIsDirty)
            { return; }

            // The current back buffer becomes the front buffer
            Form.BackgroundImage = BackBuffer;
            Form.Invalidate();

            // And we move on to the next back buffer
            backBufferIndex++;
            if (backBufferIndex >= buffers.Length)
            { backBufferIndex = 0; }

            backBufferIsDirty = false;
        }

        private void Invalidate()
        {
            backBufferIsDirty = true;
        }

        public WinFormsCube()
        {
            for (int i = 0; i < buffers.Length; i++)
            { buffers[i] = new Bitmap(Width, Height); }

            Form = new CubeForm();
            Form.Show();
            Form.MouseDown += Form_MouseUpDown;
            Form.MouseUp += Form_MouseUpDown;
            Swap();
        }

        void Form_MouseUpDown(object sender, MouseEventArgs e)
        {
            OnTouch();
        }

        public override void Paint()
        {
            Swap();
        }

        public override void ClearScreen(int colorIndex)
        {
            if (colorIndex < 0 || colorIndex >= palette.Length)
            { throw new ArgumentOutOfRangeException("colorIndex", "colorIndex is out of the range of allowed palette indices."); }

            for (int x = 0; x < Width; x++)
            {
                for (int y = 0; y < Height; y++)
                {
                    BackBuffer.SetPixel(x, y, palette[colorIndex]);
                }
            }
            Invalidate();
        }

        public override void DrawPoint(int x, int y, int colorIndex)
        {
            if (colorIndex < 0 || colorIndex >= palette.Length)
            { throw new ArgumentOutOfRangeException("colorIndex", "colorIndex is out of the range of allowed palette indices."); }

            if (x < 0 || x >= Width || y < 0 || y >= Height)
            { Debug.Print("Warning: Trying to draw out of bounds!"); }

            BackBuffer.SetPixel(x, y, palette[colorIndex]);
            Invalidate();
        }

        bool disposed = false;
        public void Dispose()
        {
            if (disposed) { return; }

            Form.Dispose();

            for (int i = 0; i < buffers.Length; i++)
            { buffers[i].Dispose(); }
        }
    }
}
