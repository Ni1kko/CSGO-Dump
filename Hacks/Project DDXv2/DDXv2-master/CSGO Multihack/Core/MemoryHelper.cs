using System;
using System.Diagnostics;
using System.Globalization;

namespace DDX.Core
{
    public class SignatureScanner
    {
        private class Scanner
        {
            private readonly ProcessModule _module;
            private readonly Memory _mem;
            
            private byte[] _dump;

            public Scanner(ProcessModule module, Memory memory)
            {
                _module = module;
                _mem = memory;
            }

            public IntPtr Scan(string strPattern, int nOffset = 0)
            {
                var mask = string.Empty;
                var patternBlocks = strPattern.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

                var pattern = new byte[patternBlocks.Length];

                for (var i = 0; i < patternBlocks.Length; i++)
                {
                    var block = patternBlocks[i];
                    if (block == "?")
                    {
                        mask += '?';
                        pattern[i] = 0;
                    }
                    else
                    {
                        mask += 'x';

                        if (!byte.TryParse(patternBlocks[i], NumberStyles.HexNumber, CultureInfo.DefaultThreadCurrentCulture, out pattern[i]))
                        {
                            return IntPtr.Zero;
                        }
                    }
                }

                return Scan(pattern, mask, nOffset);
            }

            public IntPtr Scan(byte[] signature, string mask, int offset = 0x0)
            {
                if (_dump == null)
                {
                    DumpMemory();
                }

                // Scan every byte
                for (var i = 0; i < _dump.Length; i++)
                {
                    // Found
                    if (CheckSignature(i, signature, mask))
                    {
                        return _module.BaseAddress + i + offset;
                    }
                }

                return IntPtr.Zero;
            }

            private void DumpMemory()
            {
                _dump = _mem.Read(_module.BaseAddress, _module.ModuleMemorySize);
            }

            private bool CheckSignature(int now, byte[] signature, string mask)
            {
                for (var i = 0; i < signature.Length; i++)
                {
                    if (mask[i] == '?')
                    {
                        continue;
                    }

                    if (signature[i] != _dump[now + i])
                    {
                        return false;
                    }
                }

                return true;
            }
        }

        public void Scan()
        {
            // Client
            var clientScan            = new Scanner(Module.Client, G.Mem);
            var attack1               = clientScan.Scan("C1 83 CA 01", -0x7);
            var attack2               = clientScan.Scan("C1 81 CA 00 08", -0x7);
            var backward              = clientScan.Scan("C1 83 CA 10", -0x7);
            var bombPlanted           = clientScan.Scan("7E 26 8B 0D", -0x5);
            var duck                  = clientScan.Scan("C1 83 CE 04", -0x7);
            var entityList            = clientScan.Scan("BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 0x1);
            var forward               = clientScan.Scan("C1 83 CE 08", -0x7);
            var globalVars            = clientScan.Scan("A1 ? ? ? ? 68 00 00 04 00", 0x1);
            var glowObject            = clientScan.Scan("A1 ? ? ? ? A8 01 75 4B", 0x1);
            var input                 = clientScan.Scan("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10", 0x1);
            var jump                  = clientScan.Scan("89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 08", 0x2);
            var left                  = clientScan.Scan("C1 81 CA 00 02", -0x7);
            var localPlayer           = clientScan.Scan("8D 34 85 ? ? ? ? 89 15", 0x3);
            var mouseEnabled          = clientScan.Scan("B9 ? ? ? ? FF 50 34 85 C0 75 10", 0x1);
            var noshake               = clientScan.Scan("8B CF FF 90 ? ? ? ? 8B 0D ? ? ? ? 81 F9", 0x1A);
            var playerResource        = clientScan.Scan("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7", 0x2);
            var radarBase             = clientScan.Scan("A1 ? ? ? ? 8B 0C B0 8B 01 FF 50 ? 46 3B 35 ? ? ? ? 7C EA 8B 0D", 0x1);
            var right                 = clientScan.Scan("C1 81 CE 00 04", -0x7);
            var viewMatrix            = clientScan.Scan("0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9", 0x3);

            // Engine
            var engineScan            = new Scanner(Module.Engine, G.Mem);
            var clientCmd             = engineScan.Scan("55 8B EC 8B ? ? ? ? ? 81 F9 ? ? ? ? 75 0C A1 ? ? ? ? 35 ? ? ? ? EB 05 8B 01 FF 50 34 50 A1");
            var clientState           = engineScan.Scan("A1 ? ? ? ? 33 D2 6A ? 6A ? 33 C9 89 B0 08 4E ? ? A1", 0x1);
            var entityListLength      = engineScan.Scan("89 01 89 0D ? ? ? ? 66", -0x4);
            var sendPackets           = engineScan.Scan("B3 ? 8B 01 8B", 0x1);

            Base.m_bSendPackets       = sendPackets;
            Base.m_dwAttack1          = G.Mem.Read<IntPtr>(attack1);
            Base.m_dwAttack2          = G.Mem.Read<IntPtr>(attack2);
            Base.m_dwBackward         = G.Mem.Read<IntPtr>(backward);
            Base.m_dwBombPlanted      = G.Mem.Read<IntPtr>(bombPlanted);
            Base.m_dwClientCmd        = clientCmd;
            Base.m_dwClientState      = G.Mem.Read<IntPtr>(clientState);
            Base.m_dwDuck             = G.Mem.Read<IntPtr>(duck);
            Base.m_dwEntityList       = G.Mem.Read<IntPtr>(entityList);
            Base.m_dwEntityListLength = G.Mem.Read<IntPtr>(entityListLength);
            Base.m_dwForward          = G.Mem.Read<IntPtr>(forward);
            Base.m_dwGlobalVars       = G.Mem.Read<IntPtr>(globalVars);
            Base.m_dwGlowObject       = G.Mem.Read<IntPtr>(glowObject) + 0x4;
            Base.m_dwInput            = G.Mem.Read<IntPtr>(input);
            Base.m_dwJump             = G.Mem.Read<IntPtr>(jump);
            Base.m_dwLeft             = G.Mem.Read<IntPtr>(left);
            Base.m_dwLocalPlayer      = G.Mem.Read<IntPtr>(localPlayer) + 0x4;
            Base.m_dwMouseEnable      = G.Mem.Read<IntPtr>(mouseEnabled);
            Base.m_dwNoShake          = G.Mem.Read<IntPtr>(noshake);
            Base.m_dwPlayerResource   = G.Mem.Read<IntPtr>(playerResource);
            Base.m_dwRadarBase        = G.Mem.Read<IntPtr>(radarBase);
            Base.m_dwRight            = G.Mem.Read<IntPtr>(right);
            Base.m_dwViewMatrix       = G.Mem.Read<IntPtr>(viewMatrix) + 0xB0;
        }
    }
}
