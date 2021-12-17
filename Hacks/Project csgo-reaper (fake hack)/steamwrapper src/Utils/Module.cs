using System;
using System.Diagnostics;

namespace ScriptKidAntiCheat
{
    public class Module : IDisposable
    {
        public Process Process { get; private set; }

        public ProcessModule ProcessModule { get; private set; }
        public Module(Process process, ProcessModule processModule)
        {
            Process = process;
            ProcessModule = processModule;
        }
        public void Dispose()
        {
            Process = default;

            ProcessModule.Dispose();
            ProcessModule = default;
        }
    }
}
