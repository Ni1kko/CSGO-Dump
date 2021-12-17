using System;
using DDX.Functions;

namespace DDX.Objects
{
    public class Bomb : Entity
    {
        public Bomb(int index, IntPtr handle) : base(index, handle)
        { }

        private ulong _lastBlowSecondsRef;
        private float _lastBlowSeconds;
        public float GetBlowSeconds(bool force = false)
        {
            if (!force && _lastBlowSecondsRef == G.Ref)
                return _lastBlowSeconds;

            _lastBlowSecondsRef = G.Ref;
            return _lastBlowSeconds = GetBlowTime() - GlobalRead.GetCurrentTime();
        }

        private ulong _lastBlowTimeRef;
        private float _lastBlowTime;
        public float GetBlowTime(bool force = false)
        {
            if (!force && _lastBlowTimeRef == G.Ref)
                return _lastBlowTime;

            _lastBlowTimeRef = G.Ref;
            return _lastBlowTime = G.Mem.Read<float>(G.Bomb.Handle + Offset.m_flC4Blow);
        }

        private ulong _lastDefuseLengthRef;
        private float _lastDefuseLength;
        public float GetDefuseLength(bool force = false)
        {
            if (!force && _lastDefuseLengthRef == G.Ref)
                return _lastDefuseLength;

            _lastDefuseLengthRef = G.Ref;
            return _lastDefuseLength = G.Mem.Read<float>(G.Bomb.Handle + Offset.m_flDefuseLength);
        }

        private ulong _lastDefuseSecondsRef;
        private float _lastDefuseSeconds;
        public float GetDefuseSeconds(bool force = false)
        {
            if (!force && _lastDefuseSecondsRef == G.Ref)
                return _lastDefuseSeconds;

            _lastDefuseSecondsRef = G.Ref;
            return _lastDefuseSeconds = GetDefuseTime() - GlobalRead.GetCurrentTime();
        }

        private ulong _lastDefuseTimeRef;
        private float _lastDefuseTime;
        public float GetDefuseTime(bool force = false)
        {
            if (!force && _lastDefuseTimeRef == G.Ref)
                return _lastDefuseTime;

            _lastDefuseTimeRef = G.Ref;
            return _lastDefuseTime = G.Mem.Read<float>(G.Bomb.Handle + Offset.m_flDefuseCountDown);
        }

        private ulong _lastIsBombTickingRef;
        private bool _lastIsBombTicking;
        public bool GetIsBombTicking(bool force = false)
        {
            if (!force && _lastIsBombTickingRef == G.Ref)
                return _lastIsBombTicking;

            _lastIsBombTickingRef = G.Ref;
            return _lastIsBombTicking = G.Mem.Read<byte>(Handle + Offset.m_bBombTicking) == 1;
        }
    }
}
