using FadeAPI.Memory;
using FadeAPI.Objects;
using System;
using System.Collections.Generic;

namespace FadeAPI.GameObjects
{
    public class GameObjectsCore
    {
        public GameObject[] Get()
        {
            var returnArray = new List<GameObject>();

            var length = GlobalRead.GameObjectsCount;
            for (int i = 64; i < length; i++)
            {
                var thisGameObject = new GameObject(i);

                if (thisGameObject.Handle == IntPtr.Zero)
                    continue;

                returnArray.Add(thisGameObject);
            }

            return returnArray.ToArray();
        }
    }
}