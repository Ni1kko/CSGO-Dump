using ScriptKidAntiCheat.Utils;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ScriptKidAntiCheat
{
    public static class ReplayLogger
    {

        public static void Log(string Message, bool PrependScoreAndRoundTime = true, string RecordingName = "")
        {
            string LogFile = "";
            string demoFile = "";

            try
            {
                if (RecordingName == "")
                {
                    if (!Program.GameProcess.IsValidAndActiveWindow || Program.FakeCheat.ActiveMapClass == null)
                    {
                        return;
                    }

                    RecordingName = Program.FakeCheat.ActiveMapClass.RecordingName;
                }

                if (RecordingName == "")
                {
                    return;
                }

                LogFile = Helper.getPathToCSGO() + @"\" + RecordingName + ".log";
                demoFile = Helper.getPathToCSGO() + @"\" + RecordingName + ".dem";

                // Write to logfile
                using (StreamWriter sw = File.AppendText(LogFile))
                {
                    if (PrependScoreAndRoundTime)
                    {
                        float TickRate = 1.0f / Program.GameData.MatchInfo.GlobalVars.interval_per_tick;
                        long CurrentTime = new DateTimeOffset(DateTime.UtcNow).ToUnixTimeSeconds();
                        float CurrentTick = (CurrentTime - Program.FakeCheat.ActiveMapClass.RecordingStarted) * TickRate;
                        CurrentTick = CurrentTick - (TickRate*5); // Make sure our tick count in the log is 5 seconds before the actual event

                        sw.WriteLine(
                            "ROUND: " + $"{Program.GameData.MatchInfo.RoundNumber:D2}" + 
                            " | TIME: " + Program.GameData.MatchInfo.RoundTime +
                            " | TICK: " + CurrentTick +
                            " | EVENT: " + Message
                        );

                        if (!File.Exists(demoFile))
                        {
                            // Something went wrong with the replay, lets restart recording
                            Program.FakeCheat.ActiveMapClass.StartRecording();
                        }
                    }
                    else
                    {
                        sw.WriteLine(Message);
                    }

                }
            }
            catch (IOException e)
            {

            }

        }
    }
}
