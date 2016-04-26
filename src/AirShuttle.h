#ifndef SRC_AIRSHUTTLE_H_
#define SRC_AIRSHUTTLE_H_
#include <iostream>
#include <vector>
#include "Graph.h"
#include "Node.h"
#include "Road.h"
#include "Reservation.h"
#include "Van.h"
#include "Passenger.h"

using namespace std;

class AirShuttle{
private:
	vector<Reservation> reservations;
	vector<Van> vans;

public:
	AirShuttle();
	vector<Passenger> getPassengers();
	vector<Reservation> getReservations();
	vector<Van> getVans();
	bool addReservation(Reservation r);
	bool removeReservation(int id);
	bool addVan(Van v);
	bool removeVan(Van v);
	vector<Reservation> getReservationByDate(Date &d);
	vector<Reservation> getReservationByDestination(string destination);
	vector<Reservation> getReservationByPassenger(Passenger p);
	bool changePassengerNIF(string oldNif, string newNif);
	bool changePassengerName(string oldName, string newName);
	void distributePassengers();
	void setReservations(vector<Reservation> res);
	void sortDistributions(Graph<Node,Road> &g, unsigned int vanNumber);
	vector<Reservation> sortTrips(vector<Node> path, Date pDate);
};


#endif /* SRC_AIRSHUTTLE_H_ */
