using SharpDX;
using ScriptKidAntiCheat.Classes;
using System;
using ScriptKidAntiCheat.Internal;
using System.Timers;

namespace ScriptKidAntiCheat.Utils
{
    public class TripWire
    {
        public bool active;
        public bool IsBeingTripped { get; set; } = false;

        public int x1, y1, x2, y2, x3, y3, x4, y4, z;

        public Team triggerForTeam;

        public int chance;

        public bool disableOnTriggered { get; set; } = true;

        public bool resetOnLeave { get; set; } = false;

        public bool checkFromMemory { get; set; } = false;

        public delegate void TripWireTrigger(TripWire TripWire);

        public event TripWireTrigger OnTriggered;

        private static System.Timers.Timer ticker;

        public TripWire(dynamic points, int triggerChance = 100, Team triggerOnlyForTeam = Team.Unknown, int checkSpeed = 500)
        {
            x1 = points.x1;
            y1 = points.y1;
            x2 = points.x2;
            y2 = points.y2;
            x3 = points.x3;
            y3 = points.y3;
            x4 = points.x4;
            y4 = points.y4;
            z = points.z;
            chance = triggerChance;
            active = true;
            triggerForTeam = triggerOnlyForTeam;
            ticker = new System.Timers.Timer(checkSpeed);
            ticker.Elapsed += checkTripWire;
            ticker.AutoReset = true;
            ticker.Enabled = true;
        }

        public void checkTripWire(Object source, ElapsedEventArgs e)
        {
            if (!Program.GameProcess.IsValidAndActiveWindow || !Program.GameData.Player.IsAlive() || (Program.GameData.MatchInfo.isWarmup && !Program.Debug.AllowInWarmup)) return;
            
            if (active == false && resetOnLeave == false) return;
            
            IsBeingTripped = isTripWireBeingTripped();

           // Console.WriteLine(IsBeingTripped);

            if (IsBeingTripped)
            {
                if(active)
                {
                    TripWireTriggeredCallback();
                }
            } else if(resetOnLeave == true && active == false)
            {
                active = true;
            }
        }

        public bool isTripWireBeingTripped()
        {
            Vector3 playerLocation;

            if (checkFromMemory == false)
            {
                // Stored value updates every 500 ms or so
                playerLocation = Program.GameData.Player.EyePosition;
            }
            else
            {
                // Read directly from memory every time
                playerLocation = Program.GameData.Player.getPlayerEyePosition(Program.GameProcess);
            }

            // Check if player is at trip wire location or not
            if (is_player_at_location(x1, y1, x2, y2, x3, y3, x4, y4, z, (int)playerLocation.X, (int)playerLocation.Y, (int)playerLocation.Z))
            {
                if (triggerForTeam != Team.Unknown && Program.GameData.Player.Team != triggerForTeam)
                {
                    return false;
                }

                return true;
            }

            return false;
        }

        public void TripWireTriggeredCallback()
        {
            if(disableOnTriggered)
            {
                active = false;
            }
            //chance = 100;
            if (chance != 100)
            {
                // Less than 100% chance to trigger
                var r = new Random();
                var p = r.Next(100);
                if (p >= 100 - chance)
                {
                    OnTriggered.Invoke(this); // Call event
                }
            } else
            {
                // always 100% chance to trigger
                OnTriggered.Invoke(this); // Call event
            }
        }

        // A utility function to calculate area 
        // of triangle formed by (x1, y1),  
        // (x2, y2) and (x3, y3) 
        static float area(int x1, int y1, int x2, int y2, int x3, int y3)
        {
            return (float)System.Math.Abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
        }

        // A function to check whether point P(x, y)  
        // lies inside the rectangle formed by A(x1, y1),  
        // B(x2, y2), C(x3, y3) and D(x4, y4)  
        static bool is_player_at_location(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int z, int playerX, int playerY, int playerZ)
        {
        
            if(z != 0)
            {
                if(playerZ < z)
                {
                    return false;
                }
            }

            if (playerX == 0 && playerY == 0 && playerZ == 0) return false;

            // Calculate area of rectangle ABCD  
            float A = area(x1, y1, x2, y2, x3, y3) + area(x1, y1, x4, y4, x3, y3);

            // Calculate area of triangle PAB  
            float A1 = area(playerX, playerY, x1, y1, x2, y2);

            // Calculate area of triangle PBC  
            float A2 = area(playerX, playerY, x2, y2, x3, y3);

            // Calculate area of triangle PCD  
            float A3 = area(playerX, playerY, x3, y3, x4, y4);

            // Calculate area of triangle PAD 
            float A4 = area(playerX, playerY, x1, y1, x4, y4);

            // Check if sum of A1, A2, A3   
            // and A4is same as A  
            return (A == A1 + A2 + A3 + A4);
        }

        public void reset()
        {
            active = true;
        }
    }
}
