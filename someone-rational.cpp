#include <iostream>
#include <cmath>
using namespace std;

class Rational {
private:
	int numerator;//����
	int denominator;//��ĸ
	int gcd(int i1,int i2);//�����Լ��
	int lcf(int i1,int i2) const;//��С������
	void standardize(void);//��׼������
public:

	//Ĭ�Ϲ��캯������������ʼֵ��Ϊ1��
	Rational(void) {
		numerator = 1;
		denominator = 1;
	}
	//�������캯��
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

	//��get����������Rational��const��Ա����const Rational�Ķ�����ã�����������������õ���
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

	//���������������ġ����Լ����
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

//���������������ġ���С��������
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
	//��������ز���
	Rational f3 = f1 + f2;
	Rational f4 = f2 - f1;
	Rational f5 = f1 * f2;
	Rational f6 = f1 / f2;
	//�������������ز���
	cout << f3 << ' ' << f4 << ' ' << f5 << ' ' << f6 << endl;
	cout << f2.getValue() << endl;
	Rational rat1;
	//����������
	cin >> rat1;
	Rational rat2(-0.2);
	//�Զ�������ת����������תʵ����
	cout << float(-rat1) << ' ' << rat1.getValue() << endl;
	//�������أ��󷴣�
	cout << -rat2 << ' ' << rat2.getValue() << endl;
	return 0;
}
