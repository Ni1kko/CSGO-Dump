using ScriptKidAntiCheat.Classes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ScriptKidAntiCheat.Utils;
using ScriptKidAntiCheat.Punishments;
using ScriptKidAntiCheat.Internal;
using ScriptKidAntiCheat.Data;
using System.Threading;
using SharpDX;

namespace ScriptKidAntiCheat
{
    class de_inferno : Map
    {
        public override Int32 MapID { get; set; } = 1767859556;

        public de_inferno()
        {

            // A Short / House
            TripWire a_short = new TripWire(
                new { 
                    x1 = 1928, y1 = 178,
                    x2 = 2016, y2 = 180,
                    x3 = 2024, y3 = -262,
                    x4 = 1934, y4 = -261,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_short.OnTriggered += tripWirePunishments;
            TripWires.Add(a_short);
            // ---

            // A Apartments
            TripWire a_aps = new TripWire(
                new { 
                    x1 = 1796, y1 = -251,
                    x2 = 1793, y2 = -395,
                    x3 = 1708, y3 = -383,
                    x4 = 1709, y4 = -246,
                    z = 0
                }, 100, default, 50
            );
            a_aps.checkFromMemory = true;
            a_aps.OnTriggered += DropThatGun;
            TripWires.Add(a_aps);
            // ---

            // B From Banana
            TripWire b_banana = new TripWire(
                new { 
                    x1 = 661, y1 = 2292,
                    x2 = 910, y2 = 2320,
                    x3 = 908, y3 = 2436,
                    x4 = 653, y4 = 2428,
                    z = 0
                }, 50, Team.Terrorists, 50
            );
            b_banana.OnTriggered += tripWirePunishments;
            TripWires.Add(b_banana);
            // ---

        }

        public void DropThatGun(TripWire TripWire)
        {
            Vector3 AimDirection = Program.GameData.Player.AimDirection;
            if(AimDirection.X < 0)
            {
                // Coming from CT side
                Punishment p = new Yeeeeeeeet(TripWire, -5000, -1000, 5000, 1000);
            } else
            {
                // Coming from T side
                Punishment p = new Yeeeeeeeet(TripWire, 5000, -1000, -5000, 1000);
            }
            
        }


    }
}
