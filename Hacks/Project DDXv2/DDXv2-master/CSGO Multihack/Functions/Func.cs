using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using DDX.Enums;
using DDX.Extensions;
using DDX.Keyboard;
using DDX.Objects;
using DDX.Structs;
using DDX.Structs.Config;
using SharpDX.Direct2D1;
using SharpDX.DirectWrite;
using SharpDX.Mathematics.Interop;
using Brush = SharpDX.Direct2D1.Brush;
using Color = System.Windows.Media.Color;
using Entity = DDX.Objects.Entity;
using Factory = SharpDX.DirectWrite.Factory;
using FontStyle = SharpDX.DirectWrite.FontStyle;
using SolidColorBrush = SharpDX.Direct2D1.SolidColorBrush;
using TextAntialiasMode = SharpDX.Direct2D1.TextAntialiasMode;

// ReSharper disable CompareOfFloatsByEqualityOperator

namespace DDX.Functions
{
    public static class F
    {
        private const FontStyle FontStyle = SharpDX.DirectWrite.FontStyle.Normal;
        private const FontWeight FontWeight = SharpDX.DirectWrite.FontWeight.Normal;

        private static readonly Factory FontFactory = new Factory();

        public static SolidColorBrush BackgroundBrush;

        public static void BeginDraw()
        {
            G.O.D.BeginDraw();

            G.O.D.AntialiasMode = G.C.Stts.Antialiasing ? AntialiasMode.PerPrimitive : AntialiasMode.Aliased;

            G.O.D.TextAntialiasMode = TextAntialiasMode.Default;

            G.O.D.Clear(new RawColor4(0f, 0f, 0f, 0f));
        }

        public static void EndDraw()
        {
            G.O.D.TryEndDraw(out long t1, out long t2);
        }

        public static float GetSize(Player p)
        {
            var size = 30f * FuncHelper.DistanceScale(p.GetOrigin(), G.Local.GetViewOrigin());
            return size < 10f ? 10f : size;
        }

        public static float GetSize(Entity e, bool text)
        {
            if (!text)
            {
                return 5f * FuncHelper.DistanceScale(e.GetOrigin(), G.Local.GetViewOrigin());
            }

            var size = 30f * FuncHelper.DistanceScale(e.GetOrigin(), G.Local.GetViewOrigin());
            return size < 10f ? 10f : size;
        }

        public static TextFormat GetTextFormat(float size)
        {
            var niceSize = size == 0f ? 1f : size;

            var textFormat = new TextFormat(FontFactory, G.C.Stts.FontName, FontWeight, FontStyle, niceSize)
            {
                TextAlignment = TextAlignment.Center,
                WordWrapping = WordWrapping.NoWrap,
            };

            return textFormat;
        }

        public static void DrawText(TextFormat textFormat, string text, RawVector2 vec, Brush brush)
        {
            var xInc = vec.X + 1f;
            var yInc = vec.Y + 1f;

            G.O.D.DrawText(text, textFormat, new RawRectangleF(vec.X - 3f, vec.Y - 1f, xInc, yInc), BackgroundBrush);
            G.O.D.DrawText(text, textFormat, new RawRectangleF(vec.X - 3f, vec.Y + 1f, xInc, yInc), BackgroundBrush);
            G.O.D.DrawText(text, textFormat, new RawRectangleF(vec.X + 1f, vec.Y - 1f, xInc, yInc), BackgroundBrush);
            G.O.D.DrawText(text, textFormat, new RawRectangleF(vec.X + 1f, vec.Y + 1f, xInc, yInc), BackgroundBrush);

            G.O.D.DrawText(text, textFormat, new RawRectangleF(vec.X, vec.Y, vec.X, vec.Y), brush);
        }

        public static void DrawBackground(TextFormat format, string text, int x, int y)
        {
            var layout = new TextLayout(FontFactory, text, format, 0, format.FontSize);

            const float yMargin = 3;
            const float xMargin = 5;

            var width = layout.Metrics.Width;
            var xBg = x - width / 2f;
            var yBg = y;
            var height = layout.Metrics.Height;

            var tmp = G.C.Clr.Background;
            tmp.A = 80;

            G.O.D.FillRectangle(
                new RawRectangleF(
                    xBg - xMargin,
                    yBg - yMargin,
                    xBg + width + xMargin,
                    yBg + height + yMargin),
                tmp.ToBrush());

            var a = new RawVector2(xBg - xMargin, yBg - yMargin);
            var b = new RawVector2(xBg + width + xMargin, yBg - yMargin);
            var c = new RawVector2(xBg + width + xMargin, yBg + height + yMargin);
            var d = new RawVector2(xBg - xMargin, yBg + height + yMargin);

            G.O.D.DrawLine(a, b, BackgroundBrush, 2f);
            G.O.D.DrawLine(b, c, BackgroundBrush, 2f);
            G.O.D.DrawLine(c, d, BackgroundBrush, 2f);
            G.O.D.DrawLine(d, a, BackgroundBrush, 2f);
        }

        public static void Do2D(Entity e)
        {
            if (!G.C.Vis.Entities.Esp2D)
            {
                return;
            }

            var screenOrigin = e.GetScreenOrigin();
            if (!screenOrigin.IsValid())
            {
                // Not on screen
                return;
            }

            var radius = 15f * FuncHelper.DistanceScale(e.GetOrigin(), G.Local.GetViewOrigin());

            var color = e.GetIsImportant() ? G.C.Clr.EspEntitiesImportant : G.C.Clr.EspEntities;

            var ellipse = new Ellipse(screenOrigin, radius, radius);

            G.O.D.DrawEllipse(ellipse, BackgroundBrush, 3f);
            G.O.D.DrawEllipse(ellipse, color.ToBrush(), 2f);
        }

        public static void Do2D(Player p, DrawData d)
        {
            if (!G.C.Vis.Players.Esp2D)
            {
                return;
            }

            SolidColorBrush brush;

            if (p.GetIsAlly())
            {
                // Ally

                brush = G.C.Clr.EspAllies.ToBrush();
            }
            else
            {
                // Enemy

                if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                {
                    // Visible

                    brush = G.C.Clr.EspEnemiesVisible.ToBrush();
                }
                else
                {
                    // Hidden

                    brush = G.C.Clr.EspEnemies.ToBrush();
                }
            }

            // 1-11  21-2
            // |        |
            // 12      22
            //
            //
            // 32      42
            // |        |
            // 3-31  41-4

            var len = d.Width / 3;

            var p1 = new RawVector2(d.X, d.Y);
            var p2 = new RawVector2(d.X + d.Width, d.Y);
            var p3 = new RawVector2(d.X, d.Y + d.Height);
            var p4 = new RawVector2(p2.X, p3.Y);

            var p11 = new RawVector2(p1.X + len, p1.Y);
            var p12 = new RawVector2(p1.X, p1.Y + len);
            var p21 = new RawVector2(p2.X - len, p2.Y);
            var p22 = new RawVector2(p2.X, p2.Y + len);
            var p31 = new RawVector2(p3.X + len, p3.Y);
            var p32 = new RawVector2(p3.X, p3.Y - len);
            var p41 = new RawVector2(p4.X - len, p4.Y);
            var p42 = new RawVector2(p4.X, p4.Y - len);

            G.O.D.DrawLine(p1, p11, BackgroundBrush, 3f);
            G.O.D.DrawLine(p1, p12, BackgroundBrush, 3f);
            G.O.D.DrawLine(p2, p21, BackgroundBrush, 3f);
            G.O.D.DrawLine(p2, p22, BackgroundBrush, 3f);
            G.O.D.DrawLine(p3, p31, BackgroundBrush, 3f);
            G.O.D.DrawLine(p3, p32, BackgroundBrush, 3f);
            G.O.D.DrawLine(p4, p41, BackgroundBrush, 3f);
            G.O.D.DrawLine(p4, p42, BackgroundBrush, 3f);

            G.O.D.DrawLine(p1, p11, brush, 2f);
            G.O.D.DrawLine(p1, p12, brush, 2f);
            G.O.D.DrawLine(p2, p21, brush, 2f);
            G.O.D.DrawLine(p2, p22, brush, 2f);
            G.O.D.DrawLine(p3, p31, brush, 2f);
            G.O.D.DrawLine(p3, p32, brush, 2f);
            G.O.D.DrawLine(p4, p41, brush, 2f);
            G.O.D.DrawLine(p4, p42, brush, 2f);
        }

        public static void Do3D(Entity e)
        {
            if (!G.C.Vis.Entities.Esp3D)
            {
                return;
            }

            const float size = 5f;

            var origin = e.GetOrigin();
            origin.Z -= 2.5f;

            var p1 = origin;
            var p2 = origin;
            var p3 = origin;
            var p4 = origin;

            p1.X += size;
            p1.Y += size;
            var p1Raw = FuncHelper.WorldToScreen(p1);
            if (!p1Raw.IsValid())
            {
                return;
            }

            p2.X += size;
            p2.Y -= size;
            var p2Raw = FuncHelper.WorldToScreen(p2);
            if (!p2Raw.IsValid())
            {
                return;
            }

            p3.X -= size;
            p3.Y -= size;
            var p3Raw = FuncHelper.WorldToScreen(p3);
            if (!p3Raw.IsValid())
            {
                return;
            }

            p4.X -= size;
            p4.Y += size;
            var p4Raw = FuncHelper.WorldToScreen(p4);
            if (!p4Raw.IsValid())
            {
                return;
            }

            var p5 = p1;
            var p6 = p2;
            var p7 = p3;
            var p8 = p4;

            p5.Z += size;
            var p5Raw = FuncHelper.WorldToScreen(p5);
            if (!p5Raw.IsValid())
            {
                return;
            }

            p6.Z += size;
            var p6Raw = FuncHelper.WorldToScreen(p6);
            if (!p6Raw.IsValid())
            {
                return;
            }

            p7.Z += size;
            var p7Raw = FuncHelper.WorldToScreen(p7);
            if (!p7Raw.IsValid())
            {
                return;
            }

            p8.Z += size;
            var p8Raw = FuncHelper.WorldToScreen(p8);
            if (!p8Raw.IsValid())
            {
                return;
            }

            var brush = e.GetIsImportant() ? G.C.Clr.EspEntitiesImportant.ToBrush() : G.C.Clr.EspEntities.ToBrush();

            G.O.D.DrawLine(p1Raw, p2Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p2Raw, p3Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p3Raw, p4Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p4Raw, p1Raw, BackgroundBrush, 2f);

            G.O.D.DrawLine(p1Raw, p5Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p2Raw, p6Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p3Raw, p7Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p4Raw, p8Raw, BackgroundBrush, 2f);

            G.O.D.DrawLine(p5Raw, p6Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p6Raw, p7Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p7Raw, p8Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p8Raw, p5Raw, BackgroundBrush, 2f);

            G.O.D.DrawLine(p1Raw, p2Raw, brush, 1f);
            G.O.D.DrawLine(p2Raw, p3Raw, brush, 1f);
            G.O.D.DrawLine(p3Raw, p4Raw, brush, 1f);
            G.O.D.DrawLine(p4Raw, p1Raw, brush, 1f);

            G.O.D.DrawLine(p1Raw, p5Raw, brush, 1f);
            G.O.D.DrawLine(p2Raw, p6Raw, brush, 1f);
            G.O.D.DrawLine(p3Raw, p7Raw, brush, 1f);
            G.O.D.DrawLine(p4Raw, p8Raw, brush, 1f);

            G.O.D.DrawLine(p5Raw, p6Raw, brush, 1f);
            G.O.D.DrawLine(p6Raw, p7Raw, brush, 1f);
            G.O.D.DrawLine(p7Raw, p8Raw, brush, 1f);
            G.O.D.DrawLine(p8Raw, p5Raw, brush, 1f);
        }

        public static void Do3D(Player p, Model model)
        {
            if (!G.C.Vis.Players.Esp3D)
            {
                return;
            }

            const float size = 15f;
            const float heightAdd = 7f;

            var origin = p.GetOrigin();
            var rotation = p.GetRotation();

            var p1 = origin;
            var p2 = origin;
            var p3 = origin;
            var p4 = origin;

            p1.X += size;
            p1.Y += size;
            p1 = FuncHelper.RotatePoint(p1, origin, rotation);
            var p1Raw = FuncHelper.WorldToScreen(p1);
            if (!p1Raw.IsValid())
            {
                return;
            }

            p2.X += size;
            p2.Y -= size;
            p2 = FuncHelper.RotatePoint(p2, origin, rotation);
            var p2Raw = FuncHelper.WorldToScreen(p2);
            if (!p2Raw.IsValid())
            {
                return;
            }

            p3.X -= size;
            p3.Y -= size;
            p3 = FuncHelper.RotatePoint(p3, origin, rotation);
            var p3Raw = FuncHelper.WorldToScreen(p3);
            if (!p3Raw.IsValid())
            {
                return;
            }

            p4.X -= size;
            p4.Y += size;
            p4 = FuncHelper.RotatePoint(p4, origin, rotation);
            var p4Raw = FuncHelper.WorldToScreen(p4);
            if (!p4Raw.IsValid())
            {
                return;
            }

            var headOrigin = model.GetBoneOrigin(Bone.Head);
            headOrigin.Z += heightAdd;

            var p5 = p1;
            var p6 = p2;
            var p7 = p3;
            var p8 = p4;

            p5.Z = headOrigin.Z;
            var p5Raw = FuncHelper.WorldToScreen(p5);
            if (!p5Raw.IsValid())
            {
                return;
            }
            p6.Z = headOrigin.Z;
            var p6Raw = FuncHelper.WorldToScreen(p6);
            if (!p6Raw.IsValid())
            {
                return;
            }
            p7.Z = headOrigin.Z;
            var p7Raw = FuncHelper.WorldToScreen(p7);
            if (!p7Raw.IsValid())
            {
                return;
            }
            p8.Z = headOrigin.Z;
            var p8Raw = FuncHelper.WorldToScreen(p8);
            if (!p8Raw.IsValid())
            {
                return;
            }

            SolidColorBrush brush;

            if (p.GetIsAlly())
            {
                // Ally

                brush = G.C.Clr.EspAllies.ToBrush();
            }
            else
            {
                // Enemy

                if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                {
                    // Visible

                    brush = G.C.Clr.EspEnemiesVisible.ToBrush();
                }
                else
                {
                    // Hidden

                    brush = G.C.Clr.EspEnemies.ToBrush();
                }
            }

            G.O.D.DrawLine(p1Raw, p2Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p2Raw, p3Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p3Raw, p4Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p4Raw, p1Raw, BackgroundBrush, 2f);

            G.O.D.DrawLine(p1Raw, p5Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p2Raw, p6Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p3Raw, p7Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p4Raw, p8Raw, BackgroundBrush, 2f);

            G.O.D.DrawLine(p5Raw, p6Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p6Raw, p7Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p7Raw, p8Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p8Raw, p5Raw, BackgroundBrush, 2f);

            G.O.D.DrawLine(p1Raw, p2Raw, brush, 1f);
            G.O.D.DrawLine(p2Raw, p3Raw, brush, 1f);
            G.O.D.DrawLine(p3Raw, p4Raw, brush, 1f);
            G.O.D.DrawLine(p4Raw, p1Raw, brush, 1f);

            G.O.D.DrawLine(p1Raw, p5Raw, brush, 1f);
            G.O.D.DrawLine(p2Raw, p6Raw, brush, 1f);
            G.O.D.DrawLine(p3Raw, p7Raw, brush, 1f);
            G.O.D.DrawLine(p4Raw, p8Raw, brush, 1f);

            G.O.D.DrawLine(p5Raw, p6Raw, brush, 1f);
            G.O.D.DrawLine(p6Raw, p7Raw, brush, 1f);
            G.O.D.DrawLine(p7Raw, p8Raw, brush, 1f);
            G.O.D.DrawLine(p8Raw, p5Raw, brush, 1f);
        }

        public static void DoActiveWeapon(Player p, DrawData d)
        {
            if (!G.C.Vis.Players.ActiveWeapon)
            {
                return;
            }

            var weap = p.GetActiveWeapon();
            var output = weap.GetName();

            if (G.C.Vis.Players.ActiveWeaponProp.Ammo)
            {
                output += $" ({weap.GetAmmo()})";
            }

            DrawText(
                GetTextFormat(GetSize(p)),
                output,
                new RawVector2(
                    d.X + d.Width / 2f,
                    d.Y + d.Height),
                G.C.Clr.Text.ToBrush());
        }

        public static void DoAdditionalInfo()
        {
            if (!G.C.Vis.Entities.AdditionalInfo)
            {
                return;
            }

            if (G.Bomb == null)
            {
                return;
            }

            if (!G.Bomb.GetIsBombTicking())
            {
                return;
            }

            var screenOrigin = G.Bomb.GetScreenOrigin();
            if (!screenOrigin.IsValid())
            {
                // Not on screen
                return;
            }

            var size = GetSize(G.Bomb, true);
            var heightFix = G.C.Vis.Entities.Name ? size : 0f;
            screenOrigin.Y += size / 3f;

            var blowsecs = G.Bomb.GetBlowSeconds();
            if (blowsecs < 0)
            {
                blowsecs = 0;
            }

            var output = blowsecs.ToString("F1");

            var defusesecs = G.Bomb.GetDefuseSeconds();
            var defusesuccess = false;

            if (defusesecs > 0)
            {
                var playerdefusing = G.Local.GetIsDefusing();

                if (!playerdefusing)
                {
                    foreach (var p in ObjectHelper.GetPlayers())
                    {
                        if (p.Handle == IntPtr.Zero ||
                            p.Handle == G.Local.Handle)
                        {
                            continue;
                        }

                        if (p.GetIsDefusing())
                        {
                            playerdefusing = true;
                        }
                    }
                }

                if (playerdefusing)
                {
                    output = defusesecs.ToString("F0") + "/" + output;

                    if (defusesecs < blowsecs)
                    {
                        defusesuccess = true;
                    }
                }
            }

            Color color;

            if (defusesuccess)
            {
                color = G.C.Clr.TextNice;
            }
            else
            {
                color = blowsecs < 5 ? G.C.Clr.TextWarning : blowsecs < 10 ? G.C.Clr.TextImportant : G.C.Clr.Text;
            }

            DrawText(
                GetTextFormat(size),
                output,
                new RawVector2(
                    screenOrigin.X,
                    screenOrigin.Y + heightFix),
                color.ToBrush());
        }

        public static void DoAdditionalInfo(Player p, DrawData d)
        {
            if (!G.C.Vis.Players.AdditionalInfo)
            {
                return;
            }

            var info = string.Empty;

            if (p.GetIsDefusing())
            {
                info += "> Defusing <\n";
            }

            if (GlobalRead.GetPlayerC4() == p.Index)
            {
                info += "> Bomb <\n";
            }

            if (p.GetHasHostage())
            {
                info += "> Hostage <\n";
            }

            if (p.GetHasDefuser())
            {
                info += "Defuser\n";
            }

            if (info == string.Empty)
            {
                return;
            }

            var size = GetSize(p);
            var heightFix = G.C.Vis.Players.ActiveWeapon ? size : 0f;

            DrawText(
                GetTextFormat(size),
                info,
                new RawVector2(
                    d.X + d.Width / 2f,
                    d.Y + d.Height + heightFix),
                G.C.Clr.Text.ToBrush());
        }

        public static int AimbotLockLast;
        public static IntPtr AimbotLockPlayerHandle;

        private static ViewAngles _rcsLastPunch;

        public static bool DoAimbot(AimTarget aim, WeaponProperties cfg)
        {
            if (!cfg.Aimbot)
            {
                return false;
            }

            if (!aim.Selected)
            {
                return false;
            }

            if (cfg.AimbotProp.Key != Keys.F23 &&
                cfg.TriggerbotWithAimbotKey != Keys.F23 &&
                !KeyboardHelper.IsKeyPressed(cfg.AimbotProp.Key) &&
                !KeyboardHelper.IsKeyPressed(cfg.TriggerbotWithAimbotKey))
            {
                return false;
            }

            var punch = G.Local.GetPunch(true);
            var shouldRcs = ShouldDoRcs(cfg, true, false);

            var rawPunch = shouldRcs ? punch * 2f : default(ViewAngles);
            var angles = G.Local.GetViewAngles(true);

            // Now we have bullet angles
            angles += rawPunch;
            angles = angles.NormalizeAngles();

            var anglesDelta = aim.ViewAngles - angles;
            anglesDelta = anglesDelta.NormalizeAngles();

            // Smooth
            anglesDelta *= (100 - cfg.AimbotProp.Smooth) / 100f;
            anglesDelta = anglesDelta.NormalizeAngles();

            angles += anglesDelta;
            angles = angles.NormalizeAngles();

            // Now we have normal angles
            angles -= rawPunch;
            angles = angles.NormalizeAngles();

            punch.Pitch *= cfg.RcsProp.PitchStrength / 50f;
            punch.Yaw *= cfg.RcsProp.YawStrength / 50f;

            var punchDelta = punch - _rcsLastPunch;

            // Smooth
            punchDelta *= (100 - cfg.RcsProp.Smooth) / 100f;

            // Check delta
            if (Math.Abs(punchDelta.Pitch) < 2.8f)
            {
                if (shouldRcs)
                {
                    // Adding delta because of smooth
                    _rcsLastPunch += punchDelta;
                }
                else
                {
                    _rcsLastPunch = punch;
                }
            }

            G.Local.SetViewAngles(angles);
            AimbotLockPlayerHandle = aim.Player.Handle;
            AimbotLockLast = G.Now;

            return true;
        }

        public static void DoAimPoint(AimTarget aim, WeaponProperties cfg)
        {
            if (!G.C.Vis.AimPoint)
            {
                return;
            }

            if (!cfg.Aimbot)
            {
                return;
            }

            if (!aim.Selected)
            {
                return;
            }

            var model = aim.Player.GetPlayerModel();
            var screenOrigin = model.GetScreenBoneOrigin(aim.AimBone);
            if (!screenOrigin.IsValid())
            {
                // Not on screen
                return;
            }

            G.O.D.FillEllipse(new Ellipse(screenOrigin, 3f, 3f), BackgroundBrush);
            G.O.D.FillEllipse(new Ellipse(screenOrigin, 2f, 2f), G.C.Clr.AimPoint.ToBrush());
        }

        public static void DoAimFov(WeaponProperties cfg)
        {
            if (!G.C.Vis.AimFov)
            {
                return;
            }

            if (!cfg.Aimbot)
            {
                return;
            }

            if (!G.Local.GetCanShoot())
            {
                return;
            }

            var fovFixed = G.Local.GetFov();
            if (fovFixed == 0)
            {
                fovFixed = 1;
            }

            var percentage = G.ScreenParameters.Height / fovFixed;
            var size = percentage * cfg.AimbotProp.Fov * G.Local.GetScopeScale();

            var ellipse = new Ellipse(G.ScreenParameters.Center, size, size);

            G.O.D.DrawEllipse(ellipse, BackgroundBrush, 2.5f);
            G.O.D.DrawEllipse(ellipse, G.C.Clr.AimFov.ToBrush(), 1.5f);
        }

        public static void DoArmor(Player p, DrawData d)
        {
            if (!G.C.Vis.Players.Armor)
            {
                return;
            }

            if (p.GetArmor() == 0)
            {
                return;
            }

            d.Y -= 1.5f;
            d.Height += 3f;

            var size = GetSize(p);
            var margin = size * .1f;
            var width = size * .2f;

            var height = p.GetArmor() * d.Height / 100f;

            SolidColorBrush brush;

            if (p.GetIsAlly())
            {
                // Ally

                brush = G.C.Clr.ArmorAllies.ToBrush();
            }
            else
            {
                // Enemy

                if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                {
                    // Visible

                    brush = G.C.Clr.ArmorEnemies.ToBrush();
                }
                else
                {
                    // Hidden

                    brush = G.C.Clr.ArmorEnemiesVisible.ToBrush();
                }
            }

            // Semi-Background
            var tmp = BackgroundBrush;
            tmp.Opacity = .3f;

            G.O.D.FillRectangle(
                new RawRectangleF(
                    d.X - margin - width,
                    d.Y,
                    d.X - margin,
                    d.Y + d.Height), 
                tmp);

            tmp.Opacity = 1f;

            // Top background
            G.O.D.DrawLine(
                new RawVector2(
                    d.X - margin - width,
                    d.Y + d.Height - height - .25f),
                new RawVector2(
                    d.X - margin,
                    d.Y + d.Height - height - .25f), 
                BackgroundBrush, .5f);

            // Bar
            G.O.D.FillRectangle(
                new RawRectangleF(
                    d.X - margin - width,
                    d.Y + d.Height - height,
                    d.X - margin,
                    d.Y + d.Height), 
                brush);

            // Background
            G.O.D.DrawRectangle(
                new RawRectangleF(
                    d.X - margin - width,
                    d.Y,
                    d.X - margin,
                    d.Y + d.Height), 
                BackgroundBrush, .5f);
        }

        private static bool _autoDuckWorking;

        public static void DoAutoDuck(WeaponProperties cfg)
        {
            if (!G.C.Misc.AutoDuck)
            {
                return;
            }

            if (!G.Local.GetCanShoot() &&
                !_autoDuckWorking)
            {
                return;
            }

            if (KeyboardHelper.IsKeyPressed(Keys.LButton) ||
                KeyboardHelper.IsKeyPressed(cfg.TriggerbotProp.Key) ||
                KeyboardHelper.IsKeyPressed(cfg.TriggerbotWithAimbotKey) ||
                KeyboardHelper.IsKeyPressed(cfg.SilentAimProp.Key))
            {
                if (!_autoDuckWorking)
                {
                    G.Local.SetDuck(5);
                    _autoDuckWorking = true;
                }
            }
            else
            {
                if (_autoDuckWorking)
                {
                    G.Local.SetDuck(4);
                    _autoDuckWorking = false;
                }
            }
        }

        public static void DoAutoPistol()
        {
            if (!G.C.Misc.AutoPistol)
            {
                return;
            }

            if (!G.Local.GetIsMouseEnable())
            {
                return;
            }

            if (!KeyboardHelper.IsKeyPressed(Keys.LButton))
            {
                return;
            }

            if (!G.Local.GetIsAbleToShoot())
            {
                return;
            }

            if (G.Local.GetActiveWeapon().GetCategory() != WeaponCategory.Pistol)
            {
                return;
            }

            G.Local.SetAttack1(6);
        }

        private static bool _autoStopWorking;

        public static void DoAutoStop(WeaponProperties cfg)
        {
            if (!G.C.Misc.AutoStop)
            {
                return;
            }

            if (!G.Local.GetCanShoot() &&
                !_autoStopWorking)
            {
                return;
            }

            if (KeyboardHelper.IsKeyPressed(Keys.LButton) ||
                KeyboardHelper.IsKeyPressed(cfg.TriggerbotProp.Key) ||
                KeyboardHelper.IsKeyPressed(cfg.TriggerbotWithAimbotKey) ||
                KeyboardHelper.IsKeyPressed(cfg.SilentAimProp.Key))
            {
                G.Local.SetForward(4);
                G.Local.SetBackward(4);
                G.Local.SetLeft(0);
                G.Local.SetRight(0);
                _autoStopWorking = true;
            }
            else
            {
                if (_autoStopWorking)
                {
                    if (KeyboardHelper.IsKeyPressed(Keys.W))
                    {
                        G.Local.SetForward(5);
                    }

                    if (KeyboardHelper.IsKeyPressed(Keys.S))
                    {
                        G.Local.SetBackward(5);
                    }

                    if (KeyboardHelper.IsKeyPressed(Keys.A))
                    {
                        G.Local.SetLeft(1);
                    }

                    if (KeyboardHelper.IsKeyPressed(Keys.D))
                    {
                        G.Local.SetRight(1);
                    }

                    _autoStopWorking = false;
                }
            }
        }

        public static void DoBombDamage()
        {
            if (!G.C.Misc.BombDamage)
            {
                return;
            }

            if (G.Bomb == null)
            {
                return;
            }

            if (!G.Bomb.GetIsBombTicking())
            {
                return;
            }

            var p = G.Local.GetObserverPlayer();
            var distance = FuncHelper.Distance(p.GetOrigin(), G.Bomb.GetOrigin());

            const float a = 450.7f;
            const float b = 75.68f;
            const float c = 789.2f;
            var d = (distance - b) / c;

            var damage = a * Math.Exp(-d * d);
            var armor = p.GetArmor();

            const float flArmorRatio = 0.5f;
            const float flArmorBonus = 0.5f;

            if (armor > 0)
            {
                var flNew = damage * flArmorRatio;
                var flArmor = (damage - flNew) * flArmorBonus;

                if (flArmor > armor)
                {
                    flArmor = armor * (1f / flArmorBonus);
                    flNew = damage - flArmor;
                }

                damage = flNew;
            }

            var healthNow = p.GetHealth();
            var healthAfter = (int) (healthNow - damage);

            var dead = healthAfter <= 0;

            var hp = dead ? "DEAD" : healthAfter.ToString();
            var color = dead ? G.C.Clr.TextWarning : G.C.Clr.Text;

            var output = $"HP : {hp}";
            var format = GetTextFormat(G.C.Misc.BombDamageProp.Size);

            DrawBackground(
                format,
                output,
                G.C.Misc.BombDamageProp.X,
                G.C.Misc.BombDamageProp.Y);

            DrawText(
                format,
                output,
                new RawVector2(
                    G.C.Misc.BombDamageProp.X,
                    G.C.Misc.BombDamageProp.Y),
                color.ToBrush());
        }

        public static void DoBrightness()
        {
            if (!G.C.Misc.Brightness)
            {
                return;
            }

            G.O.D.FillRectangle(
                new RawRectangleF(0f, 0f, G.ScreenParameters.Width, G.ScreenParameters.Height),
                new SolidColorBrush(G.O.D,
                    new RawColor4(1f, 1f, 1f, G.C.Misc.BrightnessProp.Strength / 100f)));
        }

        public static StrafingState Strafing;
        public static ViewAngles LastStrafeAngles;

        public static bool DoBunnyhop()
        {
            if (!G.C.Misc.Bunnyhop)
            {
                return false;
            }

            if (!G.Local.GetIsAlive())
            {
                Thread.Sleep(100);
                return false;
            }

            if (G.Local.GetVelocity(true) == default(Vector3))
            {
                Thread.Sleep(100);
                return false;
            }

            if (!KeyboardHelper.IsKeyPressed(G.C.Key.Bunnyhop))
            {
                return false;
            }

            if (!G.Local.GetIsOnGround(true))
            {
                if (G.C.Misc.BunnyhopProp.AutoStrafe)
                {
                    // AutoStrafe

                    var ang = G.Local.GetViewAngles();

                    if (ang.Yaw > LastStrafeAngles.Yaw)
                    {
                        G.Local.SetLeft(1);
                        G.Local.SetRight(0);
                        Strafing = StrafingState.Left;
                    }
                    else if (ang.Yaw < LastStrafeAngles.Yaw)
                    {
                        G.Local.SetLeft(0);
                        G.Local.SetRight(1);
                        Strafing = StrafingState.Right;
                    }

                    LastStrafeAngles = ang;
                }

                return true;
            }

            while (G.Local.GetIsOnGround(true))
            {
                G.Local.SetJump(5);
                Thread.Sleep(1);
            }

            G.Local.SetJump(4);

            return true;
        }

        public static void DoChams(Player p)
        {
            if (!G.C.Vis.Players.Chams)
            {
                return;
            }

            if (G.C.Vis.Players.ChamsProp.Health)
            {
                // Chams.Health

                Color maxColor;
                Color minColor;

                if (p.GetIsAlly())
                {
                    // Ally

                    maxColor = G.C.Clr.GlowAlliesMaxHealth;
                    minColor = G.C.Clr.GlowAlliesMinHealth;
                }
                else
                {
                    // Enemy

                    if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                    {
                        // Visible

                        minColor = G.C.Clr.GlowEnemiesMinHealthVisible;
                        maxColor = G.C.Clr.GlowEnemiesMaxHealthVisible;
                    }
                    else
                    {
                        // Hidden

                        minColor = G.C.Clr.GlowEnemiesMinHealth;
                        maxColor = G.C.Clr.GlowEnemiesMaxHealth;
                    }
                }

                var health = p.GetHealth();

                var a = health / 100f;
                var b = (100 - health) / 100f;

                var final = new ColorRender(
                    (byte) (maxColor.R * a +
                            minColor.R * b),
                    (byte) (maxColor.G * a +
                            minColor.G * b),
                    (byte) (maxColor.B * a +
                            minColor.B * b),
                    (byte) (maxColor.A * a +
                            minColor.A * b));

                p.SetRenderColor(final);
            }
            else
            {
                // Chams.Static

                Color color;

                if (p.GetIsAlly())
                {
                    // Ally

                    color = G.C.Clr.ChamsAllies;
                }
                else
                {
                    // Enemy

                    if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                    {
                        // Visible

                        color = G.C.Clr.ChamsEnemiesVisible;
                    }
                    else
                    {
                        // Hidden

                        color = G.C.Clr.ChamsEnemies;
                    }
                }

                p.SetRenderColor(color);
            }
        }

        public static void DoChams(Entity e)
        {
            if (!G.C.Vis.Entities.Chams)
            {
                return;
            }

            var color = e.GetIsImportant() ? G.C.Clr.ChamsEntitiesImportant : G.C.Clr.ChamsEntities;

            e.SetRenderColor(color);
        }

        private static int _chatInfoNext;

        public static void DoChatInfo()
        {
            if (!G.C.Misc.ChatInfo)
            {
                return;
            }

            if (_chatInfoNext > G.Now)
            {
                // Wait for new data
                if (!G.C.Misc.Spam)
                {
                    Thread.Sleep(100);
                }

                return;
            }

            var headerSent = false;
            var cmd = G.C.Misc.ChatInfoProp.SendAll ? "say" : "say_team";

            foreach (var p in ObjectHelper.GetPlayers())
            {
                if (p.Handle == IntPtr.Zero ||
                    p.Handle == G.Local.Handle)
                {
                    // Invalid
                    continue;
                }

                if (!p.GetIsAlive())
                {
                    // Dead
                    continue;
                }

                if (p.GetIsAlly())
                {
                    // Ally
                    continue;
                }

                string team;
                switch (p.GetTeam())
                {
                    case Team.Terrorist:
                        team = "T";
                        break;
                    case Team.CounterTerrorist:
                        team = "CT";
                        break;
                    default:
                        continue;
                }

                var health = p.GetHealth();
                var place = p.GetPlaceName();
                var weapName = p.GetActiveWeapon().GetName();

                var output = $"{team}({health}) at {place} w/ {weapName}";

                if (!headerSent)
                {
                    G.Local.SendClientCmd($"{cmd} -=: Chat Info ({DateTime.Now:HH:mm:ss}) :=-");
                    Thread.Sleep(800);

                    headerSent = true;
                }

                G.Local.SendClientCmd($"{cmd} " + output);

                Thread.Sleep(800);
            }

            // New data received
            if (headerSent)
            {
                G.Local.SendClientCmd($"{cmd} -=: {new string('-', 21)} :=-");
                Thread.Sleep(800);

                _chatInfoNext = G.Now + 5000;
            }
        }

        private static int _fakeLagLastStart;

        public static void DoFakeLag()
        {
            //if (!G.C.Misc.FakeLag)s
            //    return;

            var now = Environment.TickCount;

            var stop = _fakeLagLastStart + G.C.Misc.FakeLagProp.Length;
            var next = stop + G.C.Misc.FakeLagProp.Pause;

            var cfg = FuncHelper.GetWeaponConfig();

            if (KeyboardHelper.IsKeyPressed(Keys.LButton) ||
                KeyboardHelper.IsKeyPressed(cfg.TriggerbotProp.Key) ||
                KeyboardHelper.IsKeyPressed(cfg.TriggerbotWithAimbotKey) ||
                KeyboardHelper.IsKeyPressed(cfg.SilentAimProp.Key) ||
                !G.Local.GetIsAlive())
            {
                // force pause
                G.Local.SetSendPackets(true);
                return;
            }

            if (stop > now)
            {
                // lag
                G.Local.SetSendPackets(false);
                return;
            }

            if (next > now)
            {
                // pause
                G.Local.SetSendPackets(true);
                return;
            }

            _fakeLagLastStart = now;
        }

        private static int _lastAmmoFastReload;
        private static IntPtr _lastWeapFastReload;

        public static void DoFastReload()
        {
            if (!G.C.Misc.FastReload)
            {
                return;
            }

            var weap = G.Local.GetActiveWeapon();
            var ammo = weap.GetAmmo();
            var handle = weap.GetHandle();

            if (ammo > _lastAmmoFastReload + 2 &&
                _lastWeapFastReload == handle)
            {
                var thr = new Thread(() =>
                {
                    G.Local.SendClientCmd("lastinv");
                    Thread.Sleep(50);
                    G.Local.SendClientCmd("lastinv");
                }) {IsBackground = true};
                thr.Start();
            }

            _lastAmmoFastReload = ammo;
            _lastWeapFastReload = handle;
        }

        private static int _lastAmmoFastSnipers;
        private static IntPtr _lastWeapFastSnipers;

        public static void DoFastSnipers()
        {
            if (!G.C.Misc.FastSnipers)
            {
                return;
            }

            var weap = G.Local.GetActiveWeapon();
            var ammo = weap.GetAmmo();
            var handle = weap.GetHandle();
            var id = weap.GetId();

            if (id.GetCategory() == WeaponCategory.Sniper &&
                id != WeaponId.SCAR20 &&
                id != WeaponId.G3SG1 &&
                ammo < _lastAmmoFastSnipers &&
                _lastWeapFastSnipers == handle)
            {
                var thr = new Thread(() =>
                    {
                        G.Local.SendClientCmd("lastinv");
                        Thread.Sleep(50);
                        G.Local.SendClientCmd("lastinv");
                    })
                    { IsBackground = true };
                thr.Start();
            }

            _lastAmmoFastSnipers = ammo;
            _lastWeapFastSnipers = handle;
        }

        public static void DoFloor(Player p)
        {
            if (!G.C.Vis.Players.Floor)
            {
                return;
            }

            const float size = 25f;

            var origin = p.GetOrigin();
            var rotation = p.GetRotation();

            var p1 = origin;
            var p2 = origin;
            var p3 = origin;
            var p4 = origin;

            p1.X += size;
            p1.Y += size;
            p1 = FuncHelper.RotatePoint(p1, origin, rotation);
            var p1Raw = FuncHelper.WorldToScreen(p1);
            if (!p1Raw.IsValid())
            {
                return;
            }

            p2.X += size;
            p2.Y -= size;
            p2 = FuncHelper.RotatePoint(p2, origin, rotation);
            var p2Raw = FuncHelper.WorldToScreen(p2);
            if (!p2Raw.IsValid())
            {
                return;
            }

            p3.X -= size;
            p3.Y -= size;
            p3 = FuncHelper.RotatePoint(p3, origin, rotation);
            var p3Raw = FuncHelper.WorldToScreen(p3);
            if (!p3Raw.IsValid())
            {
                return;
            }

            p4.X -= size;
            p4.Y += size;
            p4 = FuncHelper.RotatePoint(p4, origin, rotation);
            var p4Raw = FuncHelper.WorldToScreen(p4);
            if (!p4Raw.IsValid())
            {
                return;
            }

            SolidColorBrush brush;

            if (p.GetIsAlly())
            {
                // Ally

                brush = G.C.Clr.EspAllies.ToBrush();
            }
            else
            {
                // Enemy

                if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                {
                    // Visible

                    brush = G.C.Clr.EspEnemiesVisible.ToBrush();
                }
                else
                {
                    // Hidden

                    brush = G.C.Clr.EspEnemies.ToBrush();
                }
            }

            G.O.D.DrawLine(p1Raw, p2Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p2Raw, p3Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p3Raw, p4Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(p4Raw, p1Raw, BackgroundBrush, 2f);

            G.O.D.DrawLine(p1Raw, p2Raw, brush, 1f);
            G.O.D.DrawLine(p2Raw, p3Raw, brush, 1f);
            G.O.D.DrawLine(p3Raw, p4Raw, brush, 1f);
            G.O.D.DrawLine(p4Raw, p1Raw, brush, 1f);
        }

        public static void DoForceUpdate()
        {
            while (KeyboardHelper.IsKeyPressed(G.C.Key.ForceUpdate))
            {
                G.Local.SetForceFullUpdate();
                Thread.Sleep(3);
            }
        }

        public static void DoFovChanger()
        {
            if (!G.C.Misc.FovChanger)
            {
                return;
            }

            if (G.Local.GetFov() == G.C.Misc.FovChangerProp.Value)
            {
                return;
            }

            if (G.Local.GetIsScoped())
            {
                if (G.Local.GetFov() != 90)
                {
                    G.Local.SetFov(90);
                }

                return;
            }

            G.Local.SetFov(G.C.Misc.FovChangerProp.Value);
        }

        public static void DoHealth(Player p, DrawData d)
        {
            if (!G.C.Vis.Players.Health)
            {
                return;
            }

            d.Y -= 1.5f;
            d.Height += 3f;

            var size = GetSize(p);
            var margin = size * .1f;
            var width = size * .2f;

            var height = p.GetHealth() * d.Height / 100f;

            SolidColorBrush brush;

            if (p.GetIsAlly())
            {
                // Ally

                brush = G.C.Clr.HealthAllies.ToBrush();
            }
            else
            {
                // Enemy

                if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                {
                    // Visible

                    brush = G.C.Clr.HealthEnemiesVisible.ToBrush();
                }
                else
                {
                    // Hidden

                    brush = G.C.Clr.HealthEnemies.ToBrush();
                }
            }

            // Semi-Background
            var tmp = BackgroundBrush;
            tmp.Opacity = .30f;
            G.O.D.FillRectangle(new RawRectangleF(d.X + d.Width + margin,
                d.Y,
                d.X + d.Width + margin + width,
                d.Y + d.Height), tmp);
            tmp.Opacity = 1;

            // Top background
            G.O.D.DrawLine(new RawVector2(d.X + d.Width + margin,
                    d.Y + d.Height - height - .25f),
                new RawVector2(d.X + d.Width + margin + width,
                    d.Y + d.Height - height - .25f), BackgroundBrush, .5f);

            // Bar
            G.O.D.FillRectangle(new RawRectangleF(
                d.X + d.Width + margin,
                d.Y + d.Height - height,
                d.X + d.Width + margin + width,
                d.Y + d.Height), brush);

            // Background
            G.O.D.DrawRectangle(new RawRectangleF(
                d.X + d.Width + margin,
                d.Y,
                d.X + d.Width + margin + width,
                d.Y + d.Height), BackgroundBrush, .5f);
        }

        public static void DoKnifebot()
        {
            if (!G.C.Misc.Knifebot)
            {
                return;
            }

            var p = G.Local.GetCrosshairPlayer();

            if (p.Index == -1 || !p.GetIsAlive())
            {
                return;
            }

            if (p.GetIsAlly())
            {
                return;
            }

            if (p.GetHasImmunity())
            {
                return;
            }

            if (G.Local.GetActiveWeapon().GetCategory() != WeaponCategory.Knife)
            {
                return;
            }

            if (!G.Local.GetIsAbleToShoot(includeKnife: true))
            {
                return;
            }

            var localRot = G.Local.GetRotation();
            var targetRot = p.GetRotation();
            var delta = FuncHelper.DistanceViewAngle(localRot, targetRot);
            var backstab = delta < 60;

            var dst = FuncHelper.Distance(p.GetOrigin(), G.Local.GetOrigin());
            if (dst <= 64f)
            {
                if (backstab)
                {
                    G.Local.SetAttack2(6);
                }
                else
                {
                    var health = p.GetHealth();
                    if (health <= 21f)
                    {
                        G.Local.SetAttack1(6);
                        return;
                    }
                    if (health <= 55f)
                    {
                        G.Local.SetAttack2(6);
                        return;
                    }

                    G.Local.SetAttack1(6);
                }

                return;
            }

            if (dst <= 82f && !backstab)
            {
                G.Local.SetAttack1(6);
            }
        }

        public static void DoMoveHelper()
        {
            if (!G.C.Misc.MoveHelper)
            {
                return;
            }

            var speed = FuncHelper.Distance(G.Local.GetVelocity(), default(Vector3));

            if (speed == 0f)
            {
                return;
            }

            var size = speed * .1f;

            var ellipse = new Ellipse(G.ScreenParameters.Center, size, size);

            G.O.D.DrawEllipse(
                ellipse,
                G.C.Clr.Background.ToBrush(),
                2.5f);

            G.O.D.DrawEllipse(
                ellipse, 
                G.C.Clr.MoveHelper.ToBrush(),
                1.5f);
        }

        public static void DoName(Entity e)
        {
            if (!G.C.Vis.Entities.Name)
            {
                return;
            }

            var screenOrigin = e.GetScreenOrigin();
            if (!screenOrigin.IsValid())
            {
                // Not on screen
                return;
            }

            var size = GetSize(e, true);
            screenOrigin.Y += size / 3f;

            DrawText(
                GetTextFormat(size),
                e.GetClassId().GetName(),
                screenOrigin,
                G.C.Clr.Text.ToBrush());
        }

        public static void DoName(Player p, DrawData d)
        {
            if (!G.C.Vis.Players.Name)
            {
                return;
            }

            var size = GetSize(p);

            DrawText(
                GetTextFormat(size),
                p.GetName(),
                new RawVector2(
                    d.X + d.Width / 2f,
                    d.Y - size * 1.3f),
                G.C.Clr.Text.ToBrush());
        }

        public static void DoNoFlash()
        {
            if (!G.C.Misc.NoFlash)
            {
                return;
            }

            if (G.Local.GetFlashDuration() == 0f)
            {
                return;
            }

            G.Local.SetFlashDuration(0f);
        }

        public static void DoNoHands()
        {
            if (!G.C.Misc.NoHands)
            {
                return;
            }

            G.Local.SetModelIndex(0);
        }

        private static IntPtr _lastNoShakeValue;

        public static void DoNoShake()
        {
            var noShakeValue = Base.m_dwNoShake - 0x30;

            if (!G.C.Misc.NoShake)
            {
                if (_lastNoShakeValue != default(IntPtr) && G.Mem.Read<IntPtr>(Base.m_dwNoShake) == noShakeValue)
                {
                    G.Mem.Write(Base.m_dwNoShake, _lastNoShakeValue);
                }
            }
            else
            {
                if (G.Mem.Read<IntPtr>(Base.m_dwNoShake) != noShakeValue)
                {
                    _lastNoShakeValue = G.Mem.Read<IntPtr>(Base.m_dwNoShake);
                    G.Mem.Write(Base.m_dwNoShake, noShakeValue);
                }
            }
        }

        public static void DoNoSmoke()
        {
            if (!G.C.Misc.NoSmoke)
            {
                return;
            }

            foreach(var e in ObjectHelper.GetEntities())
            {
                if (e.Handle == IntPtr.Zero)
                {
                    // Invalid
                    continue;
                }

                if (e.GetClassId() == ClassId.CSmokeGrenadeProjectile &&
                    !e.GetHasOwner(true))
                {
                    G.Mem.Write(e.Handle + Offset.m_vecOrigin, default(Vector3));
                }
            }
        }

        public static void DoNoViewModel()
        {
            if (!G.C.Misc.NoViewModel)
            {
                return;
            }

            var addr = G.Local.Handle + Offset.m_hViewModel;

            if (!float.IsNaN(G.Mem.Read<float>(addr)))
            {
                G.Mem.Write(addr, float.NaN);
            }
        }

        public static void DoRadar(Entity e)
        {
            if (!G.C.Misc.Radar)
            {
                return;
            }

            e.SetSpotted(true);
        }

        public static void DoRadar(Player p)
        {
            if (!G.C.Misc.Radar)
            {
                return;
            }

            p.SetSpotted(true);
        }

        public static void DoRank(Player p, DrawData d)
        {
            if (!G.C.Vis.Players.Rank)
            {
                return;
            }

            var size = GetSize(p);
            var heightFix = G.C.Vis.Players.Name ? size : 0f;

            DrawText(
                GetTextFormat(size),
                p.GetRankId().GetName(),
                new RawVector2(
                    d.X + d.Width / 2f,
                    d.Y - size * 1.3f - heightFix),
                G.C.Clr.Text.ToBrush());
        }

        public static void DoRecoilCrosshair()
        {
            if (!G.C.Vis.RecoilCrosshair)
            {
                return;
            }

            const int size = 5;

            var fovFixed = G.Local.GetFov();

            if (fovFixed == 0)
            {
                fovFixed = 1;
            }

            var center = G.ScreenParameters.Center;
            var percentage = center.Y / fovFixed;

            var punch = G.Local.GetPunch() * 2f * percentage * G.Local.GetScopeScale();

            center.X -= punch.Yaw;
            center.Y += punch.Pitch;

            var brush = G.C.Clr.RecoilCrosshair.ToBrush();

            G.O.D.DrawLine(
                new RawVector2(center.X - 1f, center.Y - size),
                new RawVector2(center.X + 1f, center.Y - size),
                BackgroundBrush, 1f);
            G.O.D.DrawLine(
                new RawVector2(center.X - 1f, center.Y - size - 1f),
                new RawVector2(center.X - 1f, center.Y + size + 1f),
                BackgroundBrush, 1f);
            G.O.D.DrawLine(
                new RawVector2(center.X - 1f, center.Y + size),
                new RawVector2(center.X + 1f, center.Y + size),
                BackgroundBrush, 1f);
            G.O.D.DrawLine(
                new RawVector2(center.X - size, center.Y - 1f),
                new RawVector2(center.X - size, center.Y + 1f),
                BackgroundBrush, 1f);
            G.O.D.DrawLine(
                new RawVector2(center.X - size - 1f, center.Y - 1f),
                new RawVector2(center.X + size + 1f, center.Y - 1f),
                BackgroundBrush, 1f);
            G.O.D.DrawLine(
                new RawVector2(center.X - size - 1f, center.Y + 1f),
                new RawVector2(center.X + size + 1f, center.Y + 1f),
                BackgroundBrush, 1f);
            G.O.D.DrawLine(
                new RawVector2(center.X + 1f, center.Y - size - 1f),
                new RawVector2(center.X + 1f, center.Y + size + 1f),
                BackgroundBrush, 1f);
            G.O.D.DrawLine(
                new RawVector2(center.X + size, center.Y - 1f),
                new RawVector2(center.X + size, center.Y + 1f),
                BackgroundBrush, 1f);

            G.O.D.DrawLine(
                new RawVector2(center.X, center.Y - size),
                new RawVector2(center.X, center.Y + size),
                brush, 2f);
            G.O.D.DrawLine(
                new RawVector2(center.X - size, center.Y),
                new RawVector2(center.X + size, center.Y),
                brush, 2f);
        }

        public static void DoRcs(WeaponProperties cfg)
        {
            var punch = G.Local.GetPunch(true);

            punch.Pitch *= cfg.RcsProp.PitchStrength / 50f;
            punch.Yaw *= cfg.RcsProp.YawStrength / 50f;

            var punchDelta = punch - _rcsLastPunch;

            // Smooth
            punchDelta *= (100 - cfg.RcsProp.Smooth) / 100f;

            // Check delta
            if (Math.Abs(punchDelta.Pitch) < 2.8f)
            {
                if (!cfg.RcsProp.AimbotOnly && ShouldDoRcs(cfg, false, true))
                {
                    var angles = G.Local.GetViewAngles(true);
                    G.Local.SetViewAngles(angles - punchDelta);

                    // Adding delta because of smooth
                    _rcsLastPunch += punchDelta;
                }
                else
                {
                    _rcsLastPunch = punch;
                }
            }
        }

        public static void DoRotation(Player p)
        {
            if (!G.C.Vis.Players.RotationArrow)
            {
                return;
            }

            var origin = p.GetOrigin();
            var rotation = p.GetRotation();

            var p1 = origin;
            var p2 = origin;
            var p3 = origin;
            var p4 = origin;
            var p5 = origin;

            p1.X += 45f;
            p2.X += 20f;
            p2.Y += 5f;
            p3.Y -= 5f;
            p4.Y += 2f;
            p5.Y -= 2f;

            p3.X = p2.X;
            p4.X = p2.X;
            p5.X = p2.X;

            p1 = FuncHelper.RotatePoint(p1, origin, rotation);
            var p1Raw = FuncHelper.WorldToScreen(p1);
            if (!p1Raw.IsValid())
            {
                return;
            }

            p2 = FuncHelper.RotatePoint(p2, origin, rotation);
            var p2Raw = FuncHelper.WorldToScreen(p2);
            if (!p2Raw.IsValid())
            {
                return;
            }

            p3 = FuncHelper.RotatePoint(p3, origin, rotation);
            var p3Raw = FuncHelper.WorldToScreen(p3);
            if (!p3Raw.IsValid())
            {
                return;
            }

            p4 = FuncHelper.RotatePoint(p4, origin, rotation);
            var p4Raw = FuncHelper.WorldToScreen(p4);
            if (!p4Raw.IsValid())
            {
                return;
            }

            p5 = FuncHelper.RotatePoint(p5, origin, rotation);
            var p5Raw = FuncHelper.WorldToScreen(p5);
            if (!p5Raw.IsValid())
            {
                return;
            }

            var brush = G.C.Clr.Rotation.ToBrush();

            G.O.D.DrawLine(p1Raw, p2Raw, BackgroundBrush, 3f);
            G.O.D.DrawLine(p2Raw, p3Raw, BackgroundBrush, 3f);
            G.O.D.DrawLine(p3Raw, p1Raw, BackgroundBrush, 3f);
            G.O.D.DrawLine(p4Raw, p1Raw, BackgroundBrush, 3f);
            G.O.D.DrawLine(p5Raw, p1Raw, BackgroundBrush, 3f);

            G.O.D.DrawLine(p1Raw, p2Raw, brush, 2f);
            G.O.D.DrawLine(p2Raw, p3Raw, brush, 2f);
            G.O.D.DrawLine(p3Raw, p1Raw, brush, 2f);
            G.O.D.DrawLine(p4Raw, p1Raw, brush, 2f);
            G.O.D.DrawLine(p5Raw, p1Raw, brush, 2f);
        }

        public static void DoSilentAim(AimTarget aim, WeaponProperties cfg)
        {
            if (!cfg.SilentAim)
            {
                return;
            }

            if (!aim.Selected)
            {
                return;
            }

            if (!G.Local.GetIsMouseEnable())
            {
                return;
            }

            if (!G.Local.GetIsAbleToShoot())
            {
                return;
            }

            if (G.C.Misc.FakeLag)
            {
                return;
            }

            G.Local.SetViewAnglesSilent(aim.ViewAngles - G.Local.GetPunch() * 2f);
        }

        public static void DoSilentAimFov(WeaponProperties cfg)
        {
            if (!G.C.Vis.SilentAimFov)
            {
                return;
            }

            if (!cfg.SilentAim)
            {
                return;
            }

            if (!G.Local.GetCanShoot())
            {
                return;
            }

            var fovFixed = G.Local.GetFov();
            if (fovFixed == 0)
            {
                fovFixed = 1;
            }

            var percentage = G.ScreenParameters.Height / fovFixed;
            var size = percentage * cfg.SilentAimProp.Fov * G.Local.GetScopeScale();

            var ellipse = new Ellipse(G.ScreenParameters.Center, size, size);

            G.O.D.DrawEllipse(ellipse, BackgroundBrush, 2.5f);
            G.O.D.DrawEllipse(ellipse, G.C.Clr.SilentAimFov.ToBrush(), 1.5f);
        }

        public static void DoSkeleton(Player p, Model model)
        {
            if (!G.C.Vis.Players.Skeleton)
            {
                return;
            }

            var headRaw = model.GetScreenBoneOrigin(Bone.Head);
            if (!headRaw.IsValid())
            {
                return;
            }
            var neckRaw = model.GetScreenBoneOrigin(Bone.Neck);
            if (!neckRaw.IsValid())
            {
                return;
            }
            var chestRaw = model.GetScreenBoneOrigin(Bone.Chest);
            if (!chestRaw.IsValid())
            {
                return;
            }
            var stomachRaw = model.GetScreenBoneOrigin(Bone.Stomach);
            if (!stomachRaw.IsValid())
            {
                return;
            }

            var ra1Raw = model.GetScreenBoneOrigin((Bone) 4);
            if (!ra1Raw.IsValid())
            {
                return;
            }
            var ra2Raw = model.GetScreenBoneOrigin((Bone) 5);
            if (!ra2Raw.IsValid())
            {
                return;
            }
            var ra3Raw = model.GetScreenBoneOrigin((Bone) 6);
            if (!ra3Raw.IsValid())
            {
                return;
            }
            var la1Raw = model.GetScreenBoneOrigin((Bone) 7);
            if (!la1Raw.IsValid())
            {
                return;
            }
            var la2Raw = model.GetScreenBoneOrigin((Bone) 8);
            if (!la2Raw.IsValid())
            {
                return;
            }
            var la3Raw = model.GetScreenBoneOrigin((Bone) 9);
            if (!la3Raw.IsValid())
            {
                return;
            }
            var rl1Raw = model.GetScreenBoneOrigin((Bone) 10);
            if (!rl1Raw.IsValid())
            {
                return;
            }
            var rl2Raw = model.GetScreenBoneOrigin((Bone) 11);
            if (!rl2Raw.IsValid())
            {
                return;
            }
            var rl3Raw = model.GetScreenBoneOrigin((Bone) 12);
            if (!rl3Raw.IsValid())
            {
                return;
            }
            var ll1Raw = model.GetScreenBoneOrigin((Bone) 13);
            if (!ll1Raw.IsValid())
            {
                return;
            }
            var ll2Raw = model.GetScreenBoneOrigin((Bone) 14);
            if (!ll2Raw.IsValid())
            {
                return;
            }
            var ll3Raw = model.GetScreenBoneOrigin((Bone) 15);
            if (!ll3Raw.IsValid())
            {
                return;
            }

            SolidColorBrush brush;
            SolidColorBrush fill;

            if (p.GetIsAlly())
            {
                // Ally

                brush = G.C.Clr.SkeletonAllies.ToBrush();
                fill = G.C.Clr.SkeletonFillAllies.ToBrush();
            }
            else
            {
                // Enemy

                if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                {
                    // Visible

                    brush = G.C.Clr.SkeletonEnemiesVisible.ToBrush();
                    fill = G.C.Clr.SkeletonFillEnemiesVisible.ToBrush();
                }
                else
                {
                    // Hidden

                    brush = G.C.Clr.SkeletonEnemies.ToBrush();
                    fill = G.C.Clr.SkeletonFillEnemies.ToBrush();
                }
            }

            var headRadius = FuncHelper.Distance(headRaw, neckRaw);

            G.O.D.DrawLine(chestRaw, stomachRaw, BackgroundBrush, 2f);
            G.O.D.DrawLine(la1Raw, la2Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(la2Raw, la3Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(ll1Raw, ll2Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(ll2Raw, ll3Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(neckRaw, chestRaw, BackgroundBrush, 2f);
            G.O.D.DrawLine(neckRaw, la1Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(neckRaw, ra1Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(ra1Raw, ra2Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(ra2Raw, ra3Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(rl1Raw, rl2Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(rl2Raw, rl3Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(stomachRaw, ll1Raw, BackgroundBrush, 2f);
            G.O.D.DrawLine(stomachRaw, rl1Raw, BackgroundBrush, 2f);

            if (G.C.Vis.Players.SkeletonProp.Fill)
            {
                var incHeadRadius = headRadius + 1f;
                G.O.D.FillEllipse(new Ellipse(headRaw, incHeadRadius, incHeadRadius), BackgroundBrush);
            }
            else
            {
                G.O.D.DrawEllipse(new Ellipse(headRaw, headRadius, headRadius), BackgroundBrush, 2f);
            }

            G.O.D.DrawLine(chestRaw, stomachRaw, brush, 1f);
            G.O.D.DrawLine(la1Raw, la2Raw, brush, 1f);
            G.O.D.DrawLine(la2Raw, la3Raw, brush, 1f);
            G.O.D.DrawLine(ll1Raw, ll2Raw, brush, 1f);
            G.O.D.DrawLine(ll2Raw, ll3Raw, brush, 1f);
            G.O.D.DrawLine(neckRaw, chestRaw, brush, 1f);
            G.O.D.DrawLine(neckRaw, la1Raw, brush, 1f);
            G.O.D.DrawLine(neckRaw, ra1Raw, brush, 1f);
            G.O.D.DrawLine(ra1Raw, ra2Raw, brush, 1f);
            G.O.D.DrawLine(ra2Raw, ra3Raw, brush, 1f);
            G.O.D.DrawLine(rl1Raw, rl2Raw, brush, 1f);
            G.O.D.DrawLine(rl2Raw, rl3Raw, brush, 1f);
            G.O.D.DrawLine(stomachRaw, ll1Raw, brush, 1f);
            G.O.D.DrawLine(stomachRaw, rl1Raw, brush, 1f);

            if (G.C.Vis.Players.SkeletonProp.Fill)
            {
                G.O.D.FillEllipse(new Ellipse(headRaw, headRadius, headRadius), fill);
            }
            else
            {
                G.O.D.DrawEllipse(new Ellipse(headRaw, headRadius, headRadius), brush, 1f);
            }
        }

        private static readonly RawVector2 SnaplinesScreenCenter =
            new RawVector2(
                G.ScreenParameters.Width / 2f, 
                G.ScreenParameters.Height);

        public static void DoSnaplines(Entity e)
        {
            if (!G.C.Vis.Entities.Snaplines)
            {
                return;
            }

            if (!G.Local.GetIsAlive())
            {
                return;
            }

            var screenOrigin = e.GetScreenOrigin();
            if (!screenOrigin.IsValid())
            {
                // Not on screen
                return;
            }

            var color = e.GetIsImportant() ? G.C.Clr.EspEntitiesImportant : G.C.Clr.EspEntities;

            G.O.D.DrawLine(SnaplinesScreenCenter, screenOrigin, BackgroundBrush, 3f);
            G.O.D.DrawLine(SnaplinesScreenCenter, screenOrigin, color.ToBrush(), 2f);
        }

        public static void DoSnaplines(Player p)
        {
            if (!G.C.Vis.Players.Snaplines)
            {
                return;
            }

            if (!G.Local.GetIsAlive())
            {
                return;
            }

            var screenOrigin = p.GetScreenOrigin();
            if (!screenOrigin.IsValid())
            {
                // Not on screen
                return;
            }

            SolidColorBrush brush;

            if (p.GetIsAlly())
            {
                // Ally

                brush = G.C.Clr.EspAllies.ToBrush();
            }
            else
            {
                // Enemy

                if (G.C.Vis.Players.VisibilityCheck && p.GetIsVisible())
                {
                    // Visible

                    brush = G.C.Clr.EspEnemiesVisible.ToBrush();
                }
                else
                {
                    // Hidden

                    brush = G.C.Clr.EspEnemies.ToBrush();
                }
            }

            G.O.D.DrawLine(SnaplinesScreenCenter, screenOrigin, BackgroundBrush, 3);
            G.O.D.DrawLine(SnaplinesScreenCenter, screenOrigin, brush, 2);
        }

        public static void DoSpam()
        {
            if (!G.C.Misc.Spam)
            {
                return;
            }

            var cmd = G.C.Misc.SpamProp.SendAll ? "say" : "say_team";

            G.Local.SendClientCmd($"{cmd} " + G.C.Misc.SpamProp.Message);

            Thread.Sleep(800);
        }

        public static void DoSpectatorList()
        {
            if (!G.C.Misc.SpectatorList)
            {
                return;
            }

            var spectatorList = new List<string>();

            var targetIndex = G.Local.GetIsAlive() ? 
                G.Local.Index : 
                G.Local.GetObserverTargetIndex();

            foreach (var p in ObjectHelper.GetPlayers())
            {
                if (p.Handle == IntPtr.Zero ||
                    p.Handle == G.Local.Handle)
                {
                    continue;
                }

                if (p.GetIsAlive())
                {
                    continue;
                }

                var observerIndex = p.GetObserverTargetIndex();

                if (targetIndex != observerIndex ||
                    p.GetObserverMode() == ObserverMode.FreeCam)
                {
                    // Not spectating
                    continue;
                }

                var name = p.GetName();
                var niceName = name.Replace("\r", string.Empty).Replace("\n", string.Empty);

                if (niceName.StartsWith("GOTV"))
                {
                    // Skip GOTV bot
                    continue;
                }

                spectatorList.Add(niceName);
            }

            if (spectatorList.Count == 0)
            {
                return;
            }

            spectatorList.Sort();

            var spectators = spectatorList
                .Aggregate("SPECTATORS:\n", (current, spectator) => current + (spectator + "\n"))
                .TrimEnd('\n');
            var format = GetTextFormat(G.C.Misc.SpectatorListProp.Size);

            DrawBackground(
                format,
                spectators,
                G.C.Misc.SpectatorListProp.X,
                G.C.Misc.SpectatorListProp.Y);

            DrawText(
                format,
                spectators,
                new RawVector2(
                    G.C.Misc.SpectatorListProp.X,
                    G.C.Misc.SpectatorListProp.Y),
                G.C.Clr.Text.ToBrush());
        }

        public static void DoThirdPersonMode()
        {
            var mode = G.Local.GetThirdPersonMode();

            if (mode == 0 && G.C.Misc.ThirdPersonMode)
            {
                G.Local.SetThirdPersonMode(1);
            }
            else if (mode == 1 && !G.C.Misc.ThirdPersonMode)
            {
                G.Local.SetThirdPersonMode(0);
            }
        }

        private static int _triggerbotNextShoot;
        private static int _triggerbotLastShoot;

        public static async void DoTriggerbot(WeaponProperties cfg)
        {
            if (!ShouldDoTriggerbot(cfg))
            {
                return;
            }

            if (_triggerbotNextShoot > G.Now)
            {
                return;
            }

            var p = G.Local.GetCrosshairPlayer();
            if (p.Index != -1 && p.GetOrigin() != default(Vector3) && p.GetIsAlive())
            {
                _triggerbotLastShoot = G.Now + cfg.TriggerbotProp.BeforeDelay;
            }

            _triggerbotNextShoot = _triggerbotLastShoot + cfg.TriggerbotProp.ShootDelay;

            await Task.Delay(cfg.TriggerbotProp.BeforeDelay);

            G.Local.SetAttack1(6);
        }

        private static bool ShouldDoRcs(WeaponProperties cfg, bool positionFix, bool allow1Tap)
        {
            if (!cfg.Rcs)
            {
                return false;
            }

            if (!G.Local.GetIsMouseEnable())
            {
                return false;
            }

            var id = G.Local.GetActiveWeapon().GetId();

            if (id.GetIsRcsRestricted())
            {
                return false;
            }

            if (!id.GetCanShoot())
            {
                return false;
            }

            if (positionFix && _rcsLastPunch != default(ViewAngles))
            {
                return true;
            }

            var isPistol = id.GetCategory() == WeaponCategory.Pistol;
            var punch = G.Local.GetPunch();

            if (!isPistol && punch == default(ViewAngles))
            {
                return false;
            }

            if (isPistol && Math.Abs(punch.Pitch) < 0.1f)
            {
                return false;
            }

            if (G.Local.GetShotsFired() < 2 && ShouldDoTriggerbot(cfg))
            {
                return true;
            }

            if (allow1Tap && G.Local.GetShotsFired() < 2 && !isPistol)
            {
                return false;
            }

            return true;
        }

        private static bool ShouldDoTriggerbot(WeaponProperties cfg)
        {
            if (!cfg.Triggerbot)
            {
                return false;
            }

            if (cfg.TriggerbotProp.Key != Keys.F23 &&
                cfg.TriggerbotWithAimbotKey != Keys.F23 &&
                !KeyboardHelper.IsKeyPressed(cfg.TriggerbotProp.Key) &&
                !KeyboardHelper.IsKeyPressed(cfg.TriggerbotWithAimbotKey))
            {
                return false;
            }

            if (!G.Local.GetIsMouseEnable())
            {
                return false;
            }

            if (KeyboardHelper.IsKeyPressed(Keys.Tab))
            {
                return false;
            }

            var isTaser = G.Local.GetActiveWeapon().GetId() == WeaponId.Taser;

            if (!isTaser && !G.Local.GetIsAbleToShoot())
            {
                return false;
            }

            var p = G.Local.GetCrosshairPlayer();

            if (p.Index == -1 || !p.GetIsAlive())
            {
                return _triggerbotNextShoot > G.Now;
            }

            if (p.GetIsAlly())
            {
                return false;
            }

            if (p.GetHasImmunity())
            {
                return false;
            }

            if (isTaser && FuncHelper.Distance(G.Local.GetOrigin(), p.GetOrigin()) > 225)
            {
                return false;
            }

            return true;
        }
    }
}