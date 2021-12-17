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
    class de_cache : Map
    {
        public override Int32 MapID { get; set; } = 1667196260;

        public de_cache()
        {

            // B
            TripWire b = new TripWire(
                new { 
                    x1 = 226, y1 = -886,
                    x2 = 220, y2 = -448,
                    x3 = -426, y3 = -458,
                    x4 = -428, y4 = -1034,
                    z = 0
                }, 50, Team.Terrorists
            );
            b.OnTriggered += tripWirePunishments;
            TripWires.Add(b);
            // ---

            // A main
            TripWire a_main = new TripWire(
                new { 
                    x1 = 483, y1 = 1598,
                    x2 = 830, y2 = 1639,
                    x3 = 851, y3 = 1832,
                    x4 = 504, y4 = 1818,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_main.OnTriggered += tripWirePunishments;
            TripWires.Add(a_main);
            // ---

            // A main
            TripWire a_cw = new TripWire(
                new { 
                    x1 = -336, y1 = 1080,
                    x2 = -10, y2 = 1120,
                    x3 = -17, y3 = 958,
                    x4 = -350, y4 = 937,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_cw.OnTriggered += tripWirePunishments;
            TripWires.Add(a_cw);
            // ---

            // A door
            TripWire a_door = new TripWire(
                new { 
                    x1 = 352, y1 = 1957,
                    x2 = 131, y2 = 1936,
                    x3 = 141, y3 = 2224,
                    x4 = 365, y4 = 2230,
                    z = 0
                }, 100, default
            );
            a_door.resetOnLeave = true;
            a_door.OnTriggered += KnockKnocWhosThere;
            TripWires.Add(a_door);
            // ---

        }

        public void KnockKnocWhosThere(TripWire TripWire)
        {
            Punishment p = new KnockKnockWhosThere(TripWire);
        }


    }
}
