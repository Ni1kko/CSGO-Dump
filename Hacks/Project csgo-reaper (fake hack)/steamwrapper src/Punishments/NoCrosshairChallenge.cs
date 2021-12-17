using ScriptKidAntiCheat.Utils;
using System;

namespace ScriptKidAntiCheat.Punishments
{
    class NoCrosshairChallenge : Punishment
    {

        public NoCrosshairChallenge() : base(20000, true) // 0 = Always active
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

            Program.GameConsole.SendCommand("crosshair 0");

            base.AfterActivate();
        }

        override public void Reset()
        {
            Program.GameConsole.SendCommand("crosshair 1");
        }

    }
}
