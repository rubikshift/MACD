#include <iostream>
#include <fstream>
#include <string>
#include<locale>

#define PERIOD12 12
#define PERIOD26 26
#define PERIOD9 9
#define SIZE 1000

struct comma_separator : std::numpunct<char>
{
	virtual char do_decimal_point() const override
	{
		return ',';
	}
};

void generateWeights(long double* weights, unsigned int period)
{
	long double w, alpha = 2 / (period + 1);
	w = 1 - alpha;
	weights[0] = 1;
	for (unsigned int i = 1; i < period + 1; i++)
	{
		weights[i] = w;
		w *= 1 - alpha;
	}
	long double sum = 0;
	for (unsigned int i = 0; i < period + 1; i++)
		sum += weights[i];

	weights[period + 1] = sum; //na koncu tablicy z wagami - suma wag, dla optymalizacji obliczen
}

long double calcEMA(const long double* data, unsigned int start, unsigned int period, const long double* weights)
{
	long double EMA = 0;
	if (start < period)
		return EMA; // co w przypadku, gdy okres wyszedlby za tablice?
	for (unsigned int i = 0; i < period + 1; i++)
		EMA += weights[i] * data[start - i];
	EMA /= weights[period + 1];
	return EMA;
}

long double calcMACD(const long double* data, unsigned int i, const long double* weights12, const long double* weights26)
{
	long double EMA12 = calcEMA(data, i, PERIOD12, weights12);
	long double EMA26 = calcEMA(data, i, PERIOD26, weights26);
	return EMA12 - EMA26;
}

int main()
{
	long double data[SIZE], MACD[SIZE];
	long double weights12[PERIOD12 + 2], weights26[PERIOD26 + 2], weights9[PERIOD9 + 2]; // tablice z wagami, dal optymalizacji obliczen
	long double signal;

	//init
	generateWeights(weights12, PERIOD12);
	generateWeights(weights26, PERIOD26);
	generateWeights(weights9, PERIOD9);

	std::ofstream macdFile("macd.txt", std::ios::out);
	std::ofstream signalFile("signal.txt", std::ios::out);
	signalFile.imbue(std::locale(std::cout.getloc(), new comma_separator));
	macdFile.imbue(std::locale(std::cout.getloc(), new comma_separator));


	for (unsigned int i = 0; i < SIZE; i++)
		std::cin >> data[i];
	for (unsigned int i = 0; i < SIZE; i++)
	{
		MACD[i] = calcMACD(data, i, weights12, weights26);
		signal = calcEMA(MACD, i, PERIOD9, weights9);
		macdFile << MACD[i] << "\n";
		signalFile << signal << "\n";
	}

	macdFile.close();
	signalFile.close();
	return 0;
}