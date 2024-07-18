#pragma once

#include <vector>
#include <stack>


typedef class CCommandParser*	LPCCommandParser;
class CCommandParser;
typedef class CDelimiter*		LPCDelimiter;
class CDelimiter;


typedef class CCalElement*		LPCCalElement;
class CCalElement {
public:
	// constructor and destructor
	CCalElement(void)						{ m_ElementValue.m_nValue	= 0;									}
	CCalElement(CCalElement& rCalElement)	{ m_ElementValue.m_nValue	= rCalElement.m_ElementValue.m_nValue;	}
	CCalElement(int nValue)					{ m_ElementValue.m_nValue	= nValue;								}
	CCalElement(float fValue)				{ m_ElementValue.m_fValue	= fValue;								}
	~CCalElement(void)						{}

	union {
		int		m_nValue;
		float	m_fValue;
	} m_ElementValue;
};


typedef class CCalculator*		LPCCalculator;
class CCalculator
{
public:
	enum { OPERANDCHECKING_VALUE = 0x80000000 };
	enum { STATUS_LEV = 0x80000001, STATUS_STR = 0x80000002, STATUS_DEX = 0x80000003, STATUS_CON = 0x80000004, 
		   STATUS_MEN = 0x80000005, STATUS_INT = 0x80000006, STATUS_LUC = 0x80000007, 
		   OPERATOR_OPENPAREN = 0x80000008, OPERATOR_CLOSEPAREN = 0x80000009, 
		   OPERATOR_PLUS = 0x80000010, OPERATOR_MINUS = 0x80000011, OPERATOR_MULTIPLY = 0x80000012, 
		   OPERATOR_DEVIDE = 0x80000013, OPERATOR_SQUARE = 0x80000014 };

	enum { TYPE_OPERAND, TYPE_OPERATOR, TYPE_OPENPAREN, TYPE_CLOSEPAREN, TYPE_SEPERATOR	};
	enum { PRECEDENCE_NONE, PRECEDENCE_LOW, PRECEDENCE_MIDDLE, PRECEDENCE_HIGH			};

private:
	typedef std::vector<LPCCalElement>	Vector;
	typedef std::stack<LPCCalElement>	Stack;
	Vector			m_CalcVector;
	Stack			m_OperatorStack;

	int				ParseCalculation(LPCCommandParser pParser, LPCDelimiter pDelimiter, int* pnStatusIndex);
	void			ProcessOperand(int nOperand);
	void			ProcessCloseParen(void);
	void			ProcessOperator(int nOperand);
	inline float	ConvertOperand(LPCCalElement pElement, float* pStatusArray);
	inline BOOL		IsConstant(int nOperand);

public:
	// constructor and destructor
	CCalculator(void);
	~CCalculator(void);

	void			Terminate(void);
	inline int		TypeOfCommand(int nCommand);
	inline int		PrecedenceOfOperator(int nOperator);
	float			Calculate(float* pStatusArray);

	void			InitiateCalculator(LPCCommandParser pParser, CHAR* pCalculation);

	//*// test interface
	void		Print(void);
	void		OperatePrint(LPCCalElement pElement);
	//*/
};

int	CCalculator::TypeOfCommand(int nCommand)
{
	switch ( nCommand ) {
	case ' ':
	case '\t':
		return CCalculator::TYPE_SEPERATOR;
	case CCalculator::OPERATOR_OPENPAREN:
		return CCalculator::TYPE_OPENPAREN;
	case CCalculator::OPERATOR_CLOSEPAREN:
		return CCalculator::TYPE_CLOSEPAREN;
	case CCalculator::OPERATOR_PLUS:
	case CCalculator::OPERATOR_MINUS:
	case CCalculator::OPERATOR_MULTIPLY:
	case CCalculator::OPERATOR_DEVIDE:
	case CCalculator::OPERATOR_SQUARE:
		return CCalculator::TYPE_OPERATOR;
	default:
		return CCalculator::TYPE_OPERAND;
	}
}

int	CCalculator::PrecedenceOfOperator(int nOperator)
{
	switch ( nOperator ) {
	case CCalculator::OPERATOR_PLUS:
	case CCalculator::OPERATOR_MINUS:
		return CCalculator::PRECEDENCE_MIDDLE;
	case CCalculator::OPERATOR_MULTIPLY:
	case CCalculator::OPERATOR_DEVIDE:
	case CCalculator::OPERATOR_SQUARE:
		return CCalculator::PRECEDENCE_HIGH;
	default:
		return CCalculator::PRECEDENCE_NONE;
	}
}

float CCalculator::ConvertOperand(LPCCalElement pElement, float* pStatusArray)
{
	switch ( pElement->m_ElementValue.m_nValue ) {
	case CCalculator::STATUS_LEV:	return pStatusArray[0];
	case CCalculator::STATUS_STR:	return pStatusArray[1];
	case CCalculator::STATUS_DEX:	return pStatusArray[2];
	case CCalculator::STATUS_CON:	return pStatusArray[3];
	case CCalculator::STATUS_MEN:	return pStatusArray[4];
	case CCalculator::STATUS_INT:	return pStatusArray[5];
	case CCalculator::STATUS_LUC:	return pStatusArray[6];
	default:						return pElement->m_ElementValue.m_fValue;
	}
}

BOOL CCalculator::IsConstant(int nOperand)
{
	if ( ( nOperand & OPERANDCHECKING_VALUE) == OPERANDCHECKING_VALUE ) {
		return FALSE;
	} else {
		return TRUE;
	}
}
