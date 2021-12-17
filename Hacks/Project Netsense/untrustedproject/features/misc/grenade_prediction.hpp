#pragma once


class c_nadepoint {
public:
    c_nadepoint() {
        m_valid = false;
    }

    c_nadepoint(Vector start, Vector end, bool plane, bool valid, Vector normal, bool detonate) {
        m_start = start;
        m_end = end;
        m_plane = plane;
        m_valid = valid;
        m_normal = normal;
        m_detonate = detonate;
    }

    Vector m_start, m_end, m_normal;
    bool m_valid, m_plane, m_detonate;
};

enum nade_throw_act {
    ACT_NONE,
    ACT_THROW,
    ACT_LOB,
    ACT_DROP
};

class c_nade_prediction {
    std::array< c_nadepoint, 500 >    _points{ };
    bool         _predicted = false;

    void predict(CUserCmd* user_cmd) noexcept;
    bool detonated(C_BaseCombatWeapon* weapon, float time, trace_t& trace) noexcept;
public:

    void trace(CUserCmd* user_cmd) noexcept;
    void draw() noexcept;
};

extern c_nade_prediction nade_pred;




