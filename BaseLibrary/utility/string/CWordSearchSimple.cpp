#include "../../StdAfx.h"
#include "CWordSearchSimple.h"

CWordSearchSimple::CWordSearchSimple( void ) : m_bInit( FALSE )
{
}

CWordSearchSimple::~CWordSearchSimple( void )
{
}

BOOL CWordSearchSimple::AddWord( LPCTSTR szWord )
{
//void LoadCurses(const char *filename)
//{
//    static char comment_chars[] = { "$;\n" };
//    char buf[128];
//    char *ptr = m_szCurseWords;
//    FILE *file;
//
//    if (bInit) return;
//
//
//    if (file = fopen(filename, "r"), !file) {
//	fprintf(stderr, "! can't open [%s]\n", filename);
//	return;
//    }
//
//    while (fgets(buf, sizeof(buf), file)) {
//	if (strchr(comment_chars, buf[0])) continue;
//
//    	ptr += sprintf(ptr, buf);
//
//    	if (ptr - m_szCurseWords >= HUGE_NUMBER) {
//    	    fprintf(stderr, "! %s is too long, pass.\n", filename);
//    	    m_szCurseWords[HUGE_NUMBER - 1] = 0;
//    	    break;
//    	}
//    }
//    fclose(file);
//
//    fprintf(stderr, "* parsing [%s]. %d bytes read\n", filename, ptr - m_szCurseWords);
//
//    return;
//}
	return TRUE;
}

TCHAR *CWordSearchSimple::ReplaceString( LPTSTR szString )
{
    char result[HUGE_NUMBER], string[HUGE_NUMBER];
    char line[128], curse[128], replace[128];
    char *ptr = m_szCurseWords, *lptr, *cptr;

    if (!szString) return NULL;
	int max_len = strlen( szString );

    if (!max_len || max_len >= sizeof(result))
		return szString;

    // skip converting, if no curse is found.
    // in fact, 'isCurse' and 'convert' are redundant.
    // but it can be ignored and more efficient 
    // because users speak a normal sentence 
    // much more than a curse

    if ( JustSearchString( szString ) == FALSE )
		return szString;

    // filter characters to keep users from
    // speaking curses mixed with white spaces 
    // or punctuaion mark
    FilterChar( szString, string, "\t _-.:^");

    while (ptr = GetField(ptr, '\n', line), line[0]) {

	lptr = line;
	lptr = GetField(lptr, '\t', curse);		// get the first field	
	lptr = GetField(lptr, '\t', replace);	// get the second field
	cptr = string;
	strcpy(result, string);

	while (cptr = strstr(string, curse), cptr) {
	    // if there is no matching replacement of curse word
	    if (!replace[0]) {
		szString[0] = NULL;
		return szString;
	    }

	    if ((max_len - 1) < (int) ((cptr - string) + strlen(replace) + strlen(cptr + strlen(curse)))) {
                // string too long, remove the curses that linger around
                strncpy(result, string, cptr - string);
                strcpy(result + (cptr - string), cptr + strlen(curse));
            } else {               	    
	        strncpy(result, string, cptr - string);
	        strcpy(result + (cptr - string), replace);
	        strcpy(result + (cptr - string + strlen(replace)), cptr + strlen(curse));
            }
	    // refresh intermediate result
	    strcpy(string, result);
	}
    }

    result[max_len] = 0;		// prevent overflow
    strcpy( szString, result );

    return szString;
}

BOOL CWordSearchSimple::JustSearchString( LPCTSTR szString )
{
    char string[HUGE_NUMBER], line[HUGE_NUMBER], word[HUGE_NUMBER];
    char *ptr = m_szCurseWords;
    const char *sub, *subs, *subw;

    if (!szString) return FALSE;

    // filter characters to keep users from
    // speaking curses mixed with white spaces 
    // or punctuaion mark

    FilterChar(szString, string, "\t _-.:^");


    // optimized for multibyte code set.

    while (ptr = GetField(ptr, '\n', line), line[0]) {
		GetField(line, '\t', word);	
		sub = string;
		while (*sub && *(sub + 1)) {
			subs = sub;	// current comparison pointer in string
			subw = word;	// current comparison pointer in word
			while (*subs && (*subs == *subw)) {
				subs++; subw++;
				if (*subw == 0) {
					return TRUE;		// reach to null, curse
				}
			}
			sub += *sub < 0 ? 2 : 1;
		}
    }
    return FALSE;
}

LPSTR CWordSearchSimple::GetField( LPCSTR szBuffer, int delim, LPSTR szField )
{
    unsigned char *src = (unsigned char *) szBuffer;
    while ((int)*src == delim && *src != 0) src++;
    while ((int)*src != delim && *src != 0) *szField ++ = *src ++;

    *szField = '\0';
    return (char *)src;
}


void CWordSearchSimple::FilterChar( LPCSTR szSource, LPSTR szDest, LPCSTR szChars )
{
    char *srcp = (char *) szSource;
    char *dstp = szDest;

    while (*srcp) {
	if (strchr( szChars, *srcp) == NULL) {
	    *dstp = *srcp;   dstp += 1;
	} else {
	    int msb, lsb;
	    msb = *(unsigned char *)(srcp-1);
	    lsb = *(unsigned char *)srcp;
	    if (srcp > szSource && (
		(msb == 0x82 && lsb > 0x9e && lsb < 0xf2)	// hirakana
		|| (msb == 0x83 && lsb > 0x3f && lsb < 0x97)	// katakana
	       	|| (msb >= 0x88 && msb <= 0x9f && msb != 0x7f	// kanji 1 group
		    && lsb > 0x3f && lsb < 0xfd)
		|| (msb >= 0xe0 && msb <= 0xfc && msb != 0x7f	// kanji 2 group
		    && lsb > 0x3f && lsb < 0xfd))) {
		*dstp = *srcp;   dstp += 1;
	    }
	}
	srcp += 1;
    }
    *dstp = 0;
}


