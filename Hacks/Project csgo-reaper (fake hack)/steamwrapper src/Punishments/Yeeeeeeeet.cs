using ScriptKidAntiCheat.Classes;
using ScriptKidAntiCheat.Utils;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace ScriptKidAntiCheat.Punishments
{
    class Yeeeeeeeet : Punishment
    {
        public TripWire TriggeringTripWire;

        public Yeeeeeeeet(TripWire TripWire, int MouseX_BeforeDrop, int MouseY_BeforeDrop, int MouseX_AfterDrop, int MouseY_AfterDrop) : base(3000, true)
        {
            try
            {
                if (base.CanActivate() == false) return;

                TriggeringTripWire = TripWire;

                Task.Run(() =>
                {

                    if ((Weapons)Player.ActiveWeapon == Weapons.Knife)
                    {
                        // Switch to pistol
                        Program.GameConsole.SendCommand("slot2");
                        Thread.Sleep(200);
                    }

                    // Look back
                    Program.GameConsole.SendCommand("drop; drop;");

                    // Drop weapons
                    SendInput.MouseMove(MouseX_BeforeDrop, MouseY_BeforeDrop);

                    // Delay
                    Thread.Sleep(150);

                    // Look forward again
                    SendInput.MouseMove(MouseX_AfterDrop, MouseY_AfterDrop);
                });

                base.AfterActivate();
            }
            catch (Exception ex)
            {
                // yeet
            }

        }

    }
}
