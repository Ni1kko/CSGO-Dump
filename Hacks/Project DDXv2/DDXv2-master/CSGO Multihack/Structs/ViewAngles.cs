#pragma warning disable 660,661

namespace DDX.Structs
{
    public struct ViewAngles
    {
        public float Pitch;
        public float Yaw;
        //public float Roll;

        public ViewAngles(float pitch, float yaw)
        {
            Pitch = pitch;
            Yaw = yaw;
        }

        public static bool operator ==(ViewAngles left, ViewAngles right)
        {
            if (left.Pitch != right.Pitch) return false;
            if (left.Yaw != right.Yaw) return false;

            return true;
        }

        public static bool operator !=(ViewAngles left, ViewAngles right)
        {
            return !(left == right);
        }

        public static ViewAngles operator +(ViewAngles left, ViewAngles right)
        {
            left.Pitch += right.Pitch;
            left.Yaw += right.Yaw;

            return left;
        }

        public static ViewAngles operator -(ViewAngles left, ViewAngles right)
        {
            left.Pitch -= right.Pitch;
            left.Yaw -= right.Yaw;

            return left;
        }

        public static ViewAngles operator *(ViewAngles left, float right)
        {
            left.Pitch *= right;
            left.Yaw *= right;

            return left;
        }

        public static ViewAngles operator /(ViewAngles left, float right)
        {
            left.Pitch /= right;
            left.Yaw /= right;

            return left;
        }

        public override string ToString()
        {
            return $"< {Pitch} , {Yaw} >";
        }
    }
}
