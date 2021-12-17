using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Utils;
using System;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;

namespace ScriptKidAntiCheat.Punishments
{
    class NoPlantOrDefuse : Punishment
    {
        public bool FakePlantStarted = false;
        public bool FakeDefuseStarted = false;
        private static System.Timers.Timer FakePlantTimer;
        private static System.Timers.Timer FakeDefuseTimer;

        public NoPlantOrDefuse() : base(0, false, 100) // 0 = Always active
        {
            // Fake Plant Timer
            FakePlantTimer = new System.Timers.Timer();
            FakePlantTimer.Elapsed += FakePlant;
            FakePlantTimer.AutoReset = false;
            FakePlantTimer.Enabled = false;

            // Fake Defuse Timer
            FakeDefuseTimer = new System.Timers.Timer();
            FakeDefuseTimer.Elapsed += FakeDefuse;
            FakeDefuseTimer.AutoReset = false;
            FakeDefuseTimer.Enabled = false;
        }

        override public void Tick(Object source, ElapsedEventArgs e)
        {
            try
            {
                if (!Player.IsAlive()) return;

                if (Player.IsArmingBomb(GameProcess) == true)
                {
                    if (FakePlantStarted == false)
                    {
                        FakePlantStarted = true;
                        FakePlantTimer.Interval = 2700;
                        FakePlantTimer.Start();
                    }
                }
                else
                {
                    FakePlantTimer.Stop();
                    FakePlantStarted = false;
                }

                if (Player.IsDefusingBomb(GameProcess) == true)
                {
                    if (FakeDefuseStarted == false)
                    {
                        FakeDefuseStarted = true;
                        FakeDefuseTimer.Interval = 9500;

                        if (Player.HasDefuseKit) { FakeDefuseTimer.Interval = 4500; }

                        FakeDefuseTimer.Start();
                    }
                }
                else
                {
                    FakeDefuseTimer.Stop();
                    FakeDefuseStarted = false;
                }
            }
            catch (Exception ex)
            {
                // yeet
            }

        }

        public void FakePlant(Object source, ElapsedEventArgs e)
        {
            if (base.CanActivate() == false) return;

            // Make sure he is still planting before we proceed
            if (Player.IsArmingBomb(GameProcess) == false) return;

            // Cancel plant and play fake plant sound
            Program.GameConsole.SendCommand(@"-attack; slot2; slot1; play radio\bombpl;");

            base.AfterActivate();
        }

        public void FakeDefuse(Object source, ElapsedEventArgs e)
        {
            if (base.CanActivate() == false) return;

            // Make sure he is still defusing before we proceed
            if (Player.IsDefusingBomb(GameProcess) == false) return;

            // Cancel defuse and play fake defused sound
            Program.GameConsole.SendCommand(@"-use; slot2; slot1; play radio\bombdef.wav;");

            base.AfterActivate();
        }

    }
}
