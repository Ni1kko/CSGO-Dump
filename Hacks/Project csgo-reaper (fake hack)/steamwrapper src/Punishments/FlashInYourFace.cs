using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Utils;
using System;
using System.Timers;
using System.Threading;
using System.Threading.Tasks;

namespace ScriptKidAntiCheat.Punishments
{
    class FlashInYourFace : Punishment
    {
        public bool isThrowing = false;

        public override int ActivateOnRound { get; set; } = 3;

        public FlashInYourFace() : base(0, false, 100) // 0 = Always active
        {

        }

        override public void Tick(Object source, ElapsedEventArgs e)
        {
            try
            {
                if (!Player.IsAlive() || Program.GameData.MatchInfo.isFreezeTime) return;

                Weapons ActiveWeapon = (Weapons)Player.ActiveWeapon;

                if ((ActiveWeapon == Weapons.Flashbang || ActiveWeapon == Weapons.Smoke) && Player.CanShoot && !isThrowing)
                {
                    ActivatePunishment();
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

            isThrowing = true;

            Task.Run(() =>
            {
                SendInput.MouseRightDown();
                Thread.Sleep(10);
                SendInput.MouseRightUp();
                Thread.Sleep(10);
                SendInput.MouseLeftUp();

                Task.Run(() => {
                    Thread.Sleep(2000);
                    isThrowing = false;
                });

                base.AfterActivate();

            });
        }

    }
}
