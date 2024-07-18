
#ifndef	_GetDXVer_h_
#define	_GetDXVer_h_

// 현재는 8.1까지 체크가 된다.
// DX81SDK\samples\Multimedia\Misc\GetDXVer 에서 소스 차용.
// 추후 9.0으로 바뀔 때는 역시 같은 디렉토리의 같은 소스를 긁어다 쓰면 될 듯.

// 내부적으로 CoInitialize(), CoUninitialize()를 호출하므로, com을 쓰는 것들중 가장 앞에 가 있어야 한다.
#include <windows.h>

extern DWORD GetDXVersion();

#endif