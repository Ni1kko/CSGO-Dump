// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Internal.Raw.mstudiobone_t
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using SharpDX;

namespace ScriptKidAntiCheat.Internal.Raw
{
  public struct mstudiobone_t
  {
    public int sznameindex;
    public int parent;
    public unsafe fixed int bonecontroller[6];
    public Vector3 pos;
    public Quaternion quat;
    public Vector3 rot;
    public Vector3 posscale;
    public Vector3 rotscale;
    public matrix3x4_t poseToBone;
    public Quaternion qAlignment;
    public int flags;
    public int proctype;
    public int procindex;
    public int physicsbone;
    public int surfacepropidx;
    public int contents;
    public unsafe fixed int unused[8];
  }
}
