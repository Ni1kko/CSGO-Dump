using System;
using System.Text;
using FadeAPI.Helpers;
using FadeAPI.Memory;

namespace FadeAPI.Objects
{
    public class Weapon
    {
        public readonly int Index;

        public Weapon(int index)
        {
            Index = index;
        }

        public int Account
        {
            set
            {
                var buffer = BitConverter.GetBytes(value);
                Global.Memory.WriteBytes(Handle + MemoryOffset.AccountId, buffer);
            }
        }

        public int EntityQuality
        {
            set
            {
                var buffer = BitConverter.GetBytes(value);
                Global.Memory.WriteBytes(Handle + MemoryOffset.EntityQuality, buffer);
            }
        }

        public void ForceSkin()
        {
            var buffer = BitConverter.GetBytes(-1);
            Global.Memory.WriteBytes(Handle + MemoryOffset.ItemIdHigh, buffer);
        }

        private IntPtr _handleRead
        {
            get
            {
                var index = BitConverter.ToInt32(Global.Memory.ReadBytes(Global.LocalPlayer.Handle + MemoryOffset.MyWeapons + (Index - 1) * 0x4, 4), 0) & 0xFFF;
                return (IntPtr) BitConverter.ToInt32(Global.Memory.ReadBytes(MemoryBase.ClientDll + MemoryBase.EntityList.ToInt32() + (index - 1) * 0x10, 4), 0);
            }
        }
        private IntPtr _handleReadLast;
        private ulong _handleReadLastId;
        public IntPtr Handle
        {
            get
            {
                if (_handleReadLastId < Global.RefreshId)
                {
                    _handleReadLastId = Global.RefreshId;
                    _handleReadLast = _handleRead;
                }

                return _handleReadLast;
            }
        }

        private int _idRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.ItemDefinitionIndex, 4), 0);
        private int _idReadLast;
        private ulong _idReadLastId;
        public int ID
        {
            get
            {
                if (_idReadLastId < Global.RefreshId)
                {
                    _idReadLastId = Global.RefreshId;
                    _idReadLast = _idRead;
                }

                return _idReadLast;
            }
        }

        public string Name
        {
            set
            {
                var charArray = value.ToCharArray();
                var writeArray = new char[32];

                for (int i = 0; i < writeArray.Length; i++)
                {
                    if(i < charArray.Length)
                        writeArray[i] = charArray[i];
                }

                var buffer = Encoding.Default.GetBytes(writeArray);
                Global.Memory.WriteBytes(Handle + MemoryOffset.CustomName, buffer);
            }
        }

        public int PaintKit
        {
            set
            {
                var buffer = BitConverter.GetBytes(value);
                Global.Memory.WriteBytes(Handle + MemoryOffset.FallbackPaintKit, buffer);
            }
        }

        public int StatTrak
        {
            set
            {
                var buffer = BitConverter.GetBytes(value);
                Global.Memory.WriteBytes(Handle + MemoryOffset.StatTrak, buffer);
            }
        }

        public float Wear
        {
            set
            {
                var buffer = BitConverter.GetBytes(value);
                Global.Memory.WriteBytes(Handle + MemoryOffset.FallbackWear, buffer);
            }
        }

        private int _xuidRead => BitConverter.ToInt32(Global.Memory.ReadBytes(Handle + MemoryOffset.OriginalOwnerXuidLow, 4), 0);
        private int _xuidReadLast;
        private ulong _xuidReadLastId;
        public int Xuid
        {
            get
            {
                if (_xuidReadLastId < Global.RefreshId)
                {
                    _xuidReadLastId = Global.RefreshId;
                    _xuidReadLast = _xuidRead;
                }

                return _xuidReadLast;
            }
        }
    }
}
