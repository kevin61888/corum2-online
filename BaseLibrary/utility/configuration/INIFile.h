//
// File: INIFile.h
// Desc: INI file class
//

#ifndef __INIFILE_H__
#define __INIFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <BaseLibrary/Definition/TypeDef.h>
#include <BaseLibrary/Definition/SysDeps.h>
#include <vector>

#define AUTOCREATE_SECTIONS     (1L << 1)
#define AUTOCREATE_KEYS         (1L << 2)
#define MAX_BUFFER_LEN				512

class CINIFile
{
// Methods
public:
	CINIFile( void );
	CINIFile( STLSTRING szFileName );
	virtual ~CINIFile( void );

	BOOL Load( STLSTRING szFileName );
	BOOL Save( void );

protected:
	typedef struct sKeyType {
		STLSTRING	szKey;
		STLSTRING	szValue;
		STLSTRING	szComment;
	} KeyType;

	typedef std::vector<KeyType *> KeyList;
	typedef KeyList::iterator KeyItr;

	typedef struct st_section {
		STLSTRING	szName;
		STLSTRING	szComment;
		KeyList		Keys;
	} SectionType;

	typedef std::vector<SectionType *>	SectionList;
	typedef SectionList::iterator		SectionItr;

public:
	// Data handling methods
	STLSTRING		GetValue( STLSTRING szKey, STLSTRING szSection = STLSTRING("") );
	STLSTRING		GetString( STLSTRING szKey, STLSTRING szSection = STLSTRING("") );
	float			GetFloat( STLSTRING szKey, STLSTRING szSection = STLSTRING("") );
	int				GetInt( STLSTRING szKey, STLSTRING szSection = STLSTRING("") );
	BOOL			GetBOOL( STLSTRING szKey, STLSTRING szSection = STLSTRING("") );

	BOOL			SetValue( STLSTRING szKey, STLSTRING szValue, STLSTRING szComment = STLSTRING(""), STLSTRING szSection = STLSTRING("") );
	BOOL			SetFloat( STLSTRING szKey, float fValue, STLSTRING szComment = STLSTRING(""), STLSTRING szSection = STLSTRING("") );
	BOOL			SetInt( STLSTRING szKey, int nValue, STLSTRING szComment = STLSTRING(""), STLSTRING szSection = STLSTRING("") );
	BOOL			SetBOOL( STLSTRING szKey, BOOL bValue, STLSTRING szComment = STLSTRING(""), STLSTRING szSection = STLSTRING("") );

	BOOL			SetKeyComment( STLSTRING szKey, STLSTRING szComment, STLSTRING szSection = STLSTRING("") );
	BOOL			SetSectionComment( STLSTRING szSection, STLSTRING szComment );

	BOOL			DeleteKey( STLSTRING szKey, STLSTRING szFromSection = STLSTRING("") );
	BOOL			DeleteSection( STLSTRING szSection );

	// Key handling methods			
	BOOL			CreateKey( STLSTRING szKey, STLSTRING szValue, STLSTRING szComment = STLSTRING(""), STLSTRING szSection = STLSTRING("") );
	BOOL			CreateSection( STLSTRING szSection, STLSTRING szComment = STLSTRING("") );
	BOOL			CreateSection( STLSTRING szSection, STLSTRING szComment, KeyList Keys );

	// Utility methods
	int				SectionCount();
	int				KeyCount();

	void			Clear();
	void			SetFileName( STLSTRING szFileName );
	STLSTRING		CommentStr( STLSTRING szComment );

protected:
	KeyType*		GetKey( STLSTRING szKey, STLSTRING szSection );
	SectionType*	GetSection( STLSTRING szSection );

	// Data
public:
	long			m_Flags;		// Our settings flags.

protected:
	SectionList		m_Sections;		// Our list of sections
	STLSTRING		m_szFileName;	// The filename to write to
	BOOL			m_bDirty;		// Tracks whether or not data has changed.
};

#endif
