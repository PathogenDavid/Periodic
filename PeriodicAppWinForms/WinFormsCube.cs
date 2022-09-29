using PeriodicAppCore;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

namespace PeriodicAppWinForms
{
    public class WinFormsCube : Cube, IDisposable
    {
        public CubeForm Form { get; private set; }

        private static List<WinFormsCube> cubes = new List<WinFormsCube>();

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
            Form.Move += Form_Move;
            //Form.ResizeEnd += Form_Move;
            Form.Icon = PeriodicAppWinForms.Properties.Resources.None;
            Swap();

            cubes.Add(this);
        }

        void Form_MouseUpDown(object sender, MouseEventArgs e)
        {
            OnTouch();
        }

        public override void Paint()
        {
            base.Paint();
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

        /// <summary>
        /// Figures out if this cube is close enough to any other cubes to "latch on" to them and consider them to be neighbors.
        /// </summary>
        /// <remarks>Contrary to the name's implications, this event occus after moving too.</remarks>
        void Form_Move(object sender, EventArgs e)
        {
            if (Form.WindowState != FormWindowState.Normal)
            { return; }

            const int maxSep = 15; // The maximum seperation between cubes, in pixels
            const int maxOff = 15; // The maximum offset for the perpendicular axis, in pixels

            Cube newLeft = null;
            Cube newRight = null;
            Cube newTop = null;
            Cube newBottom = null;

            // Loop through every other cube and determine if it is close enough to us:
            foreach (WinFormsCube other in cubes)
            {
                if (other == this)
                {
                    continue;
                }

                if (Math.Abs(this.Form.Left - other.Form.Left) <= maxOff)
                {
                    if (newTop == null && Math.Abs(this.Form.Top - other.Form.Bottom) <= maxSep)
                    {
                        newTop = other;
                        continue;
                    }

                    if (newBottom == null && Math.Abs(this.Form.Bottom - other.Form.Top) <= maxSep)
                    {
                        newBottom = other;
                        continue;
                    }
                }

                if (Math.Abs(this.Form.Top - other.Form.Top) <= maxOff)
                {
                    if (newLeft == null && Math.Abs(this.Form.Left - other.Form.Right) <= maxSep)
                    {
                        newLeft = other;
                        continue;
                    }

                    if (newRight == null && Math.Abs(this.Form.Right - other.Form.Left) <= maxSep)
                    {
                        newRight = other;
                        continue;
                    }
                }
            }

            // These will only raise events for ones that have changed.
            SetNeighbor(Side.Left, newLeft);
            SetNeighbor(Side.Right, newRight);
            SetNeighbor(Side.Top, newTop);
            SetNeighbor(Side.Bottom, newBottom);
        }

        protected override void OnNeighborsChanged()
        {
            if (Form.InvokeRequired)
            {
                Form.Invoke(OnNeighborsChanged);
                return;
            }

            // Set the icon to the appropriate symbol:
            string name = "";

            if (GetNeighbor(Side.Top) != null)
            { name += "N"; }
            if (GetNeighbor(Side.Right) != null)
            { name += "E"; }
            if (GetNeighbor(Side.Bottom) != null)
            { name += "S"; }
            if (GetNeighbor(Side.Left) != null)
            { name += "W"; }

            if (String.IsNullOrEmpty(name))
            { name = "None"; }

            Form.Icon = (Icon)Properties.Resources.ResourceManager.GetObject(name);
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
