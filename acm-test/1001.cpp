#include <iostream>
#include <string>
#include <cstring>
using namespace std;


#define LEN 128

class bignum;
class bignum
{
	private:
		char a[LEN];
		int decimal;
		int pos;
	public:
		bignum()
		{
			decimal = 0;
			pos = 0;
			memset(a,0,LEN*sizeof(char));
		}
		bignum(string str)
		{
			int rdecimal = 0;
			int mark = 0;
			decimal = 0;
			memset(a,0,LEN*sizeof(char));
			for(int i=1;i<=str.length();i++)
			{
				
				if((str.c_str())[str.length() - i] == '.')
				{
					rdecimal = i;
					mark = 1;
				}
				else {a[LEN-i+mark] = (str.c_str())[str.length() - i] - '0';}
			}
			pos = str.length();
			decimal = rdecimal;
		}
		 
		void printnum()
		{
			int iend = 0;
			while(a[LEN - iend + 1]==0)
			{
				iend++;
			}
			int downD = pos - decimal;
			if(downD < 0)
				cout << '.';
			for(int i = LEN-pos+1; i < LEN-iend; i++,downD--)
			{
				if(downD == 0)
					cout << '.';
				cout << (int) a[i];
			}
			cout << endl;
		}
		bignum operator+ (bignum &B)
		{
			bignum sum;
			sum.decimal = decimal > B.decimal ? decimal : B.decimal;
			sum.pos = (pos - decimal + sum.decimal) > (B.pos- B.decimal + sum.decimal) ? (pos - decimal + sum.decimal) : (B.pos- B.decimal + sum.decimal);
			int vi = decimal-sum.decimal;
			int vj = B.decimal-sum.decimal;
			//cout << vi << ' '<< vj << endl;
			int temp = 0;
			for(int i = 0; i < sum.pos; i++ ,vi++,vj++)
			{
				if(vi < 0)
				{
					sum.a[LEN-i-1] = B.a[LEN - vj - 1];
					//cout << (int)B.a[LEN-i-1] << endl;
					continue;
				}
				if(vj < 0)
				{
					sum.a[LEN-i-1] = a[LEN - vi - 1];
					//cout << "b    " << (int)a[LEN-i-1] << endl;
					continue;
				}
				temp = a[LEN - vi - 1] + B.a[LEN - vj - 1] + sum.a[LEN-i-1];
				sum.a[LEN-i-1] = temp%10;
				//cout << "c    "  <<  (int)a[LEN - vi - 1] <<"    " << (int)B.a[LEN - vj - 1] <<"    " << (int)sum.a[LEN-i-1]<< endl;
				sum.a[LEN-i-2] = temp/10;
				//cout << vi << ' '<< vj << endl;
			}
			return sum;
		}

		bignum& operator<< (int B)
		{
			int change = 0;
			if(decimal > B)
			{
				decimal -= B;
			}
			else
			{
				if (decimal == B)
				{
					decimal = 0;
				}
				else
				{
					change = B - decimal;
					decimal = 0;
					for (int i = LEN - pos + 1; i < LEN; i++)
					{
						a[i - change] = a[i];
						//cout <<"done\n";
					}
					for (int i = LEN - change ; i < LEN; i++)
					{
						a[i] = 0;
						//cout <<"done\n";
					}
					pos += change;
				}
			}
			//cout <<"*****done \n";
			return (*this);//.debug();
		}
		bignum operator* (int B)
		{
			if(B == 0)
			{
				return *(new bignum);
			}
			bignum temp = *this;
			int tempInt;
			for(int i = LEN - pos + 1; i < LEN; i++)
			{
				tempInt = temp.a[i]*B;
				temp.a[i] = tempInt %10;
				temp.a[i-1] += tempInt /10;
			}
			temp.pos+=1;
			return temp;
		}
		bignum operator* (bignum B)
		{
			bignum muti,temp = *this,temp2;
			
			//muti.pos = pos + B.pos+1;
			int j = 0;
			for(int i = LEN-1; i!= LEN - B.pos-1;i--,j++)
			{
				temp2 = temp * B.a[i];
				temp2 << j;
				muti = muti + temp2;
				//muti.debug();
			}
			muti.decimal = decimal + B.decimal-1;
			//muti.pos = pos + B.pos+1;
			return muti;
		}


		bignum& debug()
		{
			cout <<"|=" << this << "  start:=============="<<endl;
			cout <<"|=decimal:"<< decimal << " pos:"<<pos<<endl;
			for(int i=0;i<LEN;i++)
			{
				cout << (int)a[i]<<' ';
			}
			cout << endl;
			cout <<"||" << this << "  end:================"<<endl;
			return *this;
		}
		bignum& stdlize()
		{
			while(a[LEN - pos + 1]==0)
			{
				pos--;
				if(decimal > pos)
					break;
			}
			return *this;
		}
};

string stdlizeStr(string input)
{
	string out;
	auto i = input.begin;
	auto j = input.rbegin;
	for(; i != input.end; i++)
	{
		if(input[i]!=0)
			break;
	}
	input = input.substr(i);
	for(;j!=input.rend;j++)
	{
		if(input[j]!=0)
			break;
	}
	out = input.erase(j);
	return out;
}

int main()
{
	bignum a("0.01"),powAns,temp,one("1.0");
	int powers = 12;


	//one.debug().printnum();
	if(powers & 0x01 == 1)
		powAns = a;
	else 
		powAns = one;

	//powAns.debug().printnum();
	temp = a;
	for(int i = 1;i < 5;i++ )
	{
		temp = temp*temp;
		powers = powers >> 1;
		if(powers & 0x01 == 1)
		{
			powAns = powAns * temp;
		}
	}
	
	powAns.stdlize().debug().printnum();



	//(a<<1<<1<<1<<1<<1<<1<<1<<1<<1).debug().printnum();
	return 0;
}