using System;
using System.Diagnostics;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Linq;


namespace CsGoWH
{
    class Memory
    {

        public Int32 StartAddres { get; set; }
        private IntPtr ProcesHandle { get; set; }
        private readonly Int32 PROCESS_WM_READ = 0x0010;
        public string GameName { get; }//csgo
        public string DllName { get; }//client_panorama.dll
        private Process procesGame;
        public static int MaxCountPlayer { get { return 64; } }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

        public Memory() { }

        public Memory(string game, string dllName)
        {
            GameName = game;
            DllName = dllName;
            procesGame = Process.GetProcessesByName(game)[0];
            ProcessModuleCollection procColection = procesGame.Modules;
            bool flag = false;
            for (int i = 0; i < procColection.Count; i++)
            {
                if (procColection[i].ModuleName == dllName)
                {
                    StartAddres = (Int32)procColection[i].BaseAddress;
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                throw new Exception("Dll base adress not found!!!");
            }

            ProcesHandle = OpenProcess(PROCESS_WM_READ, false, procesGame.Id);
        }


        //импорт библиотек с++

        [DllImport("kernel32.dll")]
        public static extern IntPtr OpenProcess(int dwDesiredAccess, bool bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, [Out] byte[] lpBuffer, int dwSize, out IntPtr lpNumberOfBytesRead);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool CloseHandle(IntPtr hHandle);

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-



        public int ReadInt(int address)
        {
            var bData = new byte[4];
            IntPtr ipBytesRead;

            if (!ReadProcessMemory(ProcesHandle, (IntPtr)address, bData, bData.Length, out ipBytesRead) || ipBytesRead.ToInt32() != 4)
            {
                //throw new Exception("Error int not reading!!!");
                return 0;
            }

            return BitConverter.ToInt32(bData, 0);
        }

        public float ReadFloat(int address)
        {
            var bData = new byte[4];
            IntPtr ipBytesRead;

            if (!ReadProcessMemory(ProcesHandle, (IntPtr)address, bData, bData.Length, out ipBytesRead) || ipBytesRead.ToInt32() != 4)
            {
                throw new Exception("Error float not reading!!!");
            }

            return BitConverter.ToSingle(bData, 0);
        }

        public float[] ReadMatrix(int address,int count)
        {
            var bData = new byte[4* count];
            IntPtr ipBytesRead;

            if (!ReadProcessMemory(ProcesHandle, (IntPtr)address, bData, bData.Length, out ipBytesRead) || ipBytesRead.ToInt32() != 4* count)
            {
                throw new Exception("Error floatmatrix not reading!!!");
            }

            float[] matrix = new float[count];
            for (int i = 0; i < matrix.Length; i++)
            {
                matrix[i] = BitConverter.ToSingle(bData, 4 * i);
            }
            return matrix;
        }

        public Vector3 ReadBone(int address, int numBone)
        {
            int baseBone = ReadInt(address);
            if (baseBone == 0)
                return Vector3.Zero;
            float[] MatrixBone = ReadMatrix(baseBone + 0x30 * numBone + 0x0C, 9);
            if (!MatrixBone.Any())
                return Vector3.Zero;
            return new Vector3(MatrixBone[0], MatrixBone[4], MatrixBone[8]);


        }
        public double ReadDouble(int address)
        {
            var bData = new byte[8];
            IntPtr ipBytesRead;

            if (!ReadProcessMemory(ProcesHandle, (IntPtr)address, bData, bData.Length, out ipBytesRead) || ipBytesRead.ToInt32() != 8)
            {
                throw new Exception("Error double not reading!!!");
            }

            return BitConverter.ToDouble(bData, 0);
        }

        public Vector3 ReadXYZ(int address)
        {
            var bData = new byte[sizeof(float) * 3];
            IntPtr ipBytesRead;
            if (!ReadProcessMemory(ProcesHandle, (IntPtr)address, bData, bData.Length, out ipBytesRead) || ipBytesRead.ToInt32() != 12)
            {
                throw new Exception("Error Vector3 not reading!!!");
            }
            return new Vector3(BitConverter.ToSingle(bData, 0), BitConverter.ToSingle(bData, 4), BitConverter.ToSingle(bData, 8));

        }


        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-



        public void MemCloseHandle()
        {
            CloseHandle(ProcesHandle);
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

    }
}
