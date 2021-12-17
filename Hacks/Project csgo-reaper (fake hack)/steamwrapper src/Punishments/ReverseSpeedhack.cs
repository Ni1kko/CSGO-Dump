// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Punishments.ReverseSpeedhack
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using System;
using System.Timers;

namespace ScriptKidAntiCheat.Punishments
{
  internal class ReverseSpeedhack : Punishment
  {
    public ReverseSpeedhack()
      : base(10000, true, 500)
    {
      ReplayLogger.Log(this.GetType().Name, true, "");
    }

    public override void Tick(object source, ElapsedEventArgs e)
    {
      try
      {
        this.ActivatePunishment();
      }
      catch (Exception ex)
      {
      }
    }

    public void ActivatePunishment()
    {
      if (!this.CanActivate())
        return;
      Program.GameConsole.SendCommand("+duck");
    }

    public override void Reset()
    {
      Program.GameConsole.SendCommand("-duck");
    }
  }
}
