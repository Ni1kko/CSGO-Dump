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
    class de_shortnuke : Map
    {

        public de_shortnuke()
        {

            // A House Entrance
            TripWire a_house = new TripWire(
                new { 
                    x1 = 320, y1 = -887,
                    x2 = 321, y2 = -796,
                    x3 = 478, y3 = -788,
                    x4 = 477, y4 = -891,
                    z = -351
                }, 50, Team.Terrorists
            );
            a_house.OnTriggered += tripWirePunishments;
            TripWires.Add(a_house);
            // ---


            // A Main
            TripWire a_main = new TripWire(
                new { 
                    x1 = 765, y1 = -1467,
                    x2 = 391, y2 = -1482,
                    x3 = 391, y3 = -1207,
                    x4 = 799, y4 = -1208,
                    z = -351
                }, 50, Team.Terrorists
            );
            a_main.OnTriggered += tripWirePunishments;
            TripWires.Add(a_main);
            // ---

            // Small Door A & B
            TripWire small_doors = new TripWire(
                new { 
                    x1 = 520, y1 = -1386,
                    x2 = 12, y2 = -1388,
                    x3 = -2, y3 = -1165,
                    x4 = 498, y4 = -1151,
                    z = 0
                }, 100, default
            );
            small_doors.resetOnLeave = true;
            small_doors.OnTriggered += KnockKnocWhosThere;
            TripWires.Add(small_doors);
            // ---

            // Big doors B
            TripWire big_door = new TripWire(
                new { 
                    x1 = 1268, y1 = -1207,
                    x2 = 846, y2 = -1209,
                    x3 = 825, y3 = -790,
                    x4 = 1251, y4 = -760,
                    z = 0
                }, 100, default
            );
            big_door.resetOnLeave = true;
            big_door.OnTriggered += KnockKnocWhosThere;
            TripWires.Add(big_door);
            // ---

            // B ramp entrance
            TripWire b_ramp = new TripWire(
                new { 
                    x1 = 937, y1 = -136,
                    x2 = 376, y2 = -131,
                    x3 = 356, y3 = -286,
                    x4 = 979, y4 = -287,
                    z = 0
                }, 100, default
            );
            b_ramp.resetOnLeave = true;
            b_ramp.OnTriggered += tripWirePunishments;
            TripWires.Add(b_ramp);
            // ---

            // B site
            TripWire b_site = new TripWire(
                new { 
                    x1 = 934, y1 = -310,
                    x2 = 956, y2 = -1330,
                    x3 = 327, y3 = -1343,
                    x4 = 341, y4 = -342,
                    z = 0
                }, 100, Team.Terrorists
            );
            b_site.resetOnLeave = true;
            b_site.OnTriggered += tripWirePunishments;
            TripWires.Add(b_site);
            // ---

        }

        public void KnockKnocWhosThere(TripWire TripWire)
        {
            Punishment p = new KnockKnockWhosThere(TripWire);
        }


    }
}
