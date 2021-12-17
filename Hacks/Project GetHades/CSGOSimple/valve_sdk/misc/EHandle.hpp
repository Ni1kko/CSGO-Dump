#pragma once

#include "BaseHandle.hpp"
#include "../sdk.hpp"

// -------------------------------------------------------------------------------------------------- //
// Game-code CBaseHandle implementation.
// -------------------------------------------------------------------------------------------------- //

inline IHandleEntity* CBaseHandle::get ( ) const
{
    return interfaces::entity_list->get_client_entity_from_handle ( *this );
}

// -------------------------------------------------------------------------------------------------- //
// CHandle.
// -------------------------------------------------------------------------------------------------- //
template < class T >
class CHandle : public CBaseHandle
{
public:

    CHandle ( );
    CHandle ( int iEntry, int iSerialNumber );
    CHandle ( const CBaseHandle& handle );
    CHandle ( T* pVal );

    // The index should have come from a call to ToInt(). If it hasn't, you're in trouble.
    static CHandle< T > FromIndex ( int index );

    T* get ( ) const;
    void set ( const T* pVal );

    operator T* ( );
    operator T* ( ) const;

    bool operator ! ( ) const;
    bool operator== ( T* val ) const;
    bool operator!= ( T* val ) const;
    const CBaseHandle& operator= ( const T* val );

    T* operator-> ( ) const;
};

// ----------------------------------------------------------------------- //
// Inlines.
// ----------------------------------------------------------------------- //

template < class T >
CHandle< T >::CHandle ( )
{
}

template < class T >
CHandle< T >::CHandle ( int iEntry, int iSerialNumber )
{
    Init ( iEntry, iSerialNumber );
}

template < class T >
CHandle< T >::CHandle ( const CBaseHandle& handle )
    : CBaseHandle ( handle )
{
}

template < class T >
CHandle< T >::CHandle ( T* pObj )
{
    Term( );
    set ( pObj );
}

template < class T >
inline CHandle< T > CHandle< T >::FromIndex ( int index )
{
    CHandle< T > ret;
    ret.m_Index = index;
    return ret;
}

template < class T >
inline T* CHandle< T >::get ( ) const
{
    return ( T* )CBaseHandle::get( );
}

template < class T >
inline CHandle< T >::operator T* ( )
{
    return get( );
}

template < class T >
inline CHandle< T >::operator T* ( ) const
{
    return get( );
}

template < class T >
inline bool CHandle< T >::operator ! ( ) const
{
    return !get( );
}

template < class T >
inline bool CHandle< T >::operator== ( T* val ) const
{
    return get( ) == val;
}

template < class T >
inline bool CHandle< T >::operator!= ( T* val ) const
{
    return get( ) != val;
}

template < class T >
void CHandle< T >::set ( const T* pVal )
{
    CBaseHandle::set ( reinterpret_cast< const IHandleEntity* > ( pVal ) );
}

template < class T >
inline const CBaseHandle& CHandle< T >::operator= ( const T* val )
{
    set ( val );
    return *this;
}

template < class T >
T* CHandle< T >::operator -> ( ) const
{
    return get( );
}
