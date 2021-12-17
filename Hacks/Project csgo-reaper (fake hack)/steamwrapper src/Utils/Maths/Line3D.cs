// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.Maths.Line3D
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using SharpDX;
using System;

namespace ScriptKidAntiCheat.Utils.Maths
{
  public readonly struct Line3D
  {
    public readonly Vector3 StartPoint;
    public readonly Vector3 EndPoint;

    public Line3D(Vector3 startPoint, Vector3 endPoint)
    {
      this.StartPoint = startPoint;
      this.EndPoint = endPoint;
    }

    public ValueTuple<Vector3, Vector3> ClosestPointsBetween(Line3D other)
    {
      if (this.IsParallelTo(other))
        return new ValueTuple<Vector3, Vector3>(this.StartPoint, other.ClosestPointTo(this.StartPoint, false));
      Vector3 direction1 = this.GetDirection();
      Vector3 direction2 = other.GetDirection();
      Vector3 right = Vector3.op_Subtraction(this.StartPoint, other.StartPoint);
      float num1 = direction1.Dot(direction1);
      float num2 = direction1.Dot(direction2);
      float num3 = direction2.Dot(direction2);
      float num4 = direction1.Dot(right);
      float num5 = direction2.Dot(right);
      float num6 = (float) (((double) num2 * (double) num5 - (double) num3 * (double) num4) / ((double) num1 * (double) num3 - (double) num2 * (double) num2));
      float num7 = (float) (((double) num1 * (double) num5 - (double) num2 * (double) num4) / ((double) num1 * (double) num3 - (double) num2 * (double) num2));
      return new ValueTuple<Vector3, Vector3>(Vector3.op_Addition(this.StartPoint, Vector3.op_Multiply(num6, direction1)), Vector3.op_Addition(other.StartPoint, Vector3.op_Multiply(num7, direction2)));
    }

    public ValueTuple<Vector3, Vector3> ClosestPointsBetween(
      Line3D other,
      bool mustBeOnSegments)
    {
      if (!this.IsParallelTo(other) || !mustBeOnSegments)
      {
        ValueTuple<Vector3, Vector3> valueTuple = this.ClosestPointsBetween(other);
        if (!mustBeOnSegments)
          return valueTuple;
        Vector3 vector3_1 = Vector3.op_Subtraction(valueTuple.Item1, this.StartPoint);
        if ((double) ((Vector3) ref vector3_1).Length() <= (double) this.GetLength())
        {
          Vector3 vector3_2 = Vector3.op_Subtraction(valueTuple.Item1, this.EndPoint);
          if ((double) ((Vector3) ref vector3_2).Length() <= (double) this.GetLength())
          {
            Vector3 vector3_3 = Vector3.op_Subtraction(valueTuple.Item2, other.StartPoint);
            if ((double) ((Vector3) ref vector3_3).Length() <= (double) other.GetLength())
            {
              Vector3 vector3_4 = Vector3.op_Subtraction(valueTuple.Item2, other.EndPoint);
              if ((double) ((Vector3) ref vector3_4).Length() <= (double) other.GetLength())
                return valueTuple;
            }
          }
        }
      }
      Vector3 vector3_5 = other.ClosestPointTo(this.StartPoint, true);
      Vector3 vector3_6 = Vector3.op_Subtraction(vector3_5, this.StartPoint);
      float num1 = ((Vector3) ref vector3_6).Length();
      ValueTuple<Vector3, Vector3> valueTuple1 = new ValueTuple<Vector3, Vector3>(this.StartPoint, vector3_5);
      float num2 = num1;
      Vector3 vector3_7 = other.ClosestPointTo(this.EndPoint, true);
      vector3_6 = Vector3.op_Subtraction(vector3_7, this.EndPoint);
      float num3 = ((Vector3) ref vector3_6).Length();
      if ((double) num3 < (double) num2)
      {
        valueTuple1 = new ValueTuple<Vector3, Vector3>(this.EndPoint, vector3_7);
        num2 = num3;
      }
      Vector3 vector3_8 = this.ClosestPointTo(other.StartPoint, true);
      vector3_6 = Vector3.op_Subtraction(vector3_8, other.StartPoint);
      float num4 = ((Vector3) ref vector3_6).Length();
      if ((double) num4 < (double) num2)
      {
        valueTuple1 = new ValueTuple<Vector3, Vector3>(vector3_8, other.StartPoint);
        num2 = num4;
      }
      Vector3 vector3_9 = this.ClosestPointTo(other.EndPoint, true);
      vector3_6 = Vector3.op_Subtraction(vector3_9, other.EndPoint);
      if ((double) ((Vector3) ref vector3_6).Length() < (double) num2)
        valueTuple1 = new ValueTuple<Vector3, Vector3>(vector3_9, other.EndPoint);
      return valueTuple1;
    }

    public Vector3 ClosestPointTo(Vector3 value, bool mustBeOnSegment)
    {
      Vector3 direction = this.GetDirection();
      float num = Vector3.op_Subtraction(value, this.StartPoint).Dot(direction);
      if (mustBeOnSegment)
      {
        if ((double) num < 0.0)
          num = 0.0f;
        float length = this.GetLength();
        if ((double) num > (double) length)
          num = length;
      }
      return Vector3.op_Addition(this.StartPoint, Vector3.op_Multiply(num, direction));
    }

    public Vector3 GetDirection()
    {
      return Vector3.op_Subtraction(this.EndPoint, this.StartPoint).Normalized();
    }

    public float GetLength()
    {
      Vector3 vector3 = Vector3.op_Subtraction(this.EndPoint, this.StartPoint);
      return ((Vector3) ref vector3).Length();
    }

    public bool IsParallelTo(Line3D other)
    {
      return this.GetDirection().IsParallelTo(other.GetDirection(), 1E-06f);
    }
  }
}
