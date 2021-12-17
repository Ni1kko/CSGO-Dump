using FadeAPI.Enums;
using FadeAPI.Helpers;
using FadeAPI.Memory;
using System;
using System.Diagnostics;

namespace FadeAPI.Process
{
    public class ProcessCore
    {
        public Attach Attach()
        {
            try
            {
                if (Global.Memory != null)
                {
                    return Enums.Attach.AlreadyAttached;
                }

                var csProcess = System.Diagnostics.Process.GetProcessesByName("csgo");

                //[swap]
                //[block]
                if (csProcess.Length > 1)
                {
                    return Enums.Attach.MultipleProcesses;
                }
                //[/block]
                //[block]
                if (csProcess.Length == 0)
                {
                    return Enums.Attach.ProcessNotFound;
                }
                //[/block]
                //[/swap]

                Global.CsProcess = csProcess[0];
                Global.Memory    = new MemoryCore(csProcess[0]);

                var result = MemoryHelper.SignatureScan();

                return result == SignatureScan.Success ? Enums.Attach.Success : Enums.Attach.SignatureScanFail;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                return Enums.Attach.Exception;
            }
        }

        public Detach Detach()
        {
            try
            {
                if (Global.Memory == null)
                {
                    return Enums.Detach.AlreadyDetached;
                }

                Global.CsProcess = null;
                Global.Memory    = null;
                return Enums.Detach.Success;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                return Enums.Detach.Exception;
            }
        }

        public bool Exist()
        {
            var csProcess = System.Diagnostics.Process.GetProcessesByName("csgo");
            return csProcess.Length != 0;
        }
    }
}
