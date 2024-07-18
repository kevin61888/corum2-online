
#include	<cmath>
#include	<conio.h>
#include	<cstdlib>
#include	<string>
#include	<iostream>
#include	<windows.h>

#define _MAX_LENGTH_ 26

class CMoneyString
{
private:
    LONGLONG amount;
    char amountStr[_MAX_LENGTH_ - 1];
    void format();
    void insertComma();

public:
	
	static const int MAX_LENGTH = _MAX_LENGTH_;

    CMoneyString(LONGLONG);

	char* GetMoneyString() {
		return amountStr; 
	}
	//ALLOWS THE SUBTRACTION OF TWO AMOUNTS
    CMoneyString operator -(CMoneyString &money)
	{
        amount = amount - money.amount;
        return CMoneyString(amount);
    }
    //ALLOWS ADDITION OF TWO AMOUNTS
    CMoneyString operator +(CMoneyString &money)
    {
        amount = amount + money.amount;
        return CMoneyString(amount);
    }
};

//ONE ARGUMENT CONSTRUCTOR
CMoneyString :: CMoneyString(LONGLONG cash)
{
    amount = cash;

	for(int i=0; i<=CMoneyString::MAX_LENGTH - 1; i++)
    {
        amountStr[i] = '\0';
    }
    format();
}

//CONVERT THE AMOUNT TO STRING AND FORMAT IT
void  CMoneyString :: format()
{
    _i64toa(amount, amountStr, 10);
    insertComma();
}

//INSERT THE COMMAS IN THE STRING
void CMoneyString :: insertComma()
{
	for(int loc=strlen(amountStr)-1, counter=1; loc>0; --loc,++counter)//loc : 문자열 내의 위치
	{
		if(counter==3) 
		{
			counter = 0;
			for(int i=strlen(amountStr); i>loc; --i)
			{
				amountStr[i] = amountStr[i-1];
			}
			amountStr[loc] = ',';
		}	        
	}
}


void main()
{
    //Create and initialize objects
    CMoneyString sales(75453.89);
    CMoneyString purchases(60278.50);
    CMoneyString small4(1000);
    CMoneyString small3(100);
    CMoneyString small2(10);
    CMoneyString small1(1);
	CMoneyString some0(10000);
	CMoneyString some1(100000);
	CMoneyString some2(1000000);
	CMoneyString some3(21909387000);
	CMoneyString big(0xffffffffffffffffL);
    //CMoneyString profit;
    //Using overloaded operator calculate profit
    CMoneyString profit = sales - purchases;
	std::cout << "\nSales: " <<	sales.GetMoneyString();
    std::cout << "\nPurchase : " <<	purchases.GetMoneyString();
    std::cout << "\nProfit: " <<	profit.GetMoneyString();
	std::cout << "\nSmall4: " << small4.GetMoneyString();
	std::cout << "\nSmall3: " << small3.GetMoneyString();
	std::cout << "\nSmall2: " << small2.GetMoneyString();
	std::cout << "\nSmall1: " << small1.GetMoneyString();
	std::cout << "\nSome0: " << some0.GetMoneyString();
	std::cout << "\nSome1: " << some1.GetMoneyString();
	std::cout << "\nSome2: " << some2.GetMoneyString();
	std::cout << "\nSome3: " << some3.GetMoneyString();
	std::cout << "\nBig: " << big.GetMoneyString();
    std::cout << std::endl;
    getch();
}

