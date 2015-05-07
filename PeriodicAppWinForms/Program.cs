using PeriodicAppCore;
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace PeriodicAppWinForms
{
    static class Program
    {
        static List<WinFormsCube> cubes = new List<WinFormsCube>();

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // Initialize GUI stuff:
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            // Start Periodic:
            for (int i = 0; i < Periodic.MaxCubeCount; i++)
            {
                WinFormsCube cube = new WinFormsCube();
                Periodic.GiveCube(cube);
                cubes.Add(cube);

                // Hide cubes other than the 0th one from the taskbar
                if (i > 0)
                {
                    cube.Form.ShowInTaskbar = false;
                }

                // Listen to some events on all of the cubes:
                cube.Form.SizeChanged += Form_SizeChanged;
                cube.Form.FormClosing += Form_FormClosing;
            }

            ignoreSizeChangedEvents = false;
            Periodic.Stopped += Periodic_Stopped;
            Periodic.Start();

            // Start the mesage pump:
            Application.Run();
        }

        static void Periodic_Stopped(object sender, EventArgs e)
        {
            Application.Exit();
        }

        static void Form_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (Periodic.IsRunning)
            {
                Periodic.RequestStop();
                e.Cancel = true;
                return;
            }

            Application.Exit();
        }

        static bool ignoreSizeChangedEvents = true; // We ignore all size changed events until we get set up

        /// <summary>
        /// Keeps the minimized state of all cubes in sync.
        /// </summary>
        static void Form_SizeChanged(object sender, EventArgs e)
        {
            if (ignoreSizeChangedEvents)
            { return; }

            ignoreSizeChangedEvents = true; // Don't create infinite recursive events

            Form senderForm = sender as Form;
            if (senderForm == null)
            { return; }

            foreach (WinFormsCube cube in cubes)
            {
                if (cube.Form == senderForm)
                { continue; }

                cube.Form.WindowState = senderForm.WindowState;
            }

            ignoreSizeChangedEvents = false;
        }
    }
}
