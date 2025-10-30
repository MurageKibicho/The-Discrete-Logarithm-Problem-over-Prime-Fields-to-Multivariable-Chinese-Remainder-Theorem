#include <stdio.h>
#include <stdint.h>

//Modular exponentiation (a^b mod m)
int64_t ModularExponent(int64_t a, int64_t b, int64_t m)
{
	int64_t result = 1;
	a = a % m;
	while (b > 0)
	{
		if(b & 1){result = (result * a) % m;}
		a = (a * a) % m;
		b = b >> 1;
	}
	return result;
}

int64_t ModularInverse(int64_t a, int64_t m)
{
	int64_t m0 = m, t, q;
	int64_t x0 = 0, x1 = 1;

	if (m == 1) return 0;
    
	while (a > 1) 
	{
		q = a / m;
		t = m;
		m = a % m;
		a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}
    
	if(x1 < 0) x1 += m0;
	return x1;
}

int64_t Q(int64_t x, int64_t p, int64_t q)
{	
	//Ensure x is coprime to p * q
	if(x % (p * q) == 0)
	{
		printf("Error: x must be coprime with p*q\n");
		return -1;
	}
	int64_t modulusSquare = p * p * q * q;
	int64_t modulusCube   = p * p * p * q * q * q;
	int64_t totientQ      = q - 1;
	int64_t exponentOfX   = p * q * totientQ;//pqφ(q)
	
	int64_t lhs = ModularExponent(x, exponentOfX, modulusCube) - 1;
	
	int64_t q_x = (lhs / modulusSquare) % (p * q);
	return q_x;
}

void SetupMultivariableCRT(int64_t a0, int64_t b0, int64_t a1, int64_t b1, int64_t p, int64_t q)
{
	//coefficient of n0 = b₀^φ(q) / a₀^φ(q)
	int64_t totientQ = q - 1;
	int64_t nCoefficient0 = (ModularExponent(b0, totientQ, p) * a1 * ModularInverse(ModularExponent(a0, totientQ, p), p)) % p;
	//coefficient of n1 = a1
	int64_t nCoefficient1 = a1 % q;
	
	int64_t rhs0 = b1 % p;
	int64_t rhs1 = b1 % q;

	printf("\nMultivariable CRT System\n");
	printf("β_n + %ldn  ≡  %ld mod %ld\n", nCoefficient0, rhs0, p);
	printf("β_n + %ldn  ≡  %ld mod %ld\n", nCoefficient1, rhs1, q);
}

void TestExample()
{
	int64_t a_0 = 2;
	int64_t b_0 = 4;
	int64_t p = 11; //prime number
	int64_t q = (p-1) / 2;
	
	int64_t q_x_a = Q(a_0, p, q);
	int64_t q_x_b = Q(b_0, p, q);


	int64_t totientQ      = q - 1;
	int64_t a_1 = (-1 * (ModularExponent(a_0, totientQ, p * q)) * q_x_a) % (p * q) ; 
	//Handle negative modulo
	if(a_1 < 0){a_1 = ((p*q) + a_1) % (p * q); }
	
	int64_t b_1 = (-1 * (ModularExponent(b_0, totientQ, p * q)) * q_x_b) % (p * q) ; 
	//Handle negative modulo
	if(b_1 < 0){b_1 = ((p*q) + b_1) % (p * q);}
	
	printf("q(%ld) = %ld, q(%ld) = %ld | a1 = %ld, b1= %ld\n", a_0, q_x_a, b_0, q_x_b, a_1, b_1);
	
	SetupMultivariableCRT(a_0, b_0, a_1, b_1, p, q);
}


int64_t main()
{
	TestExample();
}
