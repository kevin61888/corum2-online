#include "../../StdAfx.h"
#include "CalculatorManager.h"

#include <BaseLibrary/Utility/Script/ScriptFile.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constants
const char* Calculation_HP			= "HP";
const char* Calculation_SP			= "SP";
const char* Calculation_LP			= "LP";
const char* Calculation_HPRG		= "HPRG";
const char* Calculation_SPRG		= "SPRG";
const char* Calculation_ATT_MELEE	= "ATT_MELEE";
const char* Calculation_DEF_MELEE	= "DEF_MELEE";
const char* Calculation_EXP			= "EXP";

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CCalculatorManager::CCalculatorManager(void)
{
	m_CalcParser.InsertCommand(CCalculator::STATUS_LEV,				"LEV");		// level
	m_CalcParser.InsertCommand(CCalculator::STATUS_STR,				"STR");
	m_CalcParser.InsertCommand(CCalculator::STATUS_DEX,				"DEX");
	m_CalcParser.InsertCommand(CCalculator::STATUS_CON,				"CON");
	m_CalcParser.InsertCommand(CCalculator::STATUS_MEN,				"MEN");
	m_CalcParser.InsertCommand(CCalculator::STATUS_INT,				"INT");
	m_CalcParser.InsertCommand(CCalculator::STATUS_LUC,				"LUC");
	m_CalcParser.InsertCommand(CCalculator::OPERATOR_OPENPAREN,		"(");		// operator
	m_CalcParser.InsertCommand(CCalculator::OPERATOR_CLOSEPAREN,	")");
	m_CalcParser.InsertCommand(CCalculator::OPERATOR_PLUS,			"+");
	m_CalcParser.InsertCommand(CCalculator::OPERATOR_MINUS,			"-");
	m_CalcParser.InsertCommand(CCalculator::OPERATOR_MULTIPLY,		"*");
	m_CalcParser.InsertCommand(CCalculator::OPERATOR_DEVIDE,		"/");
	m_CalcParser.InsertCommand(CCalculator::OPERATOR_SQUARE,		"^");
}

CCalculatorManager::~CCalculatorManager(void)
{
}

const char*	CalculationScriptFileName	= "Calculation.txt";

// implementation
BOOL CCalculatorManager::Initiate(void)
{
	CScriptFile	CalculationScript;
	if ( FALSE == CalculationScript.Open(CalculationScriptFileName, TRUE, GENERIC_READ, OPEN_EXISTING) ) {
		DWORD dwError	= ::GetLastError();
		return FALSE;
	}

	while ( CalculationScript.ReadLine() ) {
		char	CalculationBuffer[CScriptFile::MAX_COLUMN_LENGHT];
		if (FALSE == CalculationScript.ReadString("calculation", CalculationBuffer, CScriptFile::MAX_COLUMN_LENGHT))
			break;
		char	ExpressionBuffer[CScriptFile::MAX_LINE_LENGTH];
		if (FALSE == CalculationScript.ReadString("expression", ExpressionBuffer, CScriptFile::MAX_COLUMN_LENGHT))
			break;
 		if ( 0 == strcmp(CalculationBuffer, Calculation_HP) ) {
			m_CalcHP.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		} else if ( 0 == strcmp(CalculationBuffer, Calculation_SP) ) {
			m_CalcSP.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		} else if ( 0 == strcmp(CalculationBuffer, Calculation_LP) ) {
			m_CalcLP.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		} else if ( 0 == strcmp(CalculationBuffer, Calculation_HPRG) ) {
			m_CalcHPRG.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		} else if ( 0 == strcmp(CalculationBuffer, Calculation_SPRG) ) {
			m_CalcSPRG.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		} else if ( 0 == strcmp(CalculationBuffer, Calculation_ATT_MELEE) ) {
			m_CalcATT.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		} else if ( 0 == strcmp(CalculationBuffer, Calculation_DEF_MELEE) ) {
			m_CalcDEF.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		} else if ( 0 == strcmp(CalculationBuffer, Calculation_EXP) ) {
			m_CalcEXP.InitiateCalculator(&m_CalcParser, ExpressionBuffer);
		}
	}

	return TRUE;
}
