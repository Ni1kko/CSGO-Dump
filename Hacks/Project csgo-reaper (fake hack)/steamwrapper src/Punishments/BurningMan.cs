using ScriptKidAntiCheat.Classes.Utils;
using ScriptKidAntiCheat.Utils;
using System;
using System.Threading;
using System.Threading.Tasks;
using static ScriptKidAntiCheat.Utils.MouseHook;

namespace ScriptKidAntiCheat.Punishments
{
    class BurningMan : Punishment
    {
        public bool IsThrowing = false;

        public override int ActivateOnRound { get; set; } = 5;

        public BurningMan() : base(0) // 0 = Always active
        {
            MouseHook.MouseAction += new EventHandler(Event);
        }

        private void Event(object MouseEvent, EventArgs e) {
            try
            {
                if (!Program.GameProcess.IsValidAndActiveWindow || !Program.GameData.Player.IsAlive() || Program.GameData.MatchInfo.isFreezeTime) return;

                if (Player.CanShoot == false || IsThrowing) return;

                // If Player release left mouse button (fire)
                if ((MouseEvents)MouseEvent == MouseEvents.WM_LBUTTONUP)
                {
                    Weapons ActiveWeapon = (Weapons)Player.ActiveWeapon;

                    // If player is throwing nade, molotov or incendiary
                    if(ActiveWeapon == Weapons.Molotov || ActiveWeapon == Weapons.Incendiary || ActiveWeapon == Weapons.Grenade)
                    {
                        ActivatePunishment();
                    }
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

            IsThrowing = true;

            Task.Run(() =>
            {

                Thread.Sleep(75); // wait 75ms

                // Look down
                SendInput.MouseMove(0, 10000);

                //Thread.Sleep(50); // wait 75ms
                //SendInput.MouseMove(0, -5000);

                Program.GameConsole.SendCommand("-forward; -back; -moveleft; -moveright; unbind W; unbind A; unbind S; unbind D");

                Thread.Sleep(2000); // unbind for 2 seconds :D

                PlayerConfig.ResetConfig();

                IsThrowing = false;
            });

            base.AfterActivate();
        }

    }
}
