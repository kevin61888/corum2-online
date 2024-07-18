#include "../../StdAfx.h"
#include "Calculator.h"

#include <BaseLibrary/Utility/Etc/CommandParser.h>
#include <BaseLibrary/Utility/Script/Delimiter.h>

//#include <BaseLibrary/Utility/Debugging/DebugUtils.h>
//#define new New


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CCalculator::CCalculator(void)
{
}

CCalculator::~CCalculator(void)
{
	Terminate();
}

void CCalculator::Terminate(void)
{
	while ( m_CalcVector.size() ) {
		LPCCalElement	pElement	= m_CalcVector[0];
		m_CalcVector.erase(m_CalcVector.begin());
		SAFE_DELETE(pElement);
	}
	while ( !m_OperatorStack.empty() ) {
		LPCCalElement	pElement	= m_OperatorStack.top();
		m_OperatorStack.pop();
		SAFE_DELETE(pElement);
	}
}

void CCalculator::InitiateCalculator(LPCCommandParser pParser, CHAR* pCalculation)
{
	CDelimiter	Delimiter(pCalculation, "+-*/()^ \t\0");

	int	nStatusIndex;
	while ( int nCommandSize = ParseCalculation(pParser, &Delimiter, &nStatusIndex) ) {
		switch ( TypeOfCommand(nStatusIndex) ) {
		case CCalculator::TYPE_OPENPAREN:
			m_OperatorStack.push(new CCalElement(nStatusIndex));
			break;
		case CCalculator::TYPE_CLOSEPAREN:
			ProcessCloseParen();
			break;	// pop + print until matching open paren
		case CCalculator::TYPE_OPERATOR:
			ProcessOperator(nStatusIndex);
			break;
		case TYPE_SEPERATOR:	// 진짜 seperator 는 리턴값이 0xFFFFFFFF
			if ( nCommandSize != 0xFFFFFFFF )
				ProcessOperand(nStatusIndex);
			break;
		default:
			ProcessOperand(nStatusIndex);
			break;
		}
	}

	while ( !m_OperatorStack.empty() ) {
		LPCCalElement pElement	= m_OperatorStack.top();
		m_OperatorStack.pop();
		if ( CCalculator::TYPE_OPENPAREN == pElement->m_ElementValue.m_nValue ) {
			DEBUGLOG0("error-unmatched open paren");
		}
		m_CalcVector.push_back(pElement);
	}
}

int CCalculator::ParseCalculation(LPCCommandParser pParser, LPCDelimiter pDelimiter, int* pnStatusIndex)
{
	CHAR	Command[MAX_PATH];
	CHAR	cToken			= 0;
	int		nCommandSize	= 0;

	if ( pDelimiter->ReadString(Command, MAX_PATH, &cToken) || cToken ) {
		switch ( cToken ) {
		case ' ':
		case '\t':
			*pnStatusIndex	= cToken;
			nCommandSize	= 0xFFFFFFFF;
			break;
		default:
			if ( 0 == ( nCommandSize = pParser->ParseCommand(Command, pnStatusIndex) ) ) {
				*pnStatusIndex	= atoi(Command);
				nCommandSize	= static_cast<int>(strlen(Command));
			}
			break;
		}
	}

	return nCommandSize;
}

void CCalculator::ProcessOperand(int nOperand)
{
	if ( IsConstant(nOperand) )
		m_CalcVector.push_back(new CCalElement(static_cast<float>(nOperand)));
	else
		m_CalcVector.push_back(new CCalElement(nOperand));
}

void CCalculator::ProcessCloseParen(void)
{
	while ( !m_OperatorStack.empty() ) {
		LPCCalElement	pElement	= m_OperatorStack.top();
		m_OperatorStack.pop();

		if ( CCalculator::OPERATOR_OPENPAREN == pElement->m_ElementValue.m_nValue ) {
			SAFE_DELETE(pElement);
			break;
		} else {
			m_CalcVector.push_back(pElement);
		}
	}
}

void CCalculator::ProcessOperator(int nOperand)
{
	bool bDone	= false;
	while ( !m_OperatorStack.empty() && !bDone ) {
		LPCCalElement	pElement	= m_OperatorStack.top();
		if ( ( CCalculator::TYPE_OPENPAREN == pElement->m_ElementValue.m_nValue ) || 
			 (PrecedenceOfOperator(pElement->m_ElementValue.m_nValue) >= PrecedenceOfOperator(nOperand)) ) {
			m_OperatorStack.pop();
			m_CalcVector.push_back(pElement);
		} else {
			bDone	= true;
		}
	} // end while
	//while ( !m_OperatorStack.empty() ) {
	//	LPCCalElement	pElement	= m_OperatorStack.top();
	//	if ( ( CCalculator::TYPE_OPENPAREN == pElement->m_ElementValue.m_nValue ) || 
	//		 (PrecedenceOfOperator(pElement->m_ElementValue.m_nValue) >= PrecedenceOfOperator(nOperand)) ) {
	//		m_OperatorStack.pop();
	//		m_CalcVector.push_back(pElement);
	//	} else {
	//		break;
	//	}
	//} // end while

	m_OperatorStack.push(new CCalElement(nOperand));
}

float CCalculator::Calculate(float* pStatusArray)
{
	Stack	OperationStack;

	for ( Vector::size_type i=0; i<m_CalcVector.size(); ++i ) {
		switch ( TypeOfCommand(m_CalcVector[i]->m_ElementValue.m_nValue) ) {
		case CCalculator::TYPE_OPERATOR:
			{
				LPCCalElement pRHSElement	= OperationStack.top();
				OperationStack.pop();
				float fRHS	= ConvertOperand(pRHSElement, pStatusArray);
				LPCCalElement pLHSElement	= OperationStack.top();
				OperationStack.pop();
				float fLHS	= ConvertOperand(pLHSElement, pStatusArray);
				SAFE_DELETE(pRHSElement);
				SAFE_DELETE(pLHSElement);

				LPCCalElement	pElement	= m_CalcVector[i];
				switch ( pElement->m_ElementValue.m_nValue ) {
				case CCalculator::OPERATOR_PLUS:		fLHS += fRHS;	break;
				case CCalculator::OPERATOR_MINUS:		fLHS -= fRHS;	break;
				case CCalculator::OPERATOR_MULTIPLY:	fLHS *= fRHS;	break;
				case CCalculator::OPERATOR_DEVIDE:		fLHS /= fRHS;	break;
				//case CCalculator::OPERATOR_SQUARE:	fLHS += fRHS;	break;
				default:												break;
				}
				OperationStack.push(new CCalElement(fLHS));

			} break;
		default:
			LPCCalElement	pElement	= m_CalcVector[i];
			OperationStack.push(new CCalElement(*pElement));
			break;
		}
	}

	if ( 1 != OperationStack.size() ) {
		return 0;
	} else {
		LPCCalElement	pElement	= OperationStack.top();
		OperationStack.pop();
		float	fRet	= pElement->m_ElementValue.m_fValue;
		SAFE_DELETE(pElement);
		return fRet;
	}
}

//*// test interface
void CCalculator::Print(void)
{
	for ( Vector::size_type i=0; i<m_CalcVector.size(); ++i ) {
		printf("0x%08x ", m_CalcVector[i]->m_ElementValue.m_nValue);
	}
	printf("\n");

	for ( Vector::size_type i=0; i<m_CalcVector.size(); ++i ) {
		OperatePrint( m_CalcVector[i] );
	}
	printf("\n");
}

void CCalculator::OperatePrint(LPCCalElement pElement)
{
	if ( CCalculator::OPERATOR_OPENPAREN == pElement->m_ElementValue.m_nValue ) {
		printf(" ( ");
	} else if ( CCalculator::OPERATOR_CLOSEPAREN == pElement->m_ElementValue.m_nValue ) {
		printf(" ) ");
	} else if ( CCalculator::OPERATOR_PLUS == pElement->m_ElementValue.m_nValue ) {
		printf(" + ");
	} else if ( CCalculator::OPERATOR_MINUS == pElement->m_ElementValue.m_nValue ) {
		printf(" - ");
	} else if ( CCalculator::OPERATOR_MULTIPLY == pElement->m_ElementValue.m_nValue ) {
		printf(" * ");
	} else if ( CCalculator::OPERATOR_DEVIDE == pElement->m_ElementValue.m_nValue ) {
		printf(" / ");
	} else if ( CCalculator::OPERATOR_SQUARE == pElement->m_ElementValue.m_nValue ) {
		printf(" ^ ");
	} else if ( CCalculator::STATUS_LEV == pElement->m_ElementValue.m_nValue ) {
		printf(" LEV ");
	} else if ( CCalculator::STATUS_STR == pElement->m_ElementValue.m_nValue ) {
		printf(" STR ");
	} else if ( CCalculator::STATUS_DEX == pElement->m_ElementValue.m_nValue ) {
		printf(" DEX ");
	} else if ( CCalculator::STATUS_CON == pElement->m_ElementValue.m_nValue ) {
		printf(" CON ");
	} else if ( CCalculator::STATUS_MEN == pElement->m_ElementValue.m_nValue ) {
		printf(" MEN ");
	} else if ( CCalculator::STATUS_INT == pElement->m_ElementValue.m_nValue ) {
		printf(" INT ");
	} else if ( CCalculator::STATUS_LUC == pElement->m_ElementValue.m_nValue ) {
		printf(" LUC ");
	} else {
		printf("%f", pElement->m_ElementValue.m_fValue);
	}
}
//*/
