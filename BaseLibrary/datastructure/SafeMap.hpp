#pragma once

#include <hash_map>
#include <BaseLibrary/Thread/Lock.hpp>

template<class key, class value>
class CSafeMap : stdext::hash_map<key, value>
{
private:
	typedef stdext::hash_map<key, value> _SafeBase;
	CBCSLock		m_Lock;

public:

	mapped_type& operator[](const key_type& _Keyval)
		{	// find element matching _Keyval or insert with default mapped
			
			CBCSLock::Syncronize sync(m_Lock);

			iterator _Where = this->lower_bound(_Keyval);
			if (_Where == this->end())
				_Where = this->insert(value_type(_Keyval, mapped_type())).first;
			return ((*_Where).second);
		}

	size_type erase(const key_type& _Keyval)
		{	// erase and count all that match _Keyval

			CBCSLock::Syncronize sync(m_Lock);

			_Pairii _Where = equal_range(_Keyval);
			size_type _Num = 0;
			_Distance(_Where.first, _Where.second, _Num);
			erase(_Where.first, _Where.second);
			return (_Num);
		}

};