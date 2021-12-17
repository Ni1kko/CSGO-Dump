using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Utils;
using System;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;

namespace ScriptKidAntiCheat.Punishments
{
    class BigSpender : Punishment
    {
        public bool isBuying = false;

        public override int ActivateOnRound { get; set; } = 3;

        public BigSpender() : base(0, false, 500) // 0 = Always active
        {
         
        }

        override public void Tick(Object source, ElapsedEventArgs e)
        {
            try
            {
                if (Player.IsAlive() && GameData.MatchInfo.isFreezeTime == true && isBuying == false)
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

            isBuying = true;

            Task.Run(() => {
                Thread.Sleep(500);
                Program.GameConsole.SendCommand("buy elite; drop; buy elite; drop;");
                Thread.Sleep(500);
                Program.GameConsole.SendCommand("buy nova; drop; buy nova; drop;");
                Thread.Sleep(500);
                Program.GameConsole.SendCommand("buy mac10; drop; buy mac10; drop;");
            });

            base.AfterActivate();

            this.Dispose();

        }

    }
}
