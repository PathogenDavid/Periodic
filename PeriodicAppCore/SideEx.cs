using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PeriodicAppCore
{
    public static class SideEx
    {
        public static bool IsValid(this Side side)
        {
            return side >= (Side)0 && side < Side.NumSides;
        }

        public static Side GetOpposite(this Side side)
        {
            switch (side)
            {
                case Side.Left: return Side.Right;
                case Side.Right: return Side.Left;
                case Side.Top: return Side.Bottom;
                case Side.Bottom: return Side.Top;
                default: return Side.NoSide;
            }
        }
    }
}
