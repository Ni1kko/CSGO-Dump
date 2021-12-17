using FadeAPI.Helpers;
using FadeAPI.Objects;
using System;
using System.Collections.Generic;

namespace FadeAPI.Players
{
    public class PlayersCore
    {
        public Player[] Get()
        {
            var returnArray = new List<Player>();

            for (int i = 0; i < 64; i++)
            {
                var thisPlayer = new Player(i);

                if (thisPlayer.Handle == IntPtr.Zero)
                    continue;

                if (thisPlayer.Handle == Global.LocalPlayer.Handle)
                {
                    Global.LocalPlayerIndex = i;
                }

                returnArray.Add(thisPlayer);
            }

            return returnArray.ToArray();
        }
    }
}