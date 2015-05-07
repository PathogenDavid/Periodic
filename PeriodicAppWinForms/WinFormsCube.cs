using PeriodicAppCore;

namespace PeriodicAppWinForms
{
    public class WinFormsCube : Cube
    {
        public CubeForm Form { get; private set; }

        public WinFormsCube()
        {
            Form = new CubeForm();
            Form.Show();
        }

        public override void Paint() { }
        public override void ClearScreen(int colorIndex) { }
        public override void DrawPoint(int x, int y, int colorIndex) { }
    }
}
