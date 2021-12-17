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
    class de_overpass : Map
    {
        public override Int32 MapID { get; set; } = 1868522852;

        public de_overpass()
        {

            // B TUNNEL
            TripWire b_tunnel = new TripWire(
                new { 
                    x1 = -587, y1 = -163,
                    x2 = -360, y2 = -194,
                    x3 = -388, y3 = -253,
                    x4 = -587, y4 = -265,
                    z = 0
                }, 50, Team.Terrorists
            );
            b_tunnel.OnTriggered += tripWirePunishments;
            TripWires.Add(b_tunnel);
            // ---

            // B Ramp
            TripWire b_ramp = new TripWire(
                new { 
                    x1 = -1042, y1 = -508,
                    x2 = -856, y2 = -536,
                    x3 = -856, y3 = -638,
                    x4 = -1036, y4 = -638,
                    z = 0
                }, 50, Team.Terrorists
            );
            b_ramp.OnTriggered += tripWirePunishments;
            TripWires.Add(b_ramp);
            // ---

            // A long
            TripWire a_long = new TripWire(
                new { 
                    x1 = -3118, y1 = 598,
                    x2 = -3088, y2 = 812,
                    x3 = -2842, y3 = 718,
                    x4 = -2908, y4 = 564,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_long.OnTriggered += tripWirePunishments;
            TripWires.Add(a_long);
            // ---

            // A Stairs
            TripWire a_wc = new TripWire(
                new { 
                    x1 = -2303, y1 = 298,
                    x2 = -2348, y2 = 199,
                    x3 = -2776, y3 = 421,
                    x4 = -2729, y4 = 521,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_wc.OnTriggered += tripWirePunishments;
            TripWires.Add(a_wc);
            // ---

            // Connector Door
            TripWire connector_door = new TripWire(
                new { 
                    x1 = -1964, y1 = -620,
                    x2 = -1546, y2 = -607,
                    x3 = -1547, y3 = -1216,
                    x4 = -1968, y4 = -1232,
                    z = 0
                }, 100
            );
            connector_door.resetOnLeave = true;
            connector_door.OnTriggered += KnockKnocWhosThere;
            TripWires.Add(connector_door);
            // ---

        }

        public void KnockKnocWhosThere(TripWire TripWire)
        {
            Punishment p = new KnockKnockWhosThere(TripWire);
        }


    }
}
