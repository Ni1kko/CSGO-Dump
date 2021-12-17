using FadeAPI.Structs;

namespace DDX_Multihack
{
    internal struct PathData
    {
        //[swap]
        public readonly int Ticks;
        public Vector3 Vector;
        //[/swap]

        public PathData(Vector3 vector, int ticks)
        {
            //[swap]
            Ticks  = ticks;
            Vector = vector;
            //[/swap]
        }
    }
}
