#include "stdafx.h"
#include "PartyMatcher.h"

// 파티 매칭용 조건 맵을 위한 연산자
bool operator<(const MATCH_CONDITION_PARTY& f, const MATCH_CONDITION_PARTY& b)
{
	int f_sum, b_sum;

	f_sum = f.bLevel * 100 + f.bExp * 10 + f.bItem;
	b_sum = b.bLevel * 100 + b.bExp * 10 + b.bItem;
	
	return (f_sum < b_sum);
}

// 전역 파티 매칭 시스템
CMatcher<MATCH_CONDITION_PARTY, DWORD, CMatcherActionParty> g_PartyMatcher;