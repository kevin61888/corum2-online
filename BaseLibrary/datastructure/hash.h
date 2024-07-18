/*********************************************************************************************************
 *
 *  Module Name:	hash.h
 *
 *  Abstract:		Hashes methods. Auxalary purpose.
 *
 *  Author:		Vyacheslav I. Levtchenko (mail-to: sl@eltrast.ru)
 *
 *  Revision History:	05.08.2003	started
 *
 *  Classes, methods and structures:
 *
 *  Export: HashKey, HashKey<const word*>, HashKey<const char*>
 *
 *  TODO:
 *
 *********************************************************************************************************/

#ifndef __HASH_H__
#define __HASH_H__

template <class Tkey, class Thash = unsigned long>
class HashKey
{
  public:

  static Thash Hash (const Tkey& key)
   {
//    Thash count = (Thash)key >> 3;
//    return (Thash)( ((Thash)key << count) | ((Thash)key >> ((sizeof (Thash) << 3) - count)) );
    return (Thash)( ((Thash)key >> 3) + ((Thash)key >> 2) + ((Thash)key >> 1));
   }
};
/*
template <>
class HashKey <const word*>
{
  public:

  static Thash Hash (const word* key)
   {
    tbrk ();

    Thash Hash = 0;

    while (*key)
      Hash = (Hash << 5) + Hash + *key++;

    return Hash;
   }
};

template <>
class HashKey <const char*>
{
  public:

  static Thash Hash (const char* key)
   {
    Thash Hash = 0; 

    while (*key)
      Hash = (Hash << 5) + Hash + *key++;

    return Hash;
   }
};
*/
#endif