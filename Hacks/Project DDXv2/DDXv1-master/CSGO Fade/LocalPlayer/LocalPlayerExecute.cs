using FadeAPI.Helpers;
using FadeAPI.Memory;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace FadeAPI.LocalPlayer
{
    public class LocalPlayerExecute
    {
        public void Jump()
        {
            if (Global.LastExecute > Environment.TickCount)
                return;

            Global.Memory.WriteBytes(MemoryBase.ClientDll + MemoryBase.Jump.ToInt32(), new byte[] { 5 });
            Thread.Sleep(20);
            Global.Memory.WriteBytes(MemoryBase.ClientDll + MemoryBase.Jump.ToInt32(), new byte[] { 4 });

            Global.LastExecute = Environment.TickCount + 5;
        }

        public async void JumpAsync()
        {
            await Task.Run(() => Jump());
        }

        public void Attack1()
        {
            if (Global.LastExecute > Environment.TickCount)
                return;

            Global.Memory.WriteBytes(MemoryBase.ClientDll + MemoryBase.Attack1.ToInt32(), new byte[] { 5 });
            Thread.Sleep(20);
            Global.Memory.WriteBytes(MemoryBase.ClientDll + MemoryBase.Attack1.ToInt32(), new byte[] { 4 });

            Global.LastExecute = Environment.TickCount + 5;
        }

        public async void Attack1Async()
        {
            await Task.Run(() => Attack1());
        }

        public void Attack2()
        {
            if (Global.LastExecute > Environment.TickCount)
                return;

            Global.Memory.WriteBytes(MemoryBase.ClientDll + MemoryBase.Attack2.ToInt32(), new byte[] { 5 });
            Thread.Sleep(20);
            Global.Memory.WriteBytes(MemoryBase.ClientDll + MemoryBase.Attack2.ToInt32(), new byte[] { 4 });

            Global.LastExecute = Environment.TickCount + 5;
        }

        public async void Attack2Async()
        {
            await Task.Run(() => Attack2());
        }
    }
}
