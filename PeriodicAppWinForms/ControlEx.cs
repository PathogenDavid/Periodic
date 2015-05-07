using System;
using System.Windows.Forms;

namespace PeriodicAppWinForms
{
    internal static class ControlEx
    {
        public static void Invoke(this Control control, Action action)
        {
            control.Invoke(action);
        }
    }
}
