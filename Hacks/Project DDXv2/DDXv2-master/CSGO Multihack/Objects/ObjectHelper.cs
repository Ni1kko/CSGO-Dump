using System;
using System.Collections.Generic;
using System.Diagnostics;
using DDX.Enums;
using DDX.Extensions;
using DDX.Functions;

namespace DDX.Objects
{
    public static class ObjectHelper
    {
        public static void AssignLocalPlayer()
        {
            if (G.Local == null)
            {
                G.Local = new LocalPlayer();
            }

            G.Local.Index = GlobalRead.GetLocalPlayerIndex() + 1;
            G.Local.Handle = GlobalRead.GetLocalPlayerHandle();
        }

        private static ulong _lastGetEntitiesRef;
        private static List<Entity> _lastGetEntities;
        public static List<Entity> GetEntities(bool force = false)
        {
            if (!force && _lastGetEntitiesRef + 30 > G.Ref)
                return _lastGetEntities;

            _lastGetEntitiesRef = G.Ref;

            var returnArray = new List<Entity>();

            for (var i = 64; i < GlobalRead.GetEntityListLength(); i++)
            {
                var thisHandle = GlobalRead.GetEntityHandleByIndex(i);

                if (thisHandle == IntPtr.Zero)
                {
                    continue;
                }

                var thisEntity = new Entity(i, thisHandle);

                if (thisEntity.GetIsDormant())
                {
                    continue;
                }

                if (thisEntity.GetHasOwner())
                {
                    if (thisEntity.GetClassId() != ClassId.CPredictedViewModel)
                    {
                        continue;
                    }
                }

                if (thisEntity.GetClassId().IsCommon())
                {
                    continue;
                }

                returnArray.Add(thisEntity);
            }

            return _lastGetEntities = returnArray;
        }

        private static ulong _lastGetPlayersRef;
        private static List<Player> _lastGetPlayers;
        public static List<Player> GetPlayers(bool force = false)
        {
            if (!force && _lastGetPlayersRef + 10 > G.Ref)
                return _lastGetPlayers;

            _lastGetPlayersRef = G.Ref;

            var returnList = new List<Player>();

            for (var i = 0; i < 32; i++)
            {
                var thisHandle = GlobalRead.GetEntityHandleByIndex(i);

                if (thisHandle == IntPtr.Zero ||
                    thisHandle == G.Local.Handle)
                {
                    continue;
                }

                var thisPlayer = new Player(i, thisHandle);

                if (thisPlayer.GetIsDormant())
                {
                    continue;
                }

                returnList.Add(thisPlayer);
            }

            return _lastGetPlayers = returnList;
        }
    }
}
