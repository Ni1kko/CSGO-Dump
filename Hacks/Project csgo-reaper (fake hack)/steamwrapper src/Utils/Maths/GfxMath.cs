// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.Maths.GfxMath
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using ScriptKidAntiCheat.Internal.Raw;
using SharpDX;
using System;

namespace ScriptKidAntiCheat.Utils.Maths
{
  public static class GfxMath
  {
    private const double _PI_Over_180 = 0.0174532925199433;

    public static float AngleTo(this Vector3 vector, Vector3 other)
    {
      return (float) Math.Acos((double) vector.Normalized().Dot(other.Normalized()));
    }

    public static Vector3 Cross(this Vector3 left, Vector3 right)
    {
      return Vector3.Cross(left, right);
    }

    public static float Dot(this Vector3 left, Vector3 right)
    {
      return Vector3.Dot(left, right);
    }

    public static Vector3[] GetCircleVertices(
      Vector3 origin,
      Vector3 normal,
      double radius,
      int segments)
    {
      Matrix matrix = GfxMath.GetOrthogonalMatrix(normal, origin);
      Vector3[] circleVertices2D = GfxMath.GetCircleVertices2D((Vector3) null, radius, segments);
      for (int index = 0; index < circleVertices2D.Length; ++index)
        circleVertices2D[index] = matrix.Transform(circleVertices2D[index]);
      return circleVertices2D;
    }

    public static Vector3[] GetCircleVertices2D(Vector3 origin, double radius, int segments)
    {
      Vector3[] vector3Array = new Vector3[segments + 1];
      double num1 = 2.0 * Math.PI / (double) segments;
      for (int index = 0; index < segments; ++index)
      {
        double num2 = num1 * (double) index;
        vector3Array[index] = new Vector3((float) (double) origin.X + (float) (radius * Math.Cos(num2)), (float) (double) origin.Y + (float) (radius * Math.Sin(num2)), 0.0f);
      }
      vector3Array[segments] = vector3Array[0];
      return vector3Array;
    }

    public static Vector3[][] GetHalfSphere(
      Vector3 origin,
      Vector3 normal,
      float radius,
      int segments,
      int layers)
    {
      ((Vector3) ref normal).Normalize();
      Vector3[][] vector3Array = new Vector3[layers][];
      for (int index = 0; index < layers; ++index)
      {
        float num = radius - (float) index * (radius / (float) layers);
        Vector3 origin1 = Vector3.op_Addition(origin, Vector3.op_Multiply(normal, (float) Math.Cos(Math.Asin((double) num / (double) radius)) * radius));
        vector3Array[index] = GfxMath.GetCircleVertices(origin1, normal, (double) num, segments);
      }
      return vector3Array;
    }

    public static Matrix GetMatrix(
      Vector3 xAxis,
      Vector3 yAxis,
      Vector3 zAxis,
      Vector3 origin)
    {
      Matrix matrix = (Matrix) null;
      matrix.M11 = xAxis.X;
      matrix.M12 = xAxis.Y;
      matrix.M13 = xAxis.Z;
      matrix.M21 = yAxis.X;
      matrix.M22 = yAxis.Y;
      matrix.M23 = yAxis.Z;
      matrix.M31 = zAxis.X;
      matrix.M32 = zAxis.Y;
      matrix.M33 = zAxis.Z;
      matrix.M41 = origin.X;
      matrix.M42 = origin.Y;
      matrix.M43 = origin.Z;
      matrix.M44 = (__Null) 1.0;
      return matrix;
    }

    public static void GetOrthogonalAxis(
      Vector3 normal,
      out Vector3 xAxis,
      out Vector3 yAxis,
      out Vector3 zAxis)
    {
      zAxis = normal.Normalized();
      Vector3 vector3;
      ((Vector3) ref vector3).\u002Ector(0.0f, 0.0f, 1f);
      float num = zAxis.AngleTo(vector3);
      xAxis = (double) num < Math.PI / 4.0 || (double) num > 3.0 * Math.PI / 4.0 ? new Vector3(0.0f, 1f, 0.0f).Cross(zAxis).Normalized() : zAxis.Cross(vector3).Normalized();
      yAxis = zAxis.Cross(xAxis).Normalized();
    }

    public static Matrix GetOrthogonalMatrix(Vector3 normal, Vector3 origin)
    {
      Vector3 xAxis;
      Vector3 yAxis;
      Vector3 zAxis;
      GfxMath.GetOrthogonalAxis(normal, out xAxis, out yAxis, out zAxis);
      return GfxMath.GetMatrix(xAxis, yAxis, zAxis, origin);
    }

    public static bool IsParallelTo(this Vector3 vector, Vector3 other, float tolerance = 1E-06f)
    {
      return Math.Abs(1.0 - (double) Math.Abs(vector.Normalized().Dot(other.Normalized()))) <= (double) tolerance;
    }

    public static Vector3 Normalized(this Vector3 value)
    {
      return Vector3.Normalize(value);
    }

    public static Matrix ToMatrix(in this matrix3x4_t matrix)
    {
      Matrix matrix1 = (Matrix) null;
      matrix1.M11 = (__Null) (double) matrix.m00;
      matrix1.M12 = (__Null) (double) matrix.m01;
      matrix1.M13 = (__Null) (double) matrix.m02;
      matrix1.M21 = (__Null) (double) matrix.m10;
      matrix1.M22 = (__Null) (double) matrix.m11;
      matrix1.M23 = (__Null) (double) matrix.m12;
      matrix1.M31 = (__Null) (double) matrix.m20;
      matrix1.M32 = (__Null) (double) matrix.m21;
      matrix1.M33 = (__Null) (double) matrix.m22;
      matrix1.M41 = (__Null) (double) matrix.m30;
      matrix1.M42 = (__Null) (double) matrix.m31;
      matrix1.M43 = (__Null) (double) matrix.m32;
      matrix1.M44 = (__Null) 1.0;
      return matrix1;
    }

    public static Vector3 Transform(in this Matrix matrix, Vector3 value)
    {
      double num = 1.0 / ((double) matrix.M14 * (double) value.X + (double) matrix.M24 * (double) value.Y + (double) matrix.M34 * (double) value.Z + (double) matrix.M44);
      return new Vector3((float) (((double) matrix.M11 * (double) value.X + (double) matrix.M21 * (double) value.Y + (double) matrix.M31 * (double) value.Z + (double) matrix.M41) * num), (float) (((double) matrix.M12 * (double) value.X + (double) matrix.M22 * (double) value.Y + (double) matrix.M32 * (double) value.Z + (double) matrix.M42) * num), (float) (((double) matrix.M13 * (double) value.X + (double) matrix.M23 * (double) value.Y + (double) matrix.M33 * (double) value.Z + (double) matrix.M43) * num));
    }

    public static double DegreeToRadian(this double degree)
    {
      return degree * (Math.PI / 180.0);
    }
  }
}
