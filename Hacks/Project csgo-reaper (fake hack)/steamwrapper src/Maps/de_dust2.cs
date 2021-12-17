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
    class de_dust2 : Map
    {
        public override Int32 MapID { get; set; } = 1683973476;

        public de_dust2()
        {

            // A LONG
            TripWire a_long = new TripWire(
                new { 
                    x1 = 1222, y1 = 1998,
                    x2 = 1692, y2 = 1756,
                    x3 = 1845, y3 = 2139,
                    x4 = 1212, y4 = 2096,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_long.OnTriggered += tripWirePunishments;
            TripWires.Add(a_long);
            // ---

            // A CW
            TripWire a_cw = new TripWire(
                new { 
                    x1 = 552, y1 = 1811,
                    x2 = 231, y2 = 1849,
                    x3 = 196, y3 = 2041,
                    x4 = 554, y4 = 2031,
                    z = 0
                }, 50, Team.Terrorists
            );
            a_cw.OnTriggered += tripWirePunishments;
            TripWires.Add(a_cw);
            // ---

            // B Main
            TripWire b_main = new TripWire(
                new { 
                    x1 = -1828, y1 = 1798,
                    x2 = -2167, y2 = 1788,
                    x3 = -2163, y3 = 1880,
                    x4 = -1830, y4 = 1874,
                    z = 0
                }, 50, Team.Terrorists
            );
            b_main.OnTriggered += tripWirePunishments;
            TripWires.Add(b_main);
            // ---

            // B Door
            TripWire b_door = new TripWire(
                new { 
                    x1 = -1376, y1 = 2257,
                    x2 = -1299, y2 = 2310,
                    x3 = -1278, y3 = 2063,
                    x4 = -1362, y4 = 2091,
                    z = 0
                }, 50, Team.Terrorists
            );
            b_door.OnTriggered += tripWirePunishments;
            TripWires.Add(b_door);
            // ---

            
        }


    }
}
