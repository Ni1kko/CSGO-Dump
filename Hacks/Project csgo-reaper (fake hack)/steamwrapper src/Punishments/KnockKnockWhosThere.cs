using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Utils;
using System;
using System.Timers;
using System.Threading;
using System.Threading.Tasks;
using static ScriptKidAntiCheat.Utils.MouseHook;
using Gma.System.MouseKeyHook;
using System.Windows.Forms;
using System.Collections.Generic;
using ScriptKidAntiCheat.Classes.Utils;

namespace ScriptKidAntiCheat.Punishments
{
    class KnockKnockWhosThere : Punishment
    {
        public TripWire triggeringTripWire;
        bool simulatedKeyDown = false;
        private IKeyboardMouseEvents m_GlobalHook;

        public KnockKnockWhosThere(TripWire TripWire) : base(0, false, 100) // 0 = Always active
        {
            triggeringTripWire = TripWire;
            Program.GameConsole.SendCommand("bind e \"play physics/flesh/fist_hit_04.wav\"");
            // Keyboard events
            Program.m_GlobalHook.KeyDown += GlobalHookKeyDown;
        }
        private void GlobalHookKeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                if (e.KeyCode == Keys.E)
                {
                    ActivatePunishment();
                }
            }
            catch (Exception ex)
            {
                // yeet
            }
        }

        override public void Tick(Object source, ElapsedEventArgs e)
        {
            try
            {
                if (!triggeringTripWire.IsBeingTripped)
                {
                    this.Dispose();
                }
            }
            catch (Exception ex)
            {
                // yeet
            }

        }
        public void ActivatePunishment()
        {
            if (base.CanActivate() == false) return;

            base.AfterActivate();
        }

        override public void Dispose()
        {
            Program.m_GlobalHook.KeyDown -= GlobalHookKeyDown;
            PlayerConfig.ResetConfig();
            base.Dispose();
        }

    }
}
