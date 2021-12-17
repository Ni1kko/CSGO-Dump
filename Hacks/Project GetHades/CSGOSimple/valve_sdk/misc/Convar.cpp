#include "Convar.hpp"

#include "../sdk.hpp"

#include "characterset.hpp"
#include "UtlBuffer.hpp"

#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) )
#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )

con_command_base* con_command_base::s_pConCommandBases = NULL;
con_command_base* con_command_base::s_pRegisteredCommands = NULL;
IConCommandBaseAccessor* con_command_base::s_pAccessor = NULL;
static int s_nDLLIdentifier = -1;
static int s_nCVarFlag = 0;
static bool s_bRegistered = false;

class CDefaultAccessor : public IConCommandBaseAccessor
{
public:
    virtual bool RegisterConCommandBase ( con_command_base* pVar )
    {
        // Link to engine's list instead
        interfaces::cvar->unregister_con_command ( pVar );
        return true;
    }
};

static CDefaultAccessor s_DefaultAccessor;

//-----------------------------------------------------------------------------
// Called by the framework to register ConCommandBases with the ICVar
//-----------------------------------------------------------------------------
void ConVar_Register ( int nCVarFlag, IConCommandBaseAccessor* pAccessor )
{
    if ( !interfaces::cvar || s_bRegistered )
        return;

    assert ( s_nDLLIdentifier < 0 );
    s_bRegistered = true;
    s_nCVarFlag = nCVarFlag;
    s_nDLLIdentifier = interfaces::cvar->allocate_dll_identifier( );

    con_command_base::s_pAccessor = pAccessor ? pAccessor : &s_DefaultAccessor;
    con_command_base* pCur = con_command_base::s_pConCommandBases;

    while ( pCur )
    {
        con_command_base* pNext = pCur->m_pNext;
        pCur->add_flags ( s_nCVarFlag );
        pCur->Init( );

        con_command_base::s_pRegisteredCommands = pCur;

        pCur = pNext;
    }

    con_command_base::s_pConCommandBases = NULL;
}

void ConVar_Unregister ( )
{
    if ( !interfaces::cvar || !s_bRegistered )
        return;

    assert ( s_nDLLIdentifier >= 0 );
    interfaces::cvar->unregister_con_commands ( s_nDLLIdentifier );
    s_nDLLIdentifier = -1;
    s_bRegistered = false;
}

con_command_base::con_command_base ( void )
{
    m_bRegistered = false;
    m_pszName = NULL;
    m_pszHelpString = NULL;

    m_nFlags = 0;
    m_pNext = NULL;
}

con_command_base::con_command_base ( const char* pName, const char* pHelpString /*=0*/, int flags /*= 0*/ )
{
    Create ( pName, pHelpString, flags );
}

con_command_base::~con_command_base ( void )
{
}

bool con_command_base::is_command ( void ) const
{
    //	assert( 0 ); This can't assert. . causes a recursive assert in Sys_Printf, etc.
    return true;
}

CVarDLLIdentifier_t con_command_base::GetDLLIdentifier ( ) const
{
    return s_nDLLIdentifier;
}

void con_command_base::Create ( const char* pName, const char* pHelpString /*= 0*/, int flags /*= 0*/ )
{
    static const char* empty_string = "";

    m_bRegistered = false;

    // Name should be static data
    m_pszName = pName;
    m_pszHelpString = pHelpString ? pHelpString : empty_string;

    m_nFlags = flags;

    if ( ! ( m_nFlags & FCVAR_UNREGISTERED ) )
    {
        m_pNext = s_pConCommandBases;
        s_pConCommandBases = this;
    }
    else
        m_pNext = NULL;
}

void con_command_base::Init ( )
{
    if ( s_pAccessor )
        s_pAccessor->RegisterConCommandBase ( this );
}

void con_command_base::Shutdown ( )
{
    if ( interfaces::cvar )
        interfaces::cvar->unregister_con_command ( this );
}

const char* con_command_base::get_name ( void ) const
{
    return m_pszName;
}

bool con_command_base::is_flag_set ( int flag ) const
{
    return ( flag & m_nFlags ) != 0;
}

void con_command_base::add_flags ( int flags )
{
    m_nFlags |= flags;
}

void con_command_base::RemoveFlags ( int flags )
{
    m_nFlags &= ~flags;
}

int con_command_base::get_flags ( void ) const
{
    return m_nFlags;
}

const con_command_base* con_command_base::GetNext ( void ) const
{
    return m_pNext;
}

con_command_base* con_command_base::GetNext ( void )
{
    return m_pNext;
}

char* con_command_base::CopyString ( const char* from ) const
{
    char* to;

    int len = strlen ( from );

    if ( len <= 0 )
    {
        to = new char[1];
        to[ 0 ] = 0;
    }
    else
    {
        to = new char[len + 1];
        strncpy_s ( to, len + 1, from, len + 1 );
    }

    return to;
}

const char* con_command_base::get_help_text ( void ) const
{
    return m_pszHelpString;
}

bool con_command_base::is_registered ( void ) const
{
    return m_bRegistered;
}

static characterset_t s_BreakSet;
static bool s_bBuiltBreakSet = false;

CCommand::CCommand ( )
{
    if ( !s_bBuiltBreakSet )
    {
        s_bBuiltBreakSet = true;
        CharacterSetBuild ( &s_BreakSet, "{}()':" );
    }

    Reset( );
}

CCommand::CCommand ( int nArgC, const char** ppArgV )
{
    assert ( nArgC > 0 );

    if ( !s_bBuiltBreakSet )
    {
        s_bBuiltBreakSet = true;
        CharacterSetBuild ( &s_BreakSet, "{}()':" );
    }

    Reset( );

    char* pBuf = m_pArgvBuffer;
    char* pSBuf = m_pArgSBuffer;
    m_nArgc = nArgC;

    for ( int i = 0; i < nArgC; ++i )
    {
        m_ppArgv[ i ] = pBuf;
        int nLen = strlen ( ppArgV[ i ] );
        memcpy ( pBuf, ppArgV[ i ], nLen + 1 );

        if ( i == 0 )
            m_nArgv0Size = nLen;

        pBuf += nLen + 1;

        bool bContainsSpace = strchr ( ppArgV[ i ], ' ' ) != NULL;

        if ( bContainsSpace )
            *pSBuf++ = '\"';

        memcpy ( pSBuf, ppArgV[ i ], nLen );
        pSBuf += nLen;

        if ( bContainsSpace )
            *pSBuf++ = '\"';

        if ( i != nArgC - 1 )
            *pSBuf++ = ' ';
    }
}

void CCommand::Reset ( )
{
    m_nArgc = 0;
    m_nArgv0Size = 0;
    m_pArgSBuffer[ 0 ] = 0;
}

characterset_t* CCommand::DefaultBreakSet ( )
{
    return &s_BreakSet;
}

bool CCommand::Tokenize ( const char* pCommand, characterset_t* pBreakSet )
{
    Reset( );

    if ( !pCommand )
        return false;

    // Use default break Set
    if ( !pBreakSet )
        pBreakSet = &s_BreakSet;

    // Copy the current command into a temp buffer
    // NOTE: This is here to avoid the pointers returned by DequeueNextCommand
    // to become invalid by calling AddText. Is there a way we can avoid the memcpy?
    int nLen = strlen ( pCommand );

    if ( nLen >= COMMAND_MAX_LENGTH - 1 )
    {
        //Warning("CCommand::Tokenize: Encountered command which overflows the tokenizer buffer.. Skipping!\n");
        return false;
    }

    memcpy ( m_pArgSBuffer, pCommand, nLen + 1 );

    // Parse the current command into the current command buffer
    CUtlBuffer bufParse ( m_pArgSBuffer, nLen, CUtlBuffer::TEXT_BUFFER | CUtlBuffer::READ_ONLY );
    int nArgvBufferSize = 0;

    while ( bufParse.IsValid( ) && ( m_nArgc < COMMAND_MAX_ARGC ) )
    {
        char* pArgvBuf = &m_pArgvBuffer[ nArgvBufferSize ];
        int nMaxLen = COMMAND_MAX_LENGTH - nArgvBufferSize;
        int nStartGet = bufParse.TellGet( );
        int nSize = bufParse.ParseToken ( pBreakSet, pArgvBuf, nMaxLen );

        if ( nSize < 0 )
            break;

        // Check for overflow condition
        if ( nMaxLen == nSize )
        {
            Reset( );
            return false;
        }

        if ( m_nArgc == 1 )
        {
            // Deal with the case where the arguments were quoted
            m_nArgv0Size = bufParse.TellGet( );
            bool bFoundEndQuote = m_pArgSBuffer[ m_nArgv0Size - 1 ] == '\"';

            if ( bFoundEndQuote )
                --m_nArgv0Size;

            m_nArgv0Size -= nSize;
            assert ( m_nArgv0Size != 0 );

            // The StartGet check is to handle this case: "foo"bar
            // which will parse into 2 different args. ArgS should point to bar.
            bool bFoundStartQuote = ( m_nArgv0Size > nStartGet ) && ( m_pArgSBuffer[ m_nArgv0Size - 1 ] == '\"' );
            assert ( bFoundEndQuote == bFoundStartQuote );

            if ( bFoundStartQuote )
                --m_nArgv0Size;
        }

        m_ppArgv[ m_nArgc++ ] = pArgvBuf;

        if ( m_nArgc >= COMMAND_MAX_ARGC )
        {
            //Warning("CCommand::Tokenize: Encountered command which overflows the argument buffer.. Clamped!\n");
        }

        nArgvBufferSize += nSize + 1;
        assert ( nArgvBufferSize <= COMMAND_MAX_LENGTH );
    }

    return true;
}

const char* CCommand::FindArg ( const char* pName ) const
{
    int nArgC = ArgC( );

    for ( int i = 1; i < nArgC; i++ )
    {
        if ( !_stricmp ( Arg ( i ), pName ) )
            return ( i + 1 ) < nArgC ? Arg ( i + 1 ) : "";
    }

    return 0;
}

int CCommand::FindArgInt ( const char* pName, int nDefaultVal ) const
{
    const char* pVal = FindArg ( pName );

    if ( pVal )
        return atoi ( pVal );
    else
        return nDefaultVal;
}

int DefaultCompletionFunc ( const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH] )
{
    return 0;
}

con_command::con_command ( const char* pName, FnCommandCallbackV1_t callback, const char* pHelpString /*= 0*/, int flags /*= 0*/,
                           FnCommandCompletionCallback completionFunc /*= 0*/ )
{
    // Set the callback
    m_fnCommandCallbackV1 = callback;
    m_bUsingNewCommandCallback = false;
    m_bUsingCommandCallbackInterface = false;
    m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
    m_bHasCompletionCallback = completionFunc != 0;

    // Setup the rest
    BaseClass::Create ( pName, pHelpString, flags );
}

con_command::con_command ( const char* pName, FnCommandCallback_t callback, const char* pHelpString /*= 0*/, int flags /*= 0*/,
                           FnCommandCompletionCallback completionFunc /*= 0*/ )
{
    // Set the callback
    m_fnCommandCallback = callback;
    m_bUsingNewCommandCallback = true;
    m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
    m_bHasCompletionCallback = completionFunc != 0;
    m_bUsingCommandCallbackInterface = false;

    // Setup the rest
    BaseClass::Create ( pName, pHelpString, flags );
}

con_command::con_command ( const char* pName, ICommandCallback* pCallback, const char* pHelpString /*= 0*/, int flags /*= 0*/,
                           ICommandCompletionCallback* pCompletionCallback /*= 0*/ )
{
    // Set the callback
    m_pCommandCallback = pCallback;
    m_bUsingNewCommandCallback = false;
    m_pCommandCompletionCallback = pCompletionCallback;
    m_bHasCompletionCallback = ( pCompletionCallback != 0 );
    m_bUsingCommandCallbackInterface = true;

    // Setup the rest
    BaseClass::Create ( pName, pHelpString, flags );
}

con_command::~con_command ( void )
{
}

bool con_command::is_command ( void ) const
{
    return true;
}

void con_command::Dispatch ( const CCommand& command )
{
    if ( m_bUsingNewCommandCallback )
    {
        if ( m_fnCommandCallback )
        {
            ( *m_fnCommandCallback ) ( command );
            return;
        }
    }
    else if ( m_bUsingCommandCallbackInterface )
    {
        if ( m_pCommandCallback )
        {
            m_pCommandCallback->CommandCallback ( command );
            return;
        }
    }
    else
    {
        if ( m_fnCommandCallbackV1 )
        {
            ( *m_fnCommandCallbackV1 )( );
            return;
        }
    }

    // Command without callback!!!
    //AssertMsg(0, ("Encountered con_command without a callback!\n"));
}

int con_command::AutoCompleteSuggest ( const char* partial, CUtlVector< CUtlString >& commands )
{
    if ( m_bUsingCommandCallbackInterface )
    {
        if ( !m_pCommandCompletionCallback )
            return 0;

        return m_pCommandCompletionCallback->CommandCompletionCallback ( partial, commands );
    }

    if ( !m_fnCompletionCallback )
        return 0;

    char rgpchCommands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH];
    int iret = ( m_fnCompletionCallback ) ( partial, rgpchCommands );

    for ( int i = 0; i < iret; ++i )
    {
        CUtlString str = rgpchCommands[ i ];
        commands.AddToTail ( str );
    }

    return iret;
}

bool con_command::CanAutoComplete ( void )
{
    return m_bHasCompletionCallback;
}

con_var::con_var ( const char* pName, const char* pDefaultValue, int flags /* = 0 */ )
{
    Create ( pName, pDefaultValue, flags );
}

con_var::con_var ( const char* pName, const char* pDefaultValue, int flags, const char* pHelpString )
{
    Create ( pName, pDefaultValue, flags, pHelpString );
}

con_var::con_var ( const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax )
{
    Create ( pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax );
}

con_var::con_var ( const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, FnChangeCallback_t callback )
{
    Create ( pName, pDefaultValue, flags, pHelpString, false, 0.0, false, 0.0, callback );
}

con_var::con_var ( const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax,
                   FnChangeCallback_t callback )
{
    Create ( pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback );
}

con_var::~con_var ( void )
{
    //if(is_registered())
    //    convar->UnregisterConCommand(this);
    if ( m_Value.m_pszString )
    {
        delete[] m_Value.m_pszString;
        m_Value.m_pszString = NULL;
    }
}

void con_var::install_change_callback ( FnChangeCallback_t callback, bool bInvoke )
{
    if ( callback )
    {
        if ( m_fnChangeCallbacks.GetOffset ( callback ) != -1 )
        {
            m_fnChangeCallbacks.AddToTail ( callback );

            if ( bInvoke )
                callback ( this, m_Value.m_pszString, m_Value.m_fValue );
        }
        else
        {
            //Warning("install_change_callback ignoring duplicate change callback!!!\n");
        }
    }
    else
    {
        //Warning("install_change_callback called with NULL callback, ignoring!!!\n");
    }
}

bool con_var::is_flag_set ( int flag ) const
{
    return ( flag & m_pParent->m_nFlags ) != 0;
}

const char* con_var::get_help_text ( void ) const
{
    return m_pParent->m_pszHelpString;
}

void con_var::add_flags ( int flags )
{
    m_pParent->m_nFlags |= flags;

#ifdef ALLOW_DEVELOPMENT_CVARS
    m_pParent->m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
#endif
}

int con_var::get_flags ( void ) const
{
    return m_pParent->m_nFlags;
}

bool con_var::is_registered ( void ) const
{
    return m_pParent->m_bRegistered;
}

const char* con_var::get_name ( void ) const
{
    return m_pParent->m_pszName;
}

bool con_var::is_command ( void ) const
{
    return false;
}

void con_var::Init ( )
{
    BaseClass::Init( );
}

const char* con_var::get_base_name ( void ) const
{
    return m_pParent->m_pszName;
}

int con_var::get_split_screen_player_slot ( void ) const
{
    return 0;
}

void con_var::InternalSetValue ( const char* value )
{
    char tempVal[32];

    auto temp = * ( uint32_t* )&m_Value.m_fValue ^ ( uint32_t )this;
    float flOldValue = * ( float* )( &temp );

    char* val = ( char* )value;
    float fNewValue = ( float )atof ( value );

    if ( ClampValue ( fNewValue ) )
    {
        snprintf ( tempVal, sizeof ( tempVal ), "%f", fNewValue );
        val = tempVal;
    }

    // Redetermine value
    * ( uint32_t* )&m_Value.m_fValue = * ( uint32_t* )&fNewValue ^ ( uint32_t )this;
    * ( uint32_t* )&m_Value.m_nValue = ( uint32_t )fNewValue ^ ( uint32_t )this;

    if ( ! ( m_nFlags & FCVAR_NEVER_AS_STRING ) )
        ChangeStringValue ( val, flOldValue );
}

void con_var::ChangeStringValue ( const char* tempVal, float flOldValue )
{
    auto pszOldValue = static_cast< char* > ( _malloca ( m_Value.m_StringLength ) );
    memcpy ( pszOldValue, m_Value.m_pszString, m_Value.m_StringLength );

    int len = strlen ( tempVal ) + 1;

    if ( len > m_Value.m_StringLength )
    {
        if ( m_Value.m_pszString )
            delete[] m_Value.m_pszString;

        m_Value.m_pszString = new char[len];
        m_Value.m_StringLength = len;
    }

    memcpy ( m_Value.m_pszString, std::to_string ( this->get_float( ) ).c_str( ), len );

    // Invoke any necessary callback function
    for ( int i = 0; i < m_fnChangeCallbacks.Count( ); i++ )
        m_fnChangeCallbacks[ i ] ( this, pszOldValue, flOldValue );

    if ( interfaces::cvar )
        interfaces::cvar->call_global_change_callbacks ( this, pszOldValue, flOldValue );
}

bool con_var::ClampValue ( float& value )
{
    if ( m_bHasMin && ( value < m_fMinVal ) )
    {
        value = m_fMinVal;
        return true;
    }

    if ( m_bHasMax && ( value > m_fMaxVal ) )
    {
        value = m_fMaxVal;
        return true;
    }

    return false;
}

void con_var::InternalSetFloatValue ( float fNewValue )
{
    if ( fNewValue == m_Value.m_fValue )
        return;

    ClampValue ( fNewValue );

    // Redetermine value
    float flOldValue = m_Value.m_fValue;
    * ( uint32_t* )&m_Value.m_fValue = * ( uint32_t* )&fNewValue ^ ( uint32_t )this;
    * ( uint32_t* )&m_Value.m_nValue = ( uint32_t )fNewValue ^ ( uint32_t )this;

    if ( ! ( m_nFlags & FCVAR_NEVER_AS_STRING ) )
    {
        char tempVal[32];
        snprintf ( tempVal, sizeof ( tempVal ), "%f", m_Value.m_fValue );
        ChangeStringValue ( tempVal, flOldValue );
    }
    else
    {
        //assert(m_fnChangeCallbacks.Count() == 0);
    }
}

void con_var::InternalSetIntValue ( int nValue )
{
    if ( nValue == ( ( int )m_Value.m_nValue ^ ( int )this ) )
        return;

    float fValue = ( float )nValue;

    if ( ClampValue ( fValue ) )
        nValue = ( int )( fValue );

    // Redetermine value
    float flOldValue = m_Value.m_fValue;
    * ( uint32_t* )&m_Value.m_fValue = * ( uint32_t* )&fValue ^ ( uint32_t )this;
    * ( uint32_t* )&m_Value.m_nValue = * ( uint32_t* )&nValue ^ ( uint32_t )this;

    if ( ! ( m_nFlags & FCVAR_NEVER_AS_STRING ) )
    {
        char tempVal[32];
        snprintf ( tempVal, sizeof ( tempVal ), "%d", m_Value.m_nValue );
        ChangeStringValue ( tempVal, flOldValue );
    }
    else
    {
        //assert(m_fnChangeCallbacks.Count() == 0);
    }
}

void con_var::InternalSetColorValue ( Color cValue )
{
    int color = ( int )cValue.GetRawColor( );
    InternalSetIntValue ( color );
}

void con_var::Create ( const char* pName, const char* pDefaultValue, int flags /*= 0*/,
                       const char* pHelpString /*= NULL*/, bool bMin /*= false*/, float fMin /*= 0.0*/,
                       bool bMax /*= false*/, float fMax /*= false*/, FnChangeCallback_t callback /*= NULL*/ )
{
    static const char* empty_string = "";

    m_pParent = this;

    // Name should be static data
    m_pszDefaultValue = pDefaultValue ? pDefaultValue : empty_string;

    m_Value.m_StringLength = strlen ( m_pszDefaultValue ) + 1;
    m_Value.m_pszString = new char[m_Value.m_StringLength];
    memcpy ( m_Value.m_pszString, m_pszDefaultValue, m_Value.m_StringLength );

    m_bHasMin = bMin;
    m_fMinVal = fMin;
    m_bHasMax = bMax;
    m_fMaxVal = fMax;

    if ( callback )
        m_fnChangeCallbacks.AddToTail ( callback );

    float value = ( float )atof ( m_Value.m_pszString );

    * ( uint32_t* )&m_Value.m_fValue = * ( uint32_t* )&value ^ ( uint32_t )this;
    * ( uint32_t* )&m_Value.m_nValue = * ( uint32_t* )&value ^ ( uint32_t )this;

    BaseClass::Create ( pName, pHelpString, flags );
}

void con_var::set_value ( const char* value )
{
    con_var* var = ( con_var* )m_pParent;
    var->InternalSetValue ( value );
}

void con_var::set_value ( float value )
{
    con_var* var = ( con_var* )m_pParent;
    var->InternalSetFloatValue ( value );
}

void con_var::set_value ( int value )
{
    con_var* var = ( con_var* )m_pParent;
    var->InternalSetIntValue ( value );
}

void con_var::set_value ( Color value )
{
    con_var* var = ( con_var* )m_pParent;
    var->InternalSetColorValue ( value );
}

void con_var::revert ( void ) const
{
    // Force default value again
    con_var* var = ( con_var* )m_pParent;
    var->set_value ( var->m_pszDefaultValue );
}

bool con_var::get_min ( float& minVal ) const
{
    minVal = m_pParent->m_fMinVal;
    return m_pParent->m_bHasMin;
}

bool con_var::get_max ( float& maxVal ) const
{
    maxVal = m_pParent->m_fMaxVal;
    return m_pParent->m_bHasMax;
}

const char* con_var::get_default ( void ) const
{
    return m_pParent->m_pszDefaultValue;
}
