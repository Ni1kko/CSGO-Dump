namespace GameInfoPlus.Classes
{ 
    public class Engine
    {
        public int EnginePtr;

        public Engine(int Base)
        {
            EnginePtr = Base;
        }

        public Entity LocalPlayer
        {
            get
            {
                return new Entity(Memory.ReadMemory<int>((int)Memory.Client +  Memory.offsets.signatures.dwLocalPlayer));
            }
        }
   
        public int ClientState
        {
            get
            {
                return Memory.ReadMemory<int>(EnginePtr +  Memory.offsets.signatures.dwClientState);
            }
        } 

        public void Jump()
        {
            Memory.WriteMemory<int>((int)Memory.Client + Memory.offsets.signatures.dwForceJump, 6);
        }

    }
}
