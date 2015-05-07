using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PeriodicAppWinForms
{
    public partial class CubeForm : Form
    {
        private static int nextNum = 0;

        private readonly int cubeNumber;

        public CubeForm()
        {
            InitializeComponent();

            // Figure out that cube number this form is for:
            cubeNumber = nextNum;
            nextNum++;

            // Position this form on the screen in a logical way.
            const int sep = 20;

            Left = 0;
            Top = 0;

            for (int num = cubeNumber; num > 0; num--)
            {
                Left += Width + sep;

                // Flow onto the next row if we run out of space to the right.
                if ((Left + Width) > Screen.PrimaryScreen.WorkingArea.Right)
                {
                    Left = 0;
                    Top += Height + sep;
                }
            }
        }
    }
}
