#pragma once
enum Decision {SELL, BUY, WAIT_TO_SELL, WAIT_TO_BUY};

struct comma_separator : std::numpunct<char>
{
	virtual char do_decimal_point() const override
	{
		return ',';
	}
};

struct Log
{
	Decision decision;
	double price;
	double money;
	double treshold;
	Log(double price, Decision decision, double money, double treshold);
};

class Investor
{
	private:
		double treshold;
		double money;
		double startPrice;
		double lastPrice;
		double buyPrice;
		unsigned int stocks;

		void buy(double price);
		void sell(double price);
	public:
		Investor(double startPrice);
		~Investor();
		double getTreshold() const;
		void react(double price, double macd, double signal);
};