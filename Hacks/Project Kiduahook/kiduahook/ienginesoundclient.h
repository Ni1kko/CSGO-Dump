#pragma once

#include "sdk.h"

class IEngineSoundClient
{
public:
	virtual bool PrecacheSound( const char *pSample, bool bPreload = false, bool bIsUISound = false ) = 0;
	virtual bool IsSoundPrecached( const char *pSample ) = 0;
	virtual void PrefetchSound( const char *pSample ) = 0;

	float GetSoundDuration( const char *pSample )
	{
		typedef float( __thiscall* OriginalFn )( PVOID, const char * );
		return getvfunc< OriginalFn >( this, 4 )( this, pSample );
	}

	virtual void EmitSound( IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity ) = 0;
	virtual void EmitSound2( /*IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSample, float flVolume, soundlevel_t iSoundlevel, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 */ ) = 0;
	virtual void EmitSentenceByIndex( /*IRecipientFilter& filter, int iEntIndex, int iChannel, int iSentenceIndex, float flVolume, soundlevel_t iSoundlevel, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector< Vector >* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1 */ ) = 0;
	virtual void StopSound( int iEntIndex, int iChannel, const char *pSample ) = 0;
	virtual void StopAllSounds( bool bClearBuffers ) = 0;
	virtual void SetRoomType( IRecipientFilter& filter, int roomType ) = 0;
	virtual void SetPlayerDSP( IRecipientFilter& filter, int dspType, bool fastReset ) = 0;
	virtual void EmitAmbientSound( /*const char *pSample, float flVolume, int iPitch = PITCH_NORM, int flags = 0, float soundtime = 0.0f*/ ) = 0;
	virtual float GetDistGainFromSoundLevel( /*soundlevel_t soundlevel, float dist*/ ) = 0;
	virtual int             GetGuidForLastSoundEmitted() = 0;
	virtual bool    IsSoundStillPlaying( int guid ) = 0;
	virtual void    StopSoundByGuid( int guid ) = 0;
	virtual void    SetVolumeByGuid( int guid, float fvol ) = 0;
	virtual void    GetActiveSounds( /*CUtlVector< SndInfo_t >& sndlist*/ ) = 0;
	virtual void    PrecacheSentenceGroup( const char *pGroupName ) = 0;
	virtual void    NotifyBeginMoviePlayback() = 0;
	virtual void    NotifyEndMoviePlayback() = 0;
};

extern IEngineSoundClient* g_pEngineSound;