using System.Diagnostics;

namespace FadeAPI.Structs
{
    [DebuggerDisplay("< {X}, {Y}, {Z} >")]
    public struct Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public Vector3(float x, float y, float z)
        {
            //[swap]
            X = x;
            Y = y;
            Z = z;
            //[/swap]
        }

        public static bool operator ==(Vector3 left, Vector3 right)
        {
            if (left.X != right.X)
                return false;
            if (left.Y != right.Y)
                return false;
            if (left.Z != right.Z)
                return false;

            return true;
        }

        public static bool operator !=(Vector3 left, Vector3 right)
        {
            if (left.X == right.X &&
                left.Y == right.Y &&
                left.Z == right.Z)
                return false;

            return true;
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            //[swap]
            left.Y += right.Y;
            left.Z += right.Z;
            left.X += right.X;
            //[/swap]

            return left;
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            //[swap]
            left.Z -= right.Z;
            left.Y -= right.Y;
            left.X -= right.X;
            //[/swap]

            return left;
        }

        public override string ToString()
        {
            return $"< {X}, {Y}, {Z} >";
        }
    }

    [DebuggerDisplay("< {X}, {Y} >")]
    public struct Vector2
    {
        public float X;
        public float Y;

        public Vector2(float x, float y)
        {
            //[swap]
            Y = y;
            X = x;
            //[/swap]
        }

        public static bool operator ==(Vector2 left, Vector2 right)
        {
            if (left.X != right.X)
                return false;
            if (left.Y != right.Y)
                return false;

            return true;
        }

        public static bool operator !=(Vector2 left, Vector2 right)
        {
            if (left.X == right.X &&
                left.Y == right.Y)
                return false;

            return true;
        }

        public static Vector2 operator +(Vector2 left, Vector2 right)
        {
            //[swap]
            left.X += right.X;
            left.Y += right.Y;
            //[/swap]

            return left;
        }

        public static Vector2 operator -(Vector2 left, Vector2 right)
        {
            //[swap]
            left.X -= right.X;
            left.Y -= right.Y;
            //[/swap]

            return left;
        }

        public override string ToString()
        {
            return $"< {X}, {Y} >";
        }
    }
}