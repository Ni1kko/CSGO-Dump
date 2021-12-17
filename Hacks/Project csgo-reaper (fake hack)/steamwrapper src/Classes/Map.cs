using System;
using System.Collections.Generic;
using ScriptKidAntiCheat.Punishments;
using ScriptKidAntiCheat.Utils;
using System.Threading;
using System.Threading.Tasks;
using ScriptKidAntiCheat.Win32;
using ScriptKidAntiCheat.Classes.Utils;

namespace ScriptKidAntiCheat.Classes
{
    public abstract class Map : IDisposable
    {
        bool disposed = false;
        public virtual Int32 MapID { get; set; }

        public string RecordingName = "";

        public List<Punishment> Punishments = new List<Punishment>();

        public List<TripWire> TripWires = new List<TripWire>();

        public long RecordingStarted;

        protected Map()
        {
            Task.Run(() =>
            {
                if(Program.Debug.SkipInitDelay == true)
                {
                    StartRecording();
                    // Setting up Punishments
                    Punishments.Add(new InvertMouseAds());
                    Punishments.Add(new NoPlantOrDefuse());
                    Punishments.Add(new NoSilentWalk());
                    Punishments.Add(new FlashInYourFace());
                    Punishments.Add(new NoSpray4U());
                    Punishments.Add(new DoYouEvenAimBro());
                    Punishments.Add(new BloodBrothers());
                    Punishments.Add(new BigSpender());
                    Punishments.Add(new BurningMan());
                } else
                {
                    Thread.Sleep(5000);
                    StartRecording();

                    // Setting up Punishments
                    Thread.Sleep(2000);
                    Punishments.Add(new InvertMouseAds());
                    Thread.Sleep(500);
                    Punishments.Add(new NoPlantOrDefuse());
                    Thread.Sleep(500);
                    Punishments.Add(new NoSilentWalk());
                    Thread.Sleep(500);
                    Punishments.Add(new FlashInYourFace());
                    Thread.Sleep(500);
                    Punishments.Add(new NoSpray4U());
                    Thread.Sleep(500);
                    Punishments.Add(new DoYouEvenAimBro());
                    Thread.Sleep(500);
                    Punishments.Add(new BloodBrothers());
                    Thread.Sleep(500);
                    Punishments.Add(new BigSpender());
                    Thread.Sleep(500);
                    Punishments.Add(new BurningMan());
                }
                

            });

            NewRound(null, null);

            Program.GameData.MatchInfo.OnMatchNewRound += NewRound;
        }

        public void StartRecording()
        {
           
            string now = DateTime.UtcNow.ToString("yyyy-MM-dd(HHmmss)");
            RecordingStarted = new DateTimeOffset(DateTime.UtcNow).ToUnixTimeSeconds();
            RecordingName = now + "#(" + this.GetType().Name + ")#sheeter";
            
            // Make sure csgo is focused when we start replay
            if (Program.GameProcess.IsValid && !Program.GameProcess.IsValidAndActiveWindow)
            {
                User32.SetForegroundWindow(Program.GameProcess.Process.MainWindowHandle);
            }

            // Start in-eye recording
            Program.GameConsole.SendCommand("stop; voice_scale 0; record \"" + RecordingName + "\"");

            Console.WriteLine("Started recording");

            Thread.Sleep(1000);

            // Clear the console (sneakyyyy)
            Program.GameConsole.SendCommand("clear");

            ReplayLogger.Log("Version: " + Program.version, false, RecordingName);
            ReplayLogger.Log("Nickname: " + Helper.getPlayerNickname(), false, RecordingName);
        }

        public void resetTripWires()
        {
            Console.WriteLine("Reset traps");
            foreach (TripWire tripwire in TripWires)
            {
                tripwire.reset();
            }
        }

        virtual public void NewRound(object sender, EventArgs e)
        {
            resetTripWires();
            PlayerConfig.ResetConfig();
        }

        public void Dispose()
        {
            foreach (Punishment Punishment in Punishments)
            {
                Punishment.Dispose();
            }
            Program.GameData.MatchInfo.OnMatchNewRound -= NewRound;
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposed)
                return;

            if (disposing)
            {
                Console.WriteLine("disposed");
            }

            disposed = true;
        }

        public enum stage1TripWirePunishments { NoCrosshairChallenge, ButterFingers }
        public enum stage2TripWirePunishments { ViolenceSpeedMomentum, ButterFingers }

        public void tripWirePunishments(TripWire TripWire)
        {

            string punishment = "";

            if (Program.GameData.MatchInfo.RoundNumber >= 3 || Program.Debug.TripWireStage == 2)
            {
                stage2TripWirePunishments ps2 = (stage2TripWirePunishments)(new Random()).Next(0, 2);
                punishment = ps2.ToString();
            }
            else if (Program.GameData.MatchInfo.RoundNumber < 3 || Program.Debug.TripWireStage == 1)
            {
                stage1TripWirePunishments ps1 = (stage1TripWirePunishments)(new Random()).Next(0, 2);
                punishment = ps1.ToString();
            }

            Activator.CreateInstance(Type.GetType("ScriptKidAntiCheat.Punishments." + punishment));

            Thread.Sleep(500);

            if (Program.Debug.ShowDebugMessages)
            {
                Program.GameConsole.SendCommand("Say \"TripWire Triggered (" + punishment + ")\"");
            }

        }

    }
}
