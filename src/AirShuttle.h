#ifndef SRC_AIRSHUTTLE_H_
#define SRC_AIRSHUTTLE_H_
#include <iostream>
#include <vector>
#include "Reservation.h"
#include "Van.h"
#include "Passenger.h"

using namespace std;

class AirShuttle{
private:
	vector<Reservation> reservations;
	vector<Van> vans;
public:
	AirShuttle(vector<Reservation> reservations, vector<Van> vans);
	vector<Passenger> getPassengers();
	vector<Reservation> getReservations();
	vector<Van> getVans();
	bool addReservation(Reservation r);
	bool removeReservation(Reservation r);
	bool addVan(Van v);
	bool removeVan(Van v);
	vector<Reservation> getReservation(Date d);

};


#endif /* SRC_AIRSHUTTLE_H_ */