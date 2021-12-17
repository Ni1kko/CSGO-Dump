using System;
using DDX.Enums;
using DDX.Extensions;
using DDX.Structs;
using DDX.Structs.Config;
using SharpDX.Mathematics.Interop;

namespace DDX.Functions
{
    public static class FuncHelper
    {
        private const double DegToRad = Math.PI / 180f;
        private const double RadToDeg = 180f / Math.PI;

        public static ViewAngles CalcAngle(Vector3 src, Vector3 dst)
        {
            var delta = new[] { src.X - dst.X, src.Y - dst.Y, src.Z - dst.Z };
            var hyp = Math.Sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

            var angles = new ViewAngles
            {
                Pitch = (float)(RadToDeg * Math.Atan2(delta[2], hyp)),
                Yaw = (float)(RadToDeg * Math.Atan(delta[1] / delta[0])),
            };

            if (delta[0] >= 0f)
            {
                angles.Yaw += 180f;
            }

            return angles.NormalizeAngles();
        }

        public static float DistanceScale(Vector3 src, Vector3 dst)
        {
            return 1f / Distance(src, dst) * 200f * G.Local.GetScopeScale();
        }

        public static float Distance(Vector3 src, Vector3 dst)
        {
            var distance = Math.Sqrt(
                (src.X - dst.X) * (src.X - dst.X) +
                (src.Y - dst.Y) * (src.Y - dst.Y) +
                (src.Z - dst.Z) * (src.Z - dst.Z));

            return (float)distance;
        }

        public static float Distance(RawVector2 src, RawVector2 dst)
        {
            var distance = Math.Sqrt(
                (src.X - dst.X) * (src.X - dst.X) +
                (src.Y - dst.Y) * (src.Y - dst.Y));

            return (float)distance;
        }

        public static float Distance(ViewAngles src, ViewAngles dst)
        {
            var distance = Math.Sqrt(
                (src.Pitch- dst.Pitch) * (src.Pitch - dst.Pitch) +
                (src.Yaw - dst.Yaw) * (src.Yaw - dst.Yaw));

            return (float)distance;
        }

        public static float DistanceViewAngles(ViewAngles v1, ViewAngles v2)
        {
            var d1 = Math.Abs(v1.Yaw - v2.Yaw);
            var d2 = Math.Abs(v1.Yaw + 360f - v2.Yaw);
            var d3 = Math.Abs(v1.Yaw - 360f - v2.Yaw);

            /*
            if (d1 < d2 && d1 < d3)
            {

            }
            */
            if (d2 < d1 && d2 < d3)
            {
                v1.Yaw += 360f;
            }
            else if (d3 < d1 && d3 < d2)
            {
                v1.Yaw -= 360f;
            }

            return Distance(v1, v2);
        }

        public static float DistanceViewAngle(float v1, float v2)
        {
            var d1 = Math.Abs(v1 - v2);
            var d2 = Math.Abs(v1 + 360f - v2);
            var d3 = Math.Abs(v1 - 360f - v2);

            /*
            if (d1 < d2 && d1 < d3)
            {

            }
            */
            if (d2 < d1 && d2 < d3)
            {
                v1 += 360f;
            }
            else if (d3 < d1 && d3 < d2)
            {
                v1 -= 360f;
            }

            return Math.Abs(v1 - v2);
        }

        public static WeaponProperties GetWeaponConfig()
        {
            var cat = G.Local.GetActiveWeapon().GetCategory();

            switch (cat)
            {
                case WeaponCategory.Pistol:
                    return G.C.Aim.Pistols;
                case WeaponCategory.Sniper:
                    return G.C.Aim.Snipers;
                default:
                    return G.C.Aim.Default;
            }
        }

        public static ViewAngles NormalizeAngles(this ViewAngles angles)
        {
            while (angles.Yaw > 180f)
            {
                angles.Yaw -= 360f;
            }
            while (angles.Yaw < -180f)
            {
                angles.Yaw += 360f;
            }

            if (angles.Pitch > 89f)
            {
                angles.Pitch = 89f;
            }
            if (angles.Pitch < -89f)
            {
                angles.Pitch = -89f;
            }

            if (angles.Yaw > 180f)
            {
                angles.Yaw = 180f;
            }
            if (angles.Yaw < -180f)
            {
                angles.Yaw = -180f;
            }

            return angles;
        }

        public static Vector3 RotatePoint(Vector3 pointToRotate, Vector3 centerPoint, double angleInDegrees)
        {
            var angleInRadians = angleInDegrees * DegToRad;

            var sinTheta = Math.Sin(angleInRadians);
            var cosTheta = Math.Cos(angleInRadians);
            
            return new Vector3
            {
                X = (float)(cosTheta * (pointToRotate.X - centerPoint.X) - sinTheta * (pointToRotate.Y - centerPoint.Y) + centerPoint.X),
                Y = (float)(sinTheta * (pointToRotate.X - centerPoint.X) + cosTheta * (pointToRotate.Y - centerPoint.Y) + centerPoint.Y),
                Z = pointToRotate.Z,
            };
        }

        public static RawVector2 WorldToScreen(Vector3 origin)
        {
            var viewMatrix = GlobalRead.GetViewMatrix();

            var vec2 = new RawVector2
            {
                X = origin.X * viewMatrix.M11 + origin.Y * viewMatrix.M12 + origin.Z * viewMatrix.M13 + viewMatrix.M14,
                Y = origin.X * viewMatrix.M21 + origin.Y * viewMatrix.M22 + origin.Z * viewMatrix.M23 + viewMatrix.M24,
            };

            var temp = viewMatrix.M41 * origin.X + viewMatrix.M42 * origin.Y + viewMatrix.M43 * origin.Z + viewMatrix.M44;

            if (temp < 0.01f)
            {
                return new RawVector2(-1f, -1f);
            }

            vec2.X /= temp;
            vec2.Y /= temp;

            vec2.X = G.ScreenParameters.Width * .5f + G.ScreenParameters.Width * vec2.X * .5f + .5f;
            vec2.Y = G.ScreenParameters.Height * .5f - G.ScreenParameters.Height * vec2.Y * .5f + .5f;

            return vec2;
        }
    }
}
