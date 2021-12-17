using System;
using System.Linq;
using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Internal;
using ScriptKidAntiCheat.Utils;

namespace ScriptKidAntiCheat.Data
{
    public class GameData : U_ThreadedComponent
    {
        protected override string ThreadName => nameof(GameData);

        private GameProcess GameProcess { get; set; }

        public Player Player { get; set; }

        public MatchInfo MatchInfo { get; set; }

        public int ClientState { get; private set; } = 0;

        public Entity[] Entities { get; private set; }

        public int PlayerID { get; private set; }

        public GameData(GameProcess gameProcess)
        {
            GameProcess = gameProcess;
            Player = new Player();
            MatchInfo = new MatchInfo();
            Entities = Enumerable.Range(0, 64).Select(index => new Entity(index)).ToArray();
        }

        public override void Dispose()
        {
            base.Dispose();
            MatchInfo = default;
            Entities = default;
            Player = default;
            GameProcess = default;
        }

        protected override void FrameAction()
        {
            if (!GameProcess.IsValidAndActiveWindow)
            {
                return;
            }

            IntPtr dwClientState = GameProcess.ModuleEngine.Read<IntPtr>(Offsets.dwClientState);

            if (dwClientState != IntPtr.Zero)
            {
                ClientState = GameProcess.Process.Read<int>(dwClientState + Offsets.dwClientState_State);
            }

            // Update match data
            MatchInfo.Update(GameProcess);

            // Update player data
            Player.Update(GameProcess);

            foreach (var entity in Entities)
            {
                // Update game entities data
                entity.Update(GameProcess);
            }
        }
    }
}
