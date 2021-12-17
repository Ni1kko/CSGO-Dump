using System;
using System.Threading;

namespace ScriptKidAntiCheat.Utils
{
    public abstract class U_ThreadedComponent : IDisposable
    {
        protected virtual string ThreadName => nameof(U_ThreadedComponent);

        protected virtual TimeSpan ThreadTimeout { get; set; } = new TimeSpan(0, 0, 0, 3);

        protected virtual TimeSpan ThreadFrameSleep { get; set; } = new TimeSpan(0, 0, 0, 0, 1);

        private Thread Thread { get; set; }
        protected U_ThreadedComponent()
        {
            Thread = new Thread(ThreadStart)
            {
                Name = ThreadName, // Virtual member call in constructor
            };
        }
        public virtual void Dispose()
        {
            Thread.Interrupt();
            if (!Thread.Join(ThreadTimeout))
            {
                Thread.Abort();
            }
            Thread = default;
        }
        public void Start()
        {
            Thread.Start();
        }
        private void ThreadStart()
        {
            try
            {
                while (true)
                {
                    FrameAction();
                    Thread.Sleep(ThreadFrameSleep);
                }
            }
            catch (ThreadInterruptedException)
            {
            }
        }
        protected abstract void FrameAction();

    }
}
