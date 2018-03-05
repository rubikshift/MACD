#include <cmath>
#include <iostream>
#include <fstream>
#include "investor.h"

Investor::Investor(double startPrice)
{
	money = 0;
	stocks = 1000;
	treshold = 0.75;
	this->startPrice = startPrice;
	lastPrice = startPrice;
}
Investor::~Investor()
{
	std::ofstream summary("investor.txt", std::ios::out);
	summary.imbue(std::locale(std::cout.getloc(), new comma_separator));
	double income = (stocks*lastPrice + money) / (startPrice * 1000);
	
	summary << "StartMoney: " << 0 << "\n";
	summary << "StartStocks: " << 1000 << "\n";
	summary << "StartPrice: " << startPrice << "\n";
	summary << "Start: " << 1000 * startPrice << "\n";

	summary << "\n";
	summary << "EndMoney: " << money << "\n";
	summary << "EndStocks: " << stocks << "\n";
	summary << "EndPrice: " << lastPrice << "\n";
	summary << "End: " << money + stocks*lastPrice << "\n";
	
	summary << "\n";
	summary << "Status: " << income * 100 << "%\n";

	summary.close();
}

void Investor::react(double price, double diff, double prevDiff)
{
	Decision decision;
	lastPrice = price;

	if (diff - prevDiff <= 0)
		decision = diff > 0 ? WAIT_TO_BUY : WAIT_TO_SELL;
	else if (diff - prevDiff > 0)
		return;

	if (money < price && decision == WAIT_TO_BUY)
		return;
	else if (stocks == 0 && decision == WAIT_TO_SELL)
		return;

	if (decision == WAIT_TO_BUY && abs(diff) < treshold)
		buy(price);
	else if (decision == WAIT_TO_SELL && abs(diff) < treshold)
		sell(price);
	return;
}

void Investor::buy(double price)
{
	unsigned int n = static_cast<unsigned int>(money / price);
	stocks += n;
	money -= n * price;
}

void Investor::sell(double price)
{
	unsigned int n = stocks;
	money += n * price;
	stocks -= n;
}