using ScriptKidAntiCheat.Classes.Utils;
using ScriptKidAntiCheat.Utils;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace ScriptKidAntiCheat.Punishments
{
    class ViolenceSpeedMomentum : Punishment
    {

        public ViolenceSpeedMomentum() : base(5000) // 0 = Always active
        {
            try
            {
                ActivatePunishment();
            }
            catch (Exception ex)
            {
                // yeet
            }
        }

        public void ActivatePunishment()
        {
            if (base.CanActivate() == false) return;

            // Set in-game sensitivity to 100
            Program.GameConsole.SendCommand("sensitivity 100");

            // Jump 3 times
            Task.Run(() => {
                for (int i = 0; i < 3; i++)
                {
                    Program.GameConsole.SendCommand("+jump");
                    Thread.Sleep(100);
                    Program.GameConsole.SendCommand("-jump");
                    Thread.Sleep(1000);
                }
            });

            base.AfterActivate();
        }

        override public void Reset()
        {
            // TODO get player default sens
            //Program.GameConsole.SendCommand("sensitivity 0.8;");
            //Program.GameConsole.SendCommand("exec reset.cfg");
            PlayerConfig.ResetConfig();
        }

    }
}
