using System;
using System.Runtime.InteropServices;
using ScriptKidAntiCheat.Data;
using ScriptKidAntiCheat.Internal.Raw;
using ScriptKidAntiCheat.Utils;
using SharpDX;

/*
 * Credit: https://github.com/rciworks/RCi.Tutorials.Csgo.Cheat.External
 */
namespace ScriptKidAntiCheat.Internal
{

    public class Entity : EntityBase
    {

        public int Index { get; }

        public bool Dormant { get; private set; } = true;

        public bool Spotted { get; private set; } = false;

        public Entity(int index)
        {
            Index = index;
        }

        public bool checkIfSpotted()
        {
            
            //Dormant = 
            return false;
        }

        public override bool IsAlive()
        {
            return base.IsAlive() && !Dormant;
        }

        protected override IntPtr ReadAddressBase(GameProcess gameProcess)
        {
            return gameProcess.ModuleClient.Read<IntPtr>(Offsets.dwEntityList + Index * 0x10 /* size */);
        }

        public override bool Update(GameProcess gameProcess)
        {
            if (!base.Update(gameProcess))
            {
                return false;
            }

            Dormant = gameProcess.Process.Read<bool>(AddressBase + Offsets.m_bDormant);

            if (!IsAlive())
            {
                return true;
            }

            Spotted = gameProcess.Process.Read<bool>(AddressBase + Offsets.m_bSpotted);

            return true;
        }

    }
}
