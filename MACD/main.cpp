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

void generateWeights(double* weights, const unsigned int period)
{
	double w, alpha = 2.0 / (period - 1.0);
	w = 1;
	for (unsigned int i = 0; i < period + 1; i++)
	{
		weights[i] = w;
		w *= (1.0 - alpha);
	}
	double sum = 0;
	for (unsigned int i = 0; i < period + 1; i++)
		sum += weights[i];

	weights[period + 1] = sum; //na koncu tablicy z wagami - suma wag, dla optymalizacji obliczen
}

double calcEMA(const double* data, const unsigned int start, const unsigned int period, const double* weights)
{
	double EMA = 0;
	for (unsigned int i = 0; i < period + 1; i++)
	{
		if (start < i)
			break;
		EMA += weights[i] * data[start - i];
	}
	EMA /= weights[period + 1];
	return EMA;
}

double calcMACD(const double* data, unsigned int i, const double* weights12, const double* weights26)
{
	double EMA12 = calcEMA(data, i, PERIOD12, weights12);
	double EMA26 = calcEMA(data, i, PERIOD26, weights26);
	return (EMA12 - EMA26);
}

int main()
{
	double data[SIZE], MACD[SIZE];
	double weights12[PERIOD12 + 2], weights26[PERIOD26 + 2], weights9[PERIOD9 + 2]; // tablice z wagami, dla optymalizacji obliczen
	double signal, histogram;

	//init
	generateWeights(weights12, PERIOD12);
	generateWeights(weights26, PERIOD26);
	generateWeights(weights9, PERIOD9);

	std::ofstream macdFile("macd.txt", std::ios::out);
	std::ofstream signalFile("signal.txt", std::ios::out);
	std::cin.imbue(std::locale(std::cout.getloc(), new comma_separator));
	signalFile.imbue(std::locale(std::cout.getloc(), new comma_separator));
	macdFile.imbue(std::locale(std::cout.getloc(), new comma_separator));


	for (unsigned int i = 0; i < SIZE; i++)
		std::cin >> data[i];
	for (unsigned int i = 0; i < SIZE; i++)
	{
		MACD[i] = calcMACD(data, i, weights12, weights26);
		signal = calcEMA(MACD, i, PERIOD9, weights9);
		histogram = MACD[i] - signal;
		macdFile << MACD[i] << "\n";
		signalFile << signal << "\n";
	}

	macdFile.close();
	signalFile.close();
	return 0;
}