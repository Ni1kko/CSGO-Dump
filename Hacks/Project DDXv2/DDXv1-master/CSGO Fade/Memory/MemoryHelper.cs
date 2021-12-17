using FadeAPI.Enums;
using FadeAPI.Helpers;
using System;
using System.Diagnostics;
using FadeAPI.Structs;

namespace FadeAPI.Memory
{
    internal static class MemoryHelper
    {
        public static SignatureScan SignatureScan()
        {
            try
            {
                var csProcess = System.Diagnostics.Process.GetProcessesByName("csgo");

                //[swap]
                //[block]
                if (csProcess.Length == 0)
                {
                    return Enums.SignatureScan.ProcessNotFound;
                }
                //[/block]
                //[block]
                if (csProcess.Length > 1)
                {
                    return Enums.SignatureScan.MultipleProcesses;
                }
                //[/block]
                //[/swap]

                //[swap]
                var engineSize = 0;
                var clientSize = 0;
                var clientBase = IntPtr.Zero;
                var engineBase = IntPtr.Zero;
                //[/swap]

                foreach (ProcessModule module in csProcess[0].Modules)
                {
                    switch (module.ModuleName)
                    {
                        //[swap]
                        //[block]
                        case "client.dll":
                            clientBase = module.BaseAddress;
                            clientSize = module.ModuleMemorySize;
                            MemoryBase.ClientDll = clientBase;
                            break;
                        //[/block]
                        //[block]
                        case "engine.dll":
                            engineBase = module.BaseAddress;
                            engineSize = module.ModuleMemorySize;
                            MemoryBase.EngineDll = engineBase;
                            break;
                        //[/block]
                        //[/swap]
                    }
                }

                if (clientBase == IntPtr.Zero || engineBase == IntPtr.Zero || clientSize == 0 || engineSize == 0)
                    return Enums.SignatureScan.ModuleNotFound;
                

                // Client
                SignatureScanner.SignatureList.Add(new Signature("89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04", 0x2)); // attack1
                SignatureScanner.SignatureList.Add(new Signature("23 C8 89 ? ? ? ? ? 8B ? ? ? ? ? 8B F2 8B C1 81 CE 00 20", 0x4)); // attack2
                SignatureScanner.SignatureList.Add(new Signature("BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 0x1)); // entityList
                SignatureScanner.SignatureList.Add(new Signature("A1 ? ? ? ? A8 01 75 4B", 0x1)); // glowObject
                SignatureScanner.SignatureList.Add(new Signature("89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 08", 0x2)); // jump
                SignatureScanner.SignatureList.Add(new Signature("A3 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 59 C3 6A", 0x1)); // localPlayer
                SignatureScanner.SignatureList.Add(new Signature("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7", 0x2)); // playerResources
                SignatureScanner.SignatureList.Add(new Signature("A1 ? ? ? ? 8B 0C B0 8B 01 FF 50 ? 46 3B 35 ? ? ? ? 7C EA 8B 0D", 0x1)); // radar
                SignatureScanner.SignatureList.Add(new Signature("0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9", 0x3)); // viewMatrix
                SignatureScanner.Scan(clientBase, clientSize);

                //[swap]
                var attack1Address        = SignatureScanner.SignatureList[0].Address;
                var jumpAddress           = SignatureScanner.SignatureList[4].Address;
                var attack2Address        = SignatureScanner.SignatureList[1].Address;
                var localPlayerAddress    = SignatureScanner.SignatureList[5].Address;
                var playerResourceAddress = SignatureScanner.SignatureList[6].Address;
                var glowObjectAddress     = SignatureScanner.SignatureList[3].Address;
                var radarAddress          = SignatureScanner.SignatureList[7].Address;
                var viewMatrixAddress     = SignatureScanner.SignatureList[8].Address;
                var entityListAddress     = SignatureScanner.SignatureList[2].Address;
                //[/swap]
                SignatureScanner.SignatureList.Clear();

                // Engine
                SignatureScanner.SignatureList.Add(new Signature("A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0", 0x1)); // clientState
                SignatureScanner.SignatureList.Add(new Signature("B3 01 8B 01 8B 40 10 FF D0 84 C0 74 0F 80 BF ? ? ? ? ? 0F 84", 0x1)); // sendPackets
                SignatureScanner.SignatureList.Add(new Signature("89 01 89 0D ? ? ? ? 66", -0x4)); // entityListLength
                SignatureScanner.Scan(engineBase, engineSize);

                //[swap]
                var sendPacketsAddress      = SignatureScanner.SignatureList[1].Address;
                var clientStateAddress      = SignatureScanner.SignatureList[0].Address;
                var entityListLengthAddress = SignatureScanner.SignatureList[2].Address;
                //[/swap]
                SignatureScanner.SignatureList.Clear();

                //[swap]
                MemoryBase.SendPackets      = sendPacketsAddress - engineBase.ToInt32();
                MemoryBase.Attack1          = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(attack1Address, 4), 0) - clientBase.ToInt32();
                MemoryBase.LocalPlayer      = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(localPlayerAddress, 4), 0) + 16 - clientBase.ToInt32();
                MemoryBase.Attack2          = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(attack2Address, 4), 0) - clientBase.ToInt32();
                MemoryBase.EntityListLength = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(entityListLengthAddress, 4), 0) - engineBase.ToInt32();
                MemoryBase.GlowObject       = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(glowObjectAddress, 4), 0) + 4 - clientBase.ToInt32();
                MemoryBase.Radar            = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(radarAddress, 4), 0) - clientBase.ToInt32();
                MemoryBase.EntityList       = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(entityListAddress, 4), 0) - clientBase.ToInt32();
                MemoryBase.ViewMatrix       = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(viewMatrixAddress, 4), 0) + 176 - clientBase.ToInt32();
                MemoryBase.Jump             = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(jumpAddress, 4), 0) - clientBase.ToInt32();
                MemoryBase.PlayerResource   = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(playerResourceAddress, 4), 0) - clientBase.ToInt32();
                MemoryBase.ClientState      = (IntPtr)BitConverter.ToInt32(Global.Memory.ReadBytes(clientStateAddress, 4), 0) - engineBase.ToInt32();
                //[/swap]

                return Enums.SignatureScan.Success;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
                return Enums.SignatureScan.Exception;
            }
        }
    }
}
