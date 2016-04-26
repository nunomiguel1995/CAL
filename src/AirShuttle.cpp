#include "AirShuttle.h"
#include "Exceptions.h"

using namespace std;

AirShuttle:: AirShuttle(){}

vector<Passenger> AirShuttle:: getPassengers(){
	vector<Passenger> p;
	for(int i=0; i< reservations.size();i++){
		p.push_back(reservations[i].getPassenger());
	}
	return p;
}
vector<Reservation> AirShuttle:: getReservations(){
	return reservations;
}
vector<Van> AirShuttle:: getVans(){
	return vans;
}
bool AirShuttle:: addReservation(Reservation r){
	for(int i=0; i< reservations.size(); i++){
		if(reservations[i]==r) {
			throw DuplicateReservationException(r);
			return false;
		}
	}

	reservations.push_back(r);
	return true;
}
bool AirShuttle:: removeReservation(int id){
	int i;
	bool exist=false;
	for(i=0;i < reservations.size();i++){
		if(reservations[i].getId()== id){
			exist=true;
			break;
		}
	}
	if(exist)
		reservations.erase(reservations.begin()+i);
	else{
		throw InexistentReservationException(id);
		return false;
	}
	return true;
}
bool AirShuttle:: addVan(Van v){
	for(int i=0; i< vans.size(); i++){
		if(vans[i]==v){
			throw DuplicateVanException(v);
		}
	}
	vans.push_back(v);
	return true;
}
bool AirShuttle:: removeVan(Van v){
	int i;
	bool exist=false;
	for(i=0;i < vans.size();i++){
		if(vans[i]== v){
			exist=true;
			break;
		}
	}

	if(exist)
		vans.erase(vans.begin()+i);
	else{
		throw InexistentVanException(v);
		return false;
	}
	return true;
}

bool AirShuttle:: changePassengerNIF(string oldNif, string newNif){
	unsigned int i;
	bool found=false;

	for(i=0; i< reservations.size(); i++){
		if(reservations[i].getPassenger().getNif() == oldNif){
			found=true;
			break;
		}
	}
	if(found){
		Passenger p = reservations[i].getPassenger();
		p.setNif(newNif);
		reservations[i].setPassenger(p);
	}
	else{
		throw InexistentPassengerException();
		return false;
	}

	return true;
}
bool AirShuttle:: changePassengerName(string oldName, string newName){
	unsigned int i;
	bool found=false;

	for(i=0; i< reservations.size(); i++){
		if(reservations[i].getPassenger().getName() == oldName){
			found=true;
			break;
		}
	}
	if(found){
		Passenger p = reservations[i].getPassenger();
		p.setName(newName);
		reservations[i].setPassenger(p);
	}
	else{
		throw InexistentPassengerException();
		return false;
	}

	return true;
}

void AirShuttle:: setReservations(vector<Reservation> res){
	this->reservations=res;
}

void AirShuttle:: distributePassengers(){
	int i=0, van=0;
	int waitTime = 30;//minutes
	Date d;

	for(;i<reservations.size();i++){
		int seats = vans[van].getPassengers();
		vector <Reservation> temp;
		temp.push_back(reservations[i]);
		d= Date(reservations[i].getDate().getHour(),reservations[i].getDate().getMinutes());
		bool wait = true;
		int j= i+1;
		while(seats > 0 && wait && j<reservations.size()){
			int dif = ((reservations[j].getDate().getHour() * 60)+ reservations[j].getDate().getMinutes()) -
					((d.getHour() * 60)+ d.getMinutes());
			if(dif>30){
				wait=false;
			}
			else{
				temp.push_back(reservations[j]);
				d=  Date(reservations[j].getDate().getHour(),reservations[j].getDate().getMinutes());
				seats--;
				j++;
			}
			if(van >= vans.size())
				van=0;
		}
		vans[van].addDate(d);
		vans[van].addReservations(temp);
		van++;
		i=j-1;
	}
}

vector<Reservation> AirShuttle:: getReservationByDestination(string destination){
	vector<Reservation> res;
	for(unsigned int i = 0; i < reservations.size(); i++){
		if(reservations[i].getDestination() == destination){
			res.push_back(reservations[i]);
		}
	}
	return res;
}


vector<Reservation> AirShuttle:: getReservationByDate(Date &d){
	vector<Reservation> res;
	for(unsigned int i = 0; i < reservations.size(); i++){
		if(reservations[i].getDate() == d) res.push_back(reservations[i]);
	}
	return res;
}

void AirShuttle:: sortDistributions(Graph<Node,Road> &g, unsigned int vanNumber){
	unsigned int cnt = 0;
	vector<Node> fullPath;
	g.dijkstraShortestPath(g.getVertex(1)->getInfo());

	vector<vector<Reservation> > res = vans[vanNumber-1].getReservations();
	vector<Reservation> final;

	for(unsigned int r = 0; r < res.size(); r++){
		vector<Reservation> path = res[r];
		vector<Reservation> trip;
		vector<Node> temp;
		Date pDate;

		fullPath = g.getPath(g.getVertex(1)->getInfo(), g.getVertex(path[cnt].getDestination())->getInfo());
		g.dijkstraShortestPath(g.getVertex(path[cnt].getDestination())->getInfo());

		for(unsigned int p = 0; p < path.size(); p++){
			if(cnt + 1 < path.size()){
				temp = g.getPath(g.getVertex(path[cnt].getDestination())->getInfo(), g.getVertex(path[cnt+1].getDestination())->getInfo());
				cnt++;
				g.dijkstraShortestPath(g.getVertex(path[cnt].getDestination())->getInfo());
			}
			for(unsigned int f = 0; f < fullPath.size(); f++){
				for(unsigned int t = 0; t < temp.size(); t++){
					if(fullPath[f].getHotelName() == temp[t].getHotelName()){
						temp.erase(temp.begin(), temp.begin() + t); //apaga se tiver no path inicial
					}
				}
			}
			for(unsigned int o = 1; o <= temp.size(); o++){
				fullPath.push_back(temp[o - 1]);
			}

			if(fullPath[fullPath.size() - 1].getNodeId() == fullPath[fullPath.size()-2].getNodeId()){
				fullPath.pop_back();
			}

			pDate = path[p].getDate();
			trip = getReservationByDate(pDate);
			for(unsigned int fin = 0; fin < trip.size(); fin ++){
				final.push_back(trip[fin]);
			}

			vans[vanNumber-1].setSortedTrips(r,final);
		}
	}
}


