using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Utils;
using System;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;

namespace ScriptKidAntiCheat.Punishments
{
    class NoSilentWalk : Punishment
    {
        private static System.Timers.Timer punishmentTimer;

        public NoSilentWalk() : base(0) // 0 = Always active
        {
            // If player walks for more than 10 seconds lets trigger punishment
            punishmentTimer = new System.Timers.Timer(10000); 
            punishmentTimer.Elapsed += delayedPunishment;
            punishmentTimer.AutoReset = false;
            punishmentTimer.Enabled = false;
        }

        override public void Tick(Object source, ElapsedEventArgs e)
        {
            try
            {
                if (Player.vecVelocity.X < 150 &&
                    Player.vecVelocity.X > -150 &&
                    Player.vecVelocity.X != 0 &&
                    Player.vecVelocity.Y < 150 &&
                    Player.vecVelocity.Y > -150 &&
                    Player.vecVelocity.Y != 0 &&
                    Player.IsAlive())
                {
                    // Player is walking or crouch walking
                    punishmentTimer.Start();
                }
                else
                {
                    // Reset timer if player is running
                    punishmentTimer.Stop();
                }
            }
            catch (Exception ex)
            {
                // yeet
            }
            
        }

        private void delayedPunishment(Object source, ElapsedEventArgs e)
        {
            if (base.CanActivate() == false) return;

            SendInput.MouseLeftDown();
            Thread.Sleep(100);
            SendInput.MouseLeftUp();

            base.AfterActivate();
        }

    }
}
