#ifndef SRC_VAN_H_
#define SRC_VAN_H_

#include <iostream>

using namespace std;

class Van{
private:
	string lPlate;
	int nPassengers;
public:
	Van(string lPlate, int nPassengers);
	string getLPlate() const;
	void setLPlate(const string& lPlate);
	int getPassengers() const;
	void setPassengers(int nPassengers);
};

#endif