using FadeAPI.Enums;
using FadeAPI.Helpers;
using FadeAPI.Memory;
using FadeAPI.Structs;
using System;
using System.Windows.Forms;

namespace FadeAPI.Objects
{
    public class PlayerModel
    {
        private readonly IntPtr _handle;
        private readonly int[] _boneArray;

        public PlayerModel(IntPtr handle, string name)
        {
            _handle = handle;

            switch (name)
            {
                //[swap]
                //[block]
                case "tm_pirate":
                    _boneArray = ModelBoneArray.tm_pirate;
                    break;
                //[/block]
                //[block]
                case "ctm_fbi":
                    _boneArray = ModelBoneArray.ctm_fbi;
                    break;
                //[/block]
                //[block]
                case "tm_phoenix":
                    _boneArray = ModelBoneArray.tm_phoenix;
                    break;
                //[/block]
                //[block]
                case "tm_professional":
                    _boneArray = ModelBoneArray.tm_professional;
                    break;
                //[/block]
                //[block]
                case "ctm_swat":
                    _boneArray = ModelBoneArray.ctm_swat;
                    break;
                //[/block]
                //[block]
                case "tm_anarchist":
                    _boneArray = ModelBoneArray.tm_anarchist;
                    break;
                //[/block]
                //[block]
                case "ctm_sas":
                    _boneArray = ModelBoneArray.ctm_sas;
                    break;
                //[/block]
                //[block]
                case "ctm_gign":
                    _boneArray = ModelBoneArray.ctm_gign;
                    break;
                //[/block]
                //[block]
                case "tm_leet":
                    _boneArray = ModelBoneArray.tm_leet;
                    break;
                //[/block]
                //[block]
                case "ctm_st6":
                    _boneArray = ModelBoneArray.ctm_st6;
                    break;
                //[/block]
                //[block]
                case "ctm_idf":
                    _boneArray = ModelBoneArray.ctm_idf;
                    break;
                //[/block]
                //[block]
                case "tm_balkan":
                    _boneArray = ModelBoneArray.tm_balkan;
                    break;
                //[/block]
                //[block]
                case "tm_separatist":
                    _boneArray = ModelBoneArray.tm_separatist;
                    break;
                //[/block]
                //[block]
                case "ctm_gsg9":
                    _boneArray = ModelBoneArray.ctm_gsg9;
                    break;
                //[/block]
                //[/swap]
            }
        }

        public Vector3 BoneOrigin(Bone bone, bool debug = false)
        {
            return debug ? Functions.BoneToVector3(_handle, (int)bone) : Functions.BoneToVector3(_handle, _boneArray[(int)bone]);
        }

        public Vector2 GetAngleToBone(Bone bone)
        {
            return Functions.CalcAngle(Global.LocalPlayer.ViewOrigin, BoneOrigin(bone));
        }

        public float ScreenBoneDistance(Bone bone)
        {
            //[swap]
            var width = Screen.PrimaryScreen.Bounds.Width;
            var height = Screen.PrimaryScreen.Bounds.Height;
            //[/swap]

            //[swap]
            var src = new Vector2(width / 2f, height / 2f);
            var dst = ScreenBoneOrigin(bone);
            //[/swap]

            return Functions.ScreenDistance(src, dst);
        }

        public float ScreenBoneDistance(Bone bone, Vector2 src)
        {
            var dst = ScreenBoneOrigin(bone);
            return Functions.ScreenDistance(src, dst);
        }

        public Vector2 ScreenBoneOrigin(Bone bone)
        {
            return Functions.WorldToScreen(BoneOrigin(bone), GlobalRead.ViewMatrix);
        }
    }
}
