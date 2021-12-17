#pragma once

#include "../../valve_sdk/csgostructs.hpp"

class c_engine_prediction
{
public:
    void run_engine_pred ( usercmd_t* cmd );
    void end_engine_pred ( ) const;
private:
    float flOldCurtime;
    float flOldFrametime;

    int iTickBaseBackup;
    int iFlagsBackup;
    int iButtonsBackup;
    int* m_pPredictionRandomSeed;
    int* m_pSetPredictionPlayer;
};
