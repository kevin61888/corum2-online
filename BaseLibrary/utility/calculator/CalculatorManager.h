#pragma once

#include "Calculator.h"

#include <BaseLibrary/Utility/Etc/CommandParser.h>


typedef class CCalculatorManager*	LPCCalculatorManager;
class CCalculatorManager
{
public:
	// constructor and destructor
	CCalculatorManager(void);
	virtual ~CCalculatorManager(void);


	CCommandParser	m_CalcParser;

	CCalculator		m_CalcHP;
	CCalculator		m_CalcSP;
	CCalculator		m_CalcLP;
	CCalculator		m_CalcHPRG;
	CCalculator		m_CalcSPRG;
	CCalculator		m_CalcATT;
	CCalculator		m_CalcDEF;
	CCalculator		m_CalcEXP;


	BOOL			Initiate(void);
};
