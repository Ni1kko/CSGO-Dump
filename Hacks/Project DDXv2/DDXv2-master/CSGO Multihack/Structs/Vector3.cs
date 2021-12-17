using System;

#pragma warning disable 660,661

namespace DDX.Structs
{
    public struct Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public Vector3(float x, float y, float z = 0)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static bool operator ==(Vector3 left, Vector3 right)
        {
            if (left.X != right.X) return false;
            if (left.Y != right.Y) return false;
            if (left.Z != right.Z) return false;

            return true;
        }

        public static bool operator !=(Vector3 left, Vector3 right)
        {
            return !(left == right);
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            left.X += right.X;
            left.Y += right.Y;
            left.Z += right.Z;

            return left;
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            left.X -= right.X;
            left.Y -= right.Y;
            left.Z -= right.Z;

            return left;
        }

        public static Vector3 operator *(Vector3 left, float right)
        {
            left.X *= right;
            left.Y *= right;
            left.Z *= right;

            return left;
        }

        public static Vector3 operator /(Vector3 left, float right)
        {
            left.X /= right;
            left.Y /= right;
            left.Z /= right;

            return left;
        }

        public double Length()
        {
            return Math.Sqrt(X * (double)X + Y * (double)Y + Z * (double)Z);
        }

        public float DotProduct(Vector3 v)
        {
            return (float)(X * (double)v.X + Y * (double)v.Y + Z * (double)v.Z);
        }

        public override string ToString()
        {
            return $"< {X} , {Y} , {Z} >";
        }
    }
}
