using System;
using DDX.Core;
using DDX.Enums;
using DDX.Functions;
using DDX.Structs;
using SharpDX.Mathematics.Interop;

namespace DDX.Objects
{
    public class Model
    {
        private readonly IntPtr _handle;
        public readonly int[] MyBones;

        public Model(IntPtr handle, string modelName)
        {
            _handle = handle;

            switch (modelName)
            {
                case "ctm_fbi":         MyBones = Bones.ctm_fbi; return;
                case "ctm_gign":        MyBones = Bones.ctm_gign; return;
                case "ctm_gsg9":        MyBones = Bones.ctm_gsg9; return;
                case "ctm_idf":         MyBones = Bones.ctm_idf; return;
                case "ctm_sas":         MyBones = Bones.ctm_sas; return;
                case "ctm_st6":         MyBones = Bones.ctm_st6; return;
                case "ctm_swat":        MyBones = Bones.ctm_swat; return;
                case "tm_anarchist":    MyBones = Bones.tm_anarchist; return;
                case "tm_balkan":       MyBones = Bones.tm_balkan; return;
                case "tm_leet":         MyBones = Bones.tm_leet; return;
                case "tm_phoenix":      MyBones = Bones.tm_phoenix; return;
                case "tm_pirate":       MyBones = Bones.tm_pirate; return;
                case "tm_professional": MyBones = Bones.tm_professional; return;
                case "tm_separatist":   MyBones = Bones.tm_separatist; return;
                default:                MyBones = Bones.tm_leet; return;
            }
        }

        public ViewAngles GetBoneAngles(Bone bone)
        {
            return FuncHelper.CalcAngle(G.Local.GetViewOrigin(), GetBoneOrigin(bone));
        }

        private ulong _lastBoneMatrixHandleRef;
        private IntPtr _lastBoneMatrixHandle;
        public IntPtr GetBoneMatrixHandle(bool force = false)
        {
            if (!force && _lastBoneMatrixHandleRef == G.Ref)
                return _lastBoneMatrixHandle;

            var temp = G.Mem.Read<IntPtr>(_handle + Offset.m_dwBoneMatrix);
            if (temp == IntPtr.Zero)
                return _lastBoneMatrixHandle;

            _lastBoneMatrixHandleRef = G.Ref;
            return _lastBoneMatrixHandle = temp;
        }

        public Vector3 GetBoneOrigin(Bone bone)
        {
            var @base = GetBoneMatrixHandle() + MyBones[(int)bone] * 0x30;

            var vec3 = new Vector3
            {
                X = G.Mem.Read<float>(@base + 0xC),
                Y = G.Mem.Read<float>(@base + 0x1C),
                Z = G.Mem.Read<float>(@base + 0x2C),
            };

            return vec3;
        }

        public RawVector2 GetScreenBoneOrigin(Bone bone)
        {
            return FuncHelper.WorldToScreen(GetBoneOrigin(bone));
        }
    }
}
