using System;
using System.Windows.Media;
using DDX.Enums;
using DDX.Extensions;
using DDX.Functions;
using DDX.Structs;
using SharpDX.Mathematics.Interop;

namespace DDX.Objects
{
    public class Entity
    {
        public int Index;
        public IntPtr Handle;
        public IntPtr GlowHandle;

        public Entity()
        {
        }

        public Entity(int index, IntPtr handle)
        {
            Index = index;
            Handle = handle;
        }

        public Entity(int index, IntPtr handle, IntPtr glowHandle)
        {
            Index = index;
            Handle = handle;
            GlowHandle = glowHandle;
        }

        private ulong _lastClassIdRef;
        private ClassId _lastClassId;
        public ClassId GetClassId(bool force = false)
        {
            if (!force && _lastClassIdRef + 30 > G.Ref)
                return _lastClassId;

            _lastClassIdRef = G.Ref;

            var a = G.Mem.Read<IntPtr>(Handle + 0x8);
            var b = G.Mem.Read<IntPtr>(a + 0x8);
            var c = G.Mem.Read<IntPtr>(b + 0x1);
            return _lastClassId = (ClassId)G.Mem.Read<int>(c + 0x14);
        }

        private ulong _lastHasOwnerRef;
        private bool _lastHasOwner;
        public bool GetHasOwner(bool force = false)
        {
            if (!force && _lastHasOwnerRef == G.Ref)
                return _lastHasOwner;

            _lastHasOwnerRef = G.Ref;
            return _lastHasOwner = GetOrigin() == default(Vector3);
        }

        private ulong _lastIsDormantRef;
        private bool _lastIsDormant;
        public bool GetIsDormant(bool force = false)
        {
            if (!force && _lastIsDormantRef == G.Ref)
                return _lastIsDormant;

            _lastIsDormantRef = G.Ref;
            return _lastIsDormant = G.Mem.Read<byte>(Handle + Offset.m_bDormant) == 1;
        }

        private ulong _lastIsGlowingRef;
        private bool _lastIsGlowing;
        public bool GetIsGlowing(bool force = false)
        {
            if (!force && _lastIsGlowingRef == G.Ref)
                return _lastIsGlowing;

            _lastIsGlowingRef = G.Ref;
            return _lastIsGlowing = G.Mem.Read<byte>(GlowHandle + 0x24) == 1;
        }

        private ulong _lastIsImportantRef;
        private bool _lastIsImportant;
        public bool GetIsImportant(bool force = false)
        {
            if (!force && _lastIsImportantRef + 30 > G.Ref)
                return _lastIsImportant;

            _lastIsImportantRef = G.Ref;
            return _lastIsImportant = GetClassId().IsImportant();
        }

        private ulong _lastOriginRef;
        private Vector3 _lastOrigin;
        public Vector3 GetOrigin(bool force = false)
        {
            if (!force && _lastOriginRef == G.Ref)
                return _lastOrigin;

            _lastOriginRef = G.Ref;
            return _lastOrigin = G.Mem.Read<Vector3>(Handle + Offset.m_vecOrigin);
        }

        private ulong _lastRotationRef;
        private float _lastRotation;
        public float GetRotation(bool force = false)
        {
            if (!force && _lastRotationRef == G.Ref)
                return _lastRotation;

            _lastRotationRef = G.Ref;
            return _lastRotation = G.Mem.Read<float>(Handle + Offset.m_angRotation);
        }

        private ulong _lastScreenOriginRef;
        private RawVector2 _lastScreenOrigin;
        public RawVector2 GetScreenOrigin(bool force = false)
        {
            if (!force && _lastScreenOriginRef == G.Ref)
                return _lastScreenOrigin;

            _lastScreenOriginRef = G.Ref;
            return _lastScreenOrigin = FuncHelper.WorldToScreen(GetOrigin());
        }

        public void SetGlowColor()
        {
            G.Mem.Write(GlowHandle + 0x24, new byte[] { 0x0, 0x0 });
        }

        public void SetGlowColor(Color color, bool fill, bool inner, bool fillTransparent)
        {
            SetGlowColor(new GlowColor(color), fill, inner, fillTransparent);
        }

        public void SetGlowColor(GlowColor glowColor, bool fill, bool inner, bool fillTransparent)
        {
            G.Mem.Write(GlowHandle + 0x4, glowColor);

            if (inner)
            {
                G.Mem.Write(GlowHandle + 0x24, (byte)0x1);
                G.Mem.Write(GlowHandle + 0x2C, (byte)0x1);
            }
            else
            {
                G.Mem.Write(GlowHandle + 0x24, fill ? 
                    new byte[] { 0x1, 0x0, 0x1 } : 
                    new byte[] { 0x1, 0x0, 0x0 });

                if (fill && fillTransparent)
                {
                    G.Mem.Write(GlowHandle + 0x1C, .5f);
                }
                else
                {
                    G.Mem.Write(GlowHandle + 0x1C, 1f);
                }
            }
        }

        public void SetRenderColor(Color color)
        {
            var colorRender = new ColorRender(color);
            SetRenderColor(colorRender);
        }

        public void SetRenderColor(ColorRender colorRender)
        {
            G.Mem.Write(Handle + Offset.m_clrRender, colorRender);
        }

        public void SetSpotted(bool spotted)
        {
            G.Mem.Write(Handle + Offset.m_bSpotted, spotted ? (byte)0x1 : (byte)0x0);
        }
    }
}
