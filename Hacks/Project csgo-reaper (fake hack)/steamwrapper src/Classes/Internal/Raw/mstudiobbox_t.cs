// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Internal.Raw.mstudiobbox_t
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using SharpDX;

namespace ScriptKidAntiCheat.Internal.Raw
{
  public struct mstudiobbox_t
  {
    public int bone;
    public int group;
    public Vector3 bbmin;
    public Vector3 bbmax;
    public int szhitboxnameindex;
    public unsafe fixed int unused[3];
    public float radius;
    public unsafe fixed int pad[4];
  }
}
