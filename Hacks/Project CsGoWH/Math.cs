using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace CsGoWH
{
    class Math
    {
        public static Vector2 WorldToScreenVector(Vector3 PosEnemy, float[] matrix, Vector2 monitor)
        {
            Vector2 Out;
            Out.X = PosEnemy.X * matrix[0] + PosEnemy.Y * matrix[1] + PosEnemy.Z * matrix[2] + matrix[3];
            Out.Y = PosEnemy.X * matrix[4] + PosEnemy.Y * matrix[5] + PosEnemy.Z * matrix[6] + matrix[7];
            float w = PosEnemy.X * matrix[12] + PosEnemy.Y * matrix[13] + PosEnemy.Z * matrix[14] + matrix[15];
            if (w < 0.01f)
                return new Vector2(1, 1);
            else
            {
                Out.X /= w;
                Out.Y /= w;

                Out.X *= monitor.X / 2.0f;
                Out.X += monitor.X / 2.0f;

                Out.Y *= -monitor.Y / 2.0f;
                Out.Y += monitor.Y / 2.0f;
                return new Vector2(Out.X, Out.Y);
            }
        }

        public static PointF WorldToScreenPoint(Vector3 PosEnemy, float[] matrix, Vector2 monitor)
        {
            PointF Out=new PointF(0,0);
            Out.X = PosEnemy.X * matrix[0] + PosEnemy.Y * matrix[1] + PosEnemy.Z * matrix[2] + matrix[3];
            Out.Y = PosEnemy.X * matrix[4] + PosEnemy.Y * matrix[5] + PosEnemy.Z * matrix[6] + matrix[7];
            float w = PosEnemy.X * matrix[12] + PosEnemy.Y * matrix[13] + PosEnemy.Z * matrix[14] + matrix[15];
            if (w < 0.01f)
                return new Point(1, 1);
            else
            {
                Out.X /= w;
                Out.Y /= w;

                Out.X *= monitor.X / 2.0f;
                Out.X += monitor.X / 2.0f;

                Out.Y *= -monitor.Y / 2.0f;
                Out.Y += monitor.Y / 2.0f;
                return new PointF(Out.X, Out.Y);
            }
        }
        public static PointF WorldToScreenPoint2(Vector3 PosEnemy, float[] matrix, Vector2 monitor)
        {
            PointF Out = new PointF(0, 0);
            Out.X = PosEnemy.X * matrix[0] + PosEnemy.Y * matrix[1] + PosEnemy.Z * matrix[2] + matrix[3];
            Out.Y = PosEnemy.X * matrix[4] + PosEnemy.Y * matrix[5] + PosEnemy.Z * matrix[6] + matrix[7];
            float w = PosEnemy.X * matrix[12] + PosEnemy.Y * matrix[13] + PosEnemy.Z * matrix[14] + matrix[15];
            if (w < 0.01f)
                return new Point(1, 1);
            else
            {
                Out.X /= w;
                Out.Y /= w;

                Out.X = (monitor.Y / 2 * Out.X) + (Out.X + monitor.Y / 2);
                Out.Y = (monitor.X / 2 * Out.Y) + (Out.Y+monitor.X/2);
                return new PointF(Out.X, Out.Y);
            }
        }


    }
}
