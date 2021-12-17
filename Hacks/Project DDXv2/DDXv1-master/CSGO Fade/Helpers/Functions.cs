using FadeAPI.Memory;
using FadeAPI.Structs;
using System;
using System.Windows.Forms;

namespace FadeAPI.Helpers
{
    public class Functions
    {
        public static Vector3 BoneToVector3(IntPtr handle, int boneId)
        {
            var boneMatrixHandle = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(handle + MemoryOffset.BoneMatrix, 4), 0);

            var retrunVector3 = new Vector3()
            {
                //[swap]
                Y = BitConverter.ToSingle(Global.Memory.ReadBytes(boneMatrixHandle + boneId * 0x30 + 0x1C, 4), 0),
                X = BitConverter.ToSingle(Global.Memory.ReadBytes(boneMatrixHandle + boneId * 0x30 + 0xC, 4), 0),
                Z = BitConverter.ToSingle(Global.Memory.ReadBytes(boneMatrixHandle + boneId * 0x30 + 0x2C, 4), 0),
                //[/swap]
            };

            return retrunVector3;
        }

        public static Vector2 CalcAngle(Vector3 src, Vector3 dst)
        {
            var delta = new[] { src.X - dst.X, src.Y - dst.Y, src.Z - dst.Z };
            var hyp = Math.Sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

            var returnAngle = new Vector2()
            {
                //[swap]
                X = (float)Math.Atan2(delta[2], hyp) * 57.295779513082f,
                Y = (float)Math.Atan(delta[1] / delta[0]) * 57.295779513082f,
                //[/swap]
            };

            if (delta[0] >= 0.0) { returnAngle.Y += 180.0f; }

            return NormalizeAngles(returnAngle);
        }

        public static Vector2 NormalizeAngles(Vector2 angles)
        {
            //[swap]
            //[block]
            while (angles.Y > 180)
                angles.Y -= 360;
            while (angles.Y < -180)
                angles.Y += 360;
            //[/block]
            //[block]
            if (angles.X > 89)
                angles.X = 89;
            else if (angles.X < -89)
                angles.X = -89;
            //[/block]
            //[/swap]

            return angles;
        }

        public static float ScreenDistance(Vector2 src, Vector2 dst)
        {
            var distance = Math.Sqrt(
                ((int)src.X - (int)dst.X) * ((int)src.X - (int)dst.X) +
                ((int)src.Y - (int)dst.Y) * ((int)src.Y - (int)dst.Y));

            distance = Math.Round(distance, 3);
            return (float)distance;
        }

        public static Vector2 WorldToScreen(Vector3 origin, Matrix4x4 viewMatrix)
        {
            var returnVector3 = new Vector2()
            {
                //[swap]
                X = viewMatrix.M11*origin.X + viewMatrix.M12*origin.Y + viewMatrix.M13*origin.Z + viewMatrix.M14,
                Y = viewMatrix.M21*origin.X + viewMatrix.M22*origin.Y + viewMatrix.M23*origin.Z + viewMatrix.M24,
                //[/swap]
            };

            var flTemp = viewMatrix.M41 * origin.X + viewMatrix.M42 * origin.Y + viewMatrix.M43 * origin.Z + viewMatrix.M44;

            if (flTemp < 0.01f)
                return new Vector2(-1, -1);

            var invFlTemp = 1f / flTemp;

            //[swap]
            var x = Screen.PrimaryScreen.Bounds.Width / 2f;
            returnVector3.Y *= invFlTemp;
            returnVector3.X *= invFlTemp;
            var y = Screen.PrimaryScreen.Bounds.Height / 2f;
            //[/swap]

            //[swap]
            x += 0.5f * returnVector3.X * Screen.PrimaryScreen.Bounds.Width + 0.5f;
            y -= 0.5f * returnVector3.Y * Screen.PrimaryScreen.Bounds.Height + 0.5f;
            //[/swap]

            //[swap]
            returnVector3.X = x;
            returnVector3.Y = y;
            //[/swap]

            return returnVector3;
        }

        public static float VectorDistance(Vector3 src, Vector3 dst)
        {
            var distance = Math.Sqrt(
                ((int)src.X - (int)dst.X) * ((int)src.X - (int)dst.X) +
                ((int)src.Y - (int)dst.Y) * ((int)src.Y - (int)dst.Y) +
                ((int)src.Z - (int)dst.Z) * ((int)src.Z - (int)dst.Z));

            distance = Math.Round(distance, 3);
            return (float)distance;
        }
    }
}
