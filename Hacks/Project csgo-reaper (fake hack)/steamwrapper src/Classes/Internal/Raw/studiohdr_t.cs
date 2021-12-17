// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Internal.Raw.studiohdr_t
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using SharpDX;

namespace ScriptKidAntiCheat.Internal.Raw
{
  public struct studiohdr_t
  {
    public int id;
    public int version;
    public int checksum;
    public unsafe fixed byte name[64];
    public int length;
    public Vector3 eyeposition;
    public Vector3 illumposition;
    public Vector3 hull_min;
    public Vector3 hull_max;
    public Vector3 view_bbmin;
    public Vector3 view_bbmax;
    public int flags;
    public int numbones;
    public int boneindex;
    public int numbonecontrollers;
    public int bonecontrollerindex;
    public int numhitboxsets;
    public int hitboxsetindex;
  }
}
