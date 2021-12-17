 namespace GameInfoPlus.Classes
{
    public class Entity
    {
        #region EntityBase 
        public int EntityBase;
      
        public Entity(int Base)
        {
            this.EntityBase = Base;
        }

        public static int GetEntityBase(int PlayerLoopID)
        {
            return Memory.ReadMemory<int>((int)Memory.Client +  Memory.offsets.signatures.dwEntityList + (PlayerLoopID * 0x10));
        }
        #endregion

        public int Flags
        {
            get
            {
                return Memory.ReadMemory<int>(EntityBase +  Memory.offsets.netvars.m_fFlags);
            }
        }

        public int GetPlayerInformationStructure()
        {
            int CurrentClientState = Memory.ReadMemory<int>((int)Memory.Client + Memory.offsets.signatures.dwClientState); 
            int CurrentUserInformationPointer = Memory.ReadMemory<int>(CurrentClientState + Memory.offsets.signatures.dwClientState_PlayerInfo);

            int One = Memory.ReadMemory<int>(CurrentUserInformationPointer + 0x40);
            int Two = Memory.ReadMemory<int>(One + 0xC);
            return Memory.ReadMemory<int>(Two + 0x28 + (EntityBase * 0x34));
        }


        public string GetSteamID()
        {
            return Memory.ReadMemoryString(GetPlayerInformationStructure() + 148, 20, false);
        }

        public string GetName()
        {
            return Memory.ReadMemoryString(GetPlayerInformationStructure() + 0x10, 128, false);
        }
        public int Team
        {
            get
            {
                return Memory.ReadMemory<int>(EntityBase + Memory.offsets.netvars.m_iTeamNum);
            }
        }

        public int Health
        {
            get
            {
                return Memory.ReadMemory<int>(EntityBase + Memory.offsets.netvars.m_iHealth);
            }
        }

        public bool Dormant
        {
            get
            {
                return Memory.ReadMemory<bool>(EntityBase +  Memory.offsets.signatures.m_bDormant);
            }
        }
        
        public bool Scoped
        {
            get
            {
                return Memory.ReadMemory<bool>(EntityBase + Memory.offsets.netvars.m_bIsScoped);
            }
        }

        public int FlashDuration
        {
            get
            {
                return Memory.ReadMemory<int>(EntityBase +  Memory.offsets.netvars.m_flFlashDuration);
            }
            set
            {
                Memory.WriteMemory<int>(EntityBase +  Memory.offsets.netvars.m_flFlashDuration, value);
            }
        } 

        public string GetTeamName()
        {
            var TeamNums = new string[] { "Connecting", "Spectator", "Terrorist", "Counter-Terrorist" };

            if (Team >= 0 && Team < TeamNums.Length)
            {
                return TeamNums[Team];
            }

            return "Undefined";
        } 
        
        public bool IsLocalPlayer
        {
            get
            {
                return Game.Engine.LocalPlayer.EntityBase == EntityBase;
            }
        }

        public bool IsTeammate
        {
            get
            {
                return Team == Game.Engine.LocalPlayer.Team;
            }
        }

        public bool Alive
        {
            get
            {
                if (Health > 0 && Health <= 100)
                    return true;
                return false;
            }
        }

        public bool Dead
        {
            get
            {
                if (!Alive)
                    return true;
                return false;
            }
        }

        public bool IsPlayer
        {
            get
            {
                if (Team == 2 || Team == 3)
                    return true;
                else if (Team == 0 || Team == 1)
                    return false;
                else
                    return false;
            }
        }

        public bool Valid
        {
            get
            {
                if (Dormant)
                    return false;
                if (Dead)
                    return false;
                if (!IsPlayer)
                    return false;
                return true;
            }
        }

    }
}
