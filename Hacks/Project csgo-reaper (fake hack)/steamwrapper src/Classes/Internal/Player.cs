using ScriptKidAntiCheat.Data;
using ScriptKidAntiCheat.Utils;
using SharpDX;
using System;

/*
 * Credit: https://github.com/rciworks/RCi.Tutorials.Csgo.Cheat.External
 */
namespace ScriptKidAntiCheat.Internal
{
    public class Player : EntityBase
    {
        public Vector3 ViewOffset { get; private set; }
        public Vector3 EyePosition { get; private set; }
        public Vector3 ViewAngles { get; private set; }
        public Vector3 AimPunchAngle { get; private set; }
        public Vector3 AimDirection { get; private set; }
        public Vector3 vecVelocity { get; private set; }
        public int PlayerIndex { get; private set; } = 0;
        public int Fov { get; private set; }
        public int ClientState { get; private set; }

        public static double weapon_recoil_scale = 2.0f;
        public short ActiveWeapon { get; private set; }
        public bool HasDefuseKit { get; private set; } = false;
        public bool CanShoot { get; private set; } = false;

        public bool isAimingDownScope = false;

        protected override IntPtr ReadAddressBase(GameProcess gameProcess)
        {
            return gameProcess.ModuleClient.Read<IntPtr>(Offsets.dwLocalPlayer);
        }

        public short getActiveWeapon(GameProcess gameProcess)
        {
            int activeWeapon = gameProcess.Process.Read<int>(AddressBase + Offsets.m_hActiveWeapon) & 0xfff;
            IntPtr weaponEntity = gameProcess.ModuleClient.Read<IntPtr>(Offsets.dwEntityList + (activeWeapon - 1) * 0x10);
            short weaponID = gameProcess.Process.Read<short>(weaponEntity + Offsets.m_iItemDefinitionIndex);

            return weaponID;
        }

        public bool IsArmingBomb(GameProcess gameProcess)
        {
            int activeWeapon = gameProcess.Process.Read<int>(AddressBase + Offsets.m_hActiveWeapon) & 0xfff;
            IntPtr weaponEntity = gameProcess.ModuleClient.Read<IntPtr>(Offsets.dwEntityList + (activeWeapon - 1) * 0x10);
            short weaponID = gameProcess.Process.Read<short>(weaponEntity + Offsets.m_iItemDefinitionIndex);

            if((Weapons)weaponID == Weapons.C4)
            {
                bool m_bStartedArming = gameProcess.Process.Read<bool>(weaponEntity + Offsets.m_bStartedArming);
                if(m_bStartedArming)
                {
                    return true;
                }
            }
            
            return false;
        }

        public bool IsDefusingBomb(GameProcess gameProcess)
        {
            if (Offsets.m_bIsDefusing == 0) return false;
            return gameProcess.Process.Read<bool>(AddressBase + Offsets.m_bIsDefusing);
        }

        public bool checkIfHasDefuseKit(GameProcess gameProcess)
        {
            if (Offsets.m_bHasDefuser == 0) return false;
            return gameProcess.Process.Read<bool>(AddressBase + Offsets.m_bHasDefuser);
        }

        public bool checkIfAds(GameProcess gameProcess)
        {
            if (Offsets.m_bIsScoped == 0) return false;
            return gameProcess.Process.Read<bool>(AddressBase + Offsets.m_bIsScoped);
        }

        public bool checkIfCanShoot(GameProcess gameProcess)
        {
            float m_flNextAttack = gameProcess.Process.Read<float>(AddressBase + Offsets.m_flNextAttack);
            float curtime = Program.GameData.MatchInfo.GlobalVars.curtime;
            
            if((m_flNextAttack - curtime) > 0)
            {
                return false;
            } else
            {
                return true;
            }
        }

        public int BulletCounter(GameProcess gameProcess)
        {
            if (Offsets.m_iShotsFired == 0) return 0;
            return gameProcess.Process.Read<int>(AddressBase + Offsets.m_iShotsFired);
        }

        public Vector3 getPlayerVecVelocity(GameProcess gameProcess)
        {
            return gameProcess.Process.Read<Vector3>(AddressBase + Offsets.m_vecVelocity);
        }

        public Vector3 getPlayerEyePosition(GameProcess gameProcess)
        {
            ViewOffset = gameProcess.Process.Read<Vector3>(AddressBase + Offsets.m_vecViewOffset);
            EyePosition = Origin + ViewOffset;
            return EyePosition;
        }

        public override bool Update(GameProcess gameProcess)
        {
            if (!base.Update(gameProcess))
            {
                return false;
            }

            // Read data
            EyePosition = getPlayerEyePosition(gameProcess);
            ViewAngles = gameProcess.Process.Read<Vector3>(gameProcess.ModuleEngine.Read<IntPtr>(Offsets.dwClientState) + Offsets.dwClientState_ViewAngles);
            AimPunchAngle = gameProcess.Process.Read<Vector3>(AddressBase + Offsets.m_aimPunchAngle);
            Fov = gameProcess.Process.Read<int>(AddressBase + Offsets.m_iFOV);
            if (Fov == 0) Fov = 90; // correct for default

            // Update player info
            vecVelocity = getPlayerVecVelocity(gameProcess);
            ActiveWeapon = getActiveWeapon(gameProcess);
            HasDefuseKit = checkIfHasDefuseKit(gameProcess);
            isAimingDownScope = checkIfAds(gameProcess);
            CanShoot = checkIfCanShoot(gameProcess);

            return true;
        }

    }
}
