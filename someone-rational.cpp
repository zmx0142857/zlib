#include <iostream>
#include <cmath>
using namespace std;

class Rational {
private:
	int numerator;//分子
	int denominator;//分母
	int gcd(int i1,int i2);//求最大公约数
	int lcf(int i1,int i2) const;//最小公倍数
	void standardize(void);//标准化分数
public:

	//默认构造函数（将分数初始值设为1）
	Rational(void) {
		numerator = 1;
		denominator = 1;
	}
	//拷贝构造函数
	Rational(const Rational& rational) {
		this->numerator = rational.getNumerator();
		this->denominator = rational.getDenominator();
	}
	Rational(int numerator, int denominator) {
		this->numerator = numerator;
		this->denominator = denominator;
		standardize();
	}
	Rational(double xiaoshu);

	//将get方法都声明Rational的const成员，供const Rational的对象调用（在运算符重载中有用到）
	double getValue() const{
		return (double(numerator)) / denominator;
	}
	int getNumerator() const{
		return numerator;
	}
	void setNumerator(int numerator) {
		this->numerator = numerator;
	}
	int getDenominator() const {
		return denominator;
	}
	void setDenominator(int denominator) {
		this->denominator = denominator;
	}
	void reduce();
	friend ostream& operator<<(ostream& out,Rational& rat);
	friend istream& operator>>(istream& in,Rational& rat);
	friend Rational operator+(const Rational& rat1,const Rational& rat2);
	friend Rational operator-(const Rational& rat1,const Rational& rat2);
	friend Rational operator-(const Rational& rat);
	friend Rational operator*(const Rational& rat1,const Rational& rat2);
	friend Rational operator/(const Rational& rat1,const Rational& rat2);
	operator float() {return this->getValue();}
};

Rational::Rational(double xiaoshu) {
	int d = 1;
	double x = abs(xiaoshu);
	while((int)(x*10)/10.0 > (int)x)
	{
		x *= 10.0;
        	d *= 10;
	}
	int cd = gcd((int)x, d);
	numerator = (int)x / cd;
	denominator = d / cd;
	if (xiaoshu < 0)
	numerator = -numerator;
}

	//计算两个正整数的“最大公约数”
int Rational::gcd(int i1,int i2) {
	i1 = abs(i1);
	i2 = abs(i2);
	int min = i1 < i2 ? i1 : i2;
	int max = i1 > i2 ? i1 : i2;
	for(int i=min; i > 1; i=min) {
		if(min%i == 0 && max%i == 0) {
			return i;
		}
		min = max % i;
		max = i;
	}
	return 1;
}

//计算两个正整数的“最小公倍数”
int Rational::lcf(int i1,int i2) const {
	i1 = abs(i1);
	i2 = abs(i2);
	int max = i1 > i2 ? i1 : i2;
	for(int i=max; i < i1*i2; i++) {
		if(i%i1 == 0 && i%i2 == 0) {
			return i;
		}
	}
	return i1*i2;
}

void Rational::standardize(void) {
	if ((numerator < 0 && numerator*denominator > 0) || (numerator > 0 && numerator*denominator < 0)) {
		numerator = -numerator;
		denominator = -denominator;
	}
}

void Rational::reduce()
{
	standardize();
	int cd = gcd(numerator,denominator);
	numerator /= cd;
	denominator /= cd;
}

ostream& operator<<(ostream& out,Rational& rational)
{
	out << rational.getNumerator() << '/' << rational.getDenominator();
	return out;
}
istream& operator>>(istream& in,Rational& rational)
{
	char c;
	int i1;
	int i2;
	cin >> i1 >> c >> i2;
	rational.setNumerator(i1);
	rational.setDenominator(i2);
	rational.reduce();
	return in;
}

Rational operator+(const Rational& rat1,const Rational& rat2)
{
	Rational ratAdd;
	int lcf = rat1.lcf(rat1.getDenominator(), rat2.getDenominator());
	int i1 = lcf / rat1.getDenominator();
	int i2 = lcf / rat2.getDenominator();
	ratAdd.setNumerator(rat1.getNumerator()*i1 + rat2.getNumerator()*i2);
	ratAdd.setDenominator(lcf);
	ratAdd.reduce();
	return ratAdd;
}
Rational operator-(const Rational& rat1,const Rational& rat2)
{
	Rational ratMinus;
	int lcf = rat1.lcf(rat1.getDenominator(), rat2.getDenominator());
	int i1 = lcf / rat1.getDenominator();
	int i2 = lcf / rat2.getDenominator();
	ratMinus.setNumerator(rat1.getNumerator()*i1 - rat2.getNumerator()*i2);
	ratMinus.setDenominator(lcf);
	ratMinus.reduce();
	return ratMinus;
}
Rational operator-(const Rational& rat)
{
	Rational rational(-rat.getNumerator(), rat.getDenominator());
	return rational;
}
Rational operator*(const Rational& rat1,const Rational& rat2)
{
	Rational ratMultiply;
	ratMultiply.setNumerator(rat1.getNumerator() * rat2.getNumerator());
	ratMultiply.setDenominator(rat1.getDenominator() * rat2.getDenominator());
	ratMultiply.reduce();
	return ratMultiply;
}
Rational operator/(const Rational& rat1,const Rational& rat2)
{
	Rational ratDivide;
	ratDivide.setNumerator(rat1.getNumerator() * rat2.getDenominator());
	ratDivide.setDenominator(rat1.getDenominator() * rat2.getNumerator());
	ratDivide.reduce();
	return ratDivide;
}




int main(){

	Rational f1(1,5);
	Rational f2(0.4);
	//运算符重载测试
	Rational f3 = f1 + f2;
	Rational f4 = f2 - f1;
	Rational f5 = f1 * f2;
	Rational f6 = f1 / f2;
	//输出流运算符重载测试
	cout << f3 << ' ' << f4 << ' ' << f5 << ' ' << f6 << endl;
	cout << f2.getValue() << endl;
	Rational rat1;
	//输入流测试
	cin >> rat1;
	Rational rat2(-0.2);
	//自定义类型转换（有理数转实数）
	cout << float(-rat1) << ' ' << rat1.getValue() << endl;
	//负号重载（求反）
	cout << -rat2 << ' ' << rat2.getValue() << endl;
	return 0;
}
