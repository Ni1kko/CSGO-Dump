using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Utils;
using System;

namespace ScriptKidAntiCheat.Punishments
{
    class ButterFingers : Punishment
    {

        public ButterFingers() : base(20000, true) // 0 = Always active
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

            Program.GameConsole.SendCommand("bind mouse1 drop");

            base.AfterActivate();
        }

        override public void Reset()
        {
            Program.GameConsole.SendCommand("bind mouse1 +attack");
        }

    }
}
