// Decompiled with JetBrains decompiler
// Type: ScriptKidAntiCheat.Utils.U_ThreadedComponent
// Assembly: SteamWrapper, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b9d60819726d5649
// MVID: 52813C95-239A-43DA-B46E-DCB9B069035E
// Assembly location: C:\Users\Ni1kk\Desktop\New folder\bin\SteamWrapper.exe

using System;
using System.Threading;

namespace ScriptKidAntiCheat.Utils
{
  public abstract class U_ThreadedComponent : IDisposable
  {
    protected virtual string ThreadName
    {
      get
      {
        return nameof (U_ThreadedComponent);
      }
    }

    protected virtual TimeSpan ThreadTimeout { get; set; } = new TimeSpan(0, 0, 0, 3);

    protected virtual TimeSpan ThreadFrameSleep { get; set; } = new TimeSpan(0, 0, 0, 0, 1);

    private Thread Thread { get; set; }

    protected U_ThreadedComponent()
    {
      this.Thread = new Thread(new ThreadStart(this.ThreadStart))
      {
        Name = this.ThreadName
      };
    }

    public virtual void Dispose()
    {
      this.Thread.Interrupt();
      if (!this.Thread.Join(this.ThreadTimeout))
        this.Thread.Abort();
      this.Thread = (Thread) null;
    }

    public void Start()
    {
      this.Thread.Start();
    }

    private void ThreadStart()
    {
      try
      {
        while (true)
        {
          this.FrameAction();
          Thread.Sleep(this.ThreadFrameSleep);
        }
      }
      catch (ThreadInterruptedException ex)
      {
      }
    }

    protected abstract void FrameAction();
  }
}
