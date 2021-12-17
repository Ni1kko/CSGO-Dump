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

namespace ScriptKidAntiCheat
{
    class de_mirage : Map
    {
        public override Int32 MapID { get; set; } = 1834968420;

        public de_mirage()
        {

            // T Spawn (debugging)
            if(Program.Debug.ShowDebugMessages)
            {
                TripWire t_spawn = new TripWire(
                    new { 
                        x1 = 1032, y1 = 351,
                        x2 = 957, y2 = 431,
                        x3 = 964, y3 = 673,
                        x4 = 1135, y4 = 646,
                        z = 0
                    }, 100, Team.Terrorists
                );
                t_spawn.OnTriggered += tripWirePunishments;
                TripWires.Add(t_spawn);
                // ---
            }

            // B Entrance
            TripWire b_entrance = new TripWire(
                new { 
                    x1 = -1952, y1 = 675,
                    x2 = -2093, y2 = 671,
                    x3 = -2089, y3 = 831,
                    x4 = -1952, y4 = 849,
                    z = 0
                }, 50, Team.Terrorists
            );
            b_entrance.OnTriggered += tripWirePunishments;
            TripWires.Add(b_entrance);
            // ---

            // B Window
            TripWire b_window = new TripWire(
                new { 
                    x1 = -1723, y1 = 629,
                    x2 = -1722, y2 = 523,
                    x3 = -1858, y3 = 534,
                    x4 = -1865, y4 = 629,
                    z = 24
                }, 50, Team.Terrorists
            );
            b_window.OnTriggered += tripWirePunishments;
            TripWires.Add(b_window);
            // ---

            // Connector to MID
            TripWire connector = new TripWire(
                new { 
                    x1 = -824, y1 = -843,
                    x2 = -534, y2 = -846,
                    x3 = -505, y3 = -1268,
                    x4 = -833, y4 = -1251,
                    z = 0
                }, 50, Team.Terrorists
            );
            connector.OnTriggered += tripWirePunishments;
            TripWires.Add(connector);
            // ---

            // CW to B
            TripWire cw = new TripWire(
                new { 
                    x1 = -676, y1 = -241,
                    x2 = -945, y2 = -295,
                    x3 = -968, y3 = -21,
                    x4 = -704, y4 = -22,
                    z = 0
                }, 50, Team.Terrorists
            );
            cw.OnTriggered += tripWirePunishments;
            TripWires.Add(cw);
            // ---

            // A main
            TripWire a_main = new TripWire(
                new
                {
                    x1 = 50, y1 = -1358,
                    x2 = 38, y2 = -1650,
                    x3 = -138, y3 = -1653,
                    x4 = -165, y4 = -1365,
                    z = -93
                }, 50, Team.Terrorists
            );
            a_main.OnTriggered += tripWirePunishments;
            TripWires.Add(a_main);
            // ---

            // Mid Window
            TripWire black_magic_window = new TripWire(
                new
                {
                    x1 = -1098, y1 = -539,
                    x2 = -1097, y2 = -763,
                    x3 = -1073, y3 = -758,
                    x4 = -1067, y4 = -520,
                    z = -143
                }, 100, default, 50
            );
            black_magic_window.checkFromMemory = true;
            black_magic_window.OnTriggered += DropWeaponsBehindMe;
            TripWires.Add(black_magic_window);
            // ---

            // A Palace Entrance
            TripWire a_palace = new TripWire(
                new
                {
                    x1 = -21, y1 = -2166,
                    x2 = -18, y2 = -2020,
                    x3 = 150, y3 = -2031,
                    x4 = 87, y4 = -2185,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_palace.OnTriggered += tripWirePunishments;
            TripWires.Add(a_palace);
            // ---

        }

        public void DropWeaponsBehindMe(TripWire TripWire)
        {
           
            Punishment p = new Yeeeeeeeet(TripWire, -8000, -500, 10000, 0);
        }

        public void debugging_tripwire(TripWire TripWire)
        {
           // Console.WriteLine("yeet");
          //  Punishment p = new ViolenceSpeedMomentum();
        }

    }
}
