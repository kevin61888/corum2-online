#pragma once

#define CLASS_DECLARATION(Type, ValueName, FuncName)	\
protected:		\
	Type ValueName; \
public:			\
	const Type& FuncName()const{return ValueName;}	\
	void FuncName(const Type &Value){ValueName = Value;}

