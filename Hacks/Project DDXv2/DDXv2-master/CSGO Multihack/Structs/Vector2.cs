namespace DDX.Structs
{
    public struct Vector2
    {
        public float X;
        public float Y;

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public bool Equals(Vector2 other)
        {
            return X.Equals(other.X) && Y.Equals(other.Y);
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is Vector2 && Equals((Vector2)obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (X.GetHashCode() * 397) ^ Y.GetHashCode();
            }
        }

        public static bool operator ==(Vector2 left, Vector2 right)
        {
            if (left.X != right.X) return false;
            if (left.Y != right.Y) return false;

            return true;
        }

        public static bool operator !=(Vector2 left, Vector2 right)
        {
            return !(left == right);
        }

        public static Vector2 operator +(Vector2 left, Vector2 right)
        {
            left.X += right.X;
            left.Y += right.Y;

            return left;
        }

        public static Vector2 operator -(Vector2 left, Vector2 right)
        {
            left.X -= right.X;
            left.Y -= right.Y;

            return left;
        }

        public static Vector2 operator *(Vector2 left, float right)
        {
            left.X *= right;
            left.Y *= right;

            return left;
        }

        public static Vector2 operator /(Vector2 left, float right)
        {
            left.X /= right;
            left.Y /= right;

            return left;
        }

        public bool IsValid()
        {
            return X != -1f && Y != -1f;
        }

        public override string ToString()
        {
            return $"< {X} , {Y} >";
        }
    }
}
