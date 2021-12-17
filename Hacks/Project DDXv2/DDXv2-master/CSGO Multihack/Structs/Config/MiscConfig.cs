namespace DDX.Structs.Config
{
    public struct BombDamageProperties
    {
        public int Size;
        public int X;
        public int Y;
    }

    public struct BrightnessProperties
    {
        public int Strength;
    }

    public struct BunnyhopProperties
    {
        public bool AutoStrafe;
    }

    public struct ChatInfoProperties
    {
        public bool SendAll;
    }

    public struct FakeLagProperties
    {
        public int Length;
        public int Pause;
    }

    public struct FovChangerProperties
    {
        public int Value;
    }

    public struct SpamProperties
    {
        public bool SendAll;
        public string Message;
    }

    public struct SpectatorListProperties
    {
        public int Size;
        public int X;
        public int Y;
    }

    public struct MiscConfig
    {
        public BombDamageProperties BombDamageProp;
        public BrightnessProperties BrightnessProp;
        public BunnyhopProperties BunnyhopProp;
        public ChatInfoProperties ChatInfoProp;
        public FakeLagProperties FakeLagProp;
        public FovChangerProperties FovChangerProp;
        public SpamProperties SpamProp;
        public SpectatorListProperties SpectatorListProp;

        public bool AutoDuck;
        public bool AutoPistol;
        public bool AutoStop;
        public bool BombDamage;
        public bool Brightness;
        public bool Bunnyhop;
        public bool ChatInfo;
        public bool FakeLag;
        public bool FastReload;
        public bool FastSnipers;
        public bool FovChanger;
        public bool Knifebot;
        public bool MoveHelper;
        public bool NoFlash;
        public bool NoHands;
        public bool NoShake;
        public bool NoSmoke;
        public bool NoViewModel;
        public bool Radar;
        public bool Spam;
        public bool SpectatorList;
        public bool ThirdPersonMode;
    }
}
