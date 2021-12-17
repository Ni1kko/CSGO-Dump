using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Data;
using ScriptKidAntiCheat.Internal;
using ScriptKidAntiCheat.Utils;
using ScriptKidAntiCheat.Win32;
using System;
using System.IO;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;

namespace ScriptKidAntiCheat.Punishments
{
    public class Punishment : IDisposable
    {
        public bool Enabled { get; set; } = true;

        public System.Timers.Timer ticker;

        private bool unsubscribeOnDispose = false;
        virtual public int ActivateOnRound { get; set; } = 0;
        virtual public int DeactivateOnRound { get; set; } = 30;
        public GameProcess GameProcess { get; set; }

        public GameData GameData { get; set; }

        public Player Player { get; set; }

        public Punishment(int resetTime = 5000, bool resetOnNewRound = false, int tickSpeed = 500)
        {
            ticker = new System.Timers.Timer(tickSpeed);

            GameProcess = Program.GameProcess;
            GameData = Program.GameData;
            Player = GameData.Player;

            Console.WriteLine("Punishment activated: " + this.GetType().Name);

            if (resetOnNewRound)
            {
                unsubscribeOnDispose = true;
                GameData.MatchInfo.OnMatchNewRound += ResetOnMatchNewRound;
            }

            if(resetTime != 0) 
            {
                ResetAfter(resetTime);
            }

            ticker.Elapsed += Tick;
            ticker.AutoReset = true;
            ticker.Enabled = true;
        }

        virtual public void AfterActivate(bool logging = true, string overrideName = "")
        {
            if(logging)
            {
                ReplayLogger.Log(this.GetType().Name);

                if (Program.Debug.ShowDebugMessages)
                {
                    Program.GameConsole.SendCommand("Say \"Punishment activated (" + this.GetType().Name + ")\"");
                }
            }
        }

        virtual public bool CanActivate()
        {
            if(Enabled == false)
            {
                return false;
            }

            if (GameData.MatchInfo.isWarmup && Program.Debug.AllowInWarmup == false)
            {
                return false;
            }

            if ((GameData.MatchInfo.RoundNumber < ActivateOnRound || GameData.MatchInfo.RoundNumber > DeactivateOnRound) && Program.Debug.IgnoreActivateOnRound == false )
            {
                return false;
            }

            return true;
        }

        private async Task ResetAfter(int ms)
        {
            await Task.Delay(ms);
            this.Reset();
            this.Dispose();
        }
        private void ResetOnMatchNewRound(object sender, EventArgs e)
        {
            this.Reset();
            this.Dispose();
        }

        virtual public void Reset()
        {
            // Reset punishment
        }

        virtual public void Tick(Object source, ElapsedEventArgs e)
        {
            // Tick Tock
        }

        public virtual void Dispose()
        {
            if (unsubscribeOnDispose)
            {
                GameData.MatchInfo.OnMatchNewRound -= this.ResetOnMatchNewRound;
            }
            ticker.Enabled = false;
            ticker.Stop();
            ticker.Dispose();
        }
    }
}
