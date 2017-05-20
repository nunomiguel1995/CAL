#include "main.h"
#include "FileReader.h"

using namespace std;

AirShuttle airShuttle;

/**Display all the reservations
 *
 */
void diplayReservations() {
	vector <Reservation> r = airShuttle.getReservations();

	for(unsigned int i=0; i < r.size(); i++){
		cout << "ID: "<< r[i].getId()<< "; Passenger:  "<<r[i].getPassenger().getName()<< " "<<r[i].getPassenger().getNif()
				<<"; Arrival: "<<r[i].getDate().getHour()<<":"<<r[i].getDate().getMinutes()
				<<" Destination: "<<r[i].getDestination()<< endl;
	}
	mainMenu();
}
/**Display all the vans
 *
 */
void displayVans(){
	vector<Van> v = airShuttle.getVans();
	for(unsigned int i=0; i < v.size(); i++){
		cout<<"License Plate: "<<v[i].getLPlate()<<" Capacity: "<<v[i].getPassengers()<<endl;
	}
	mainMenu();
}
/**Display all the trips by van, date and order of route
 *
 */
void displayTrips(){
	vector <Van> v = airShuttle.getVans();
	for (unsigned int i=0; i< v.size(); i++){
		cout<<"Van "<<v[i].getLPlate()<<endl;
		vector < vector <string> > trips = v[i].getTrips();
		vector < Date > dates= v[i].getD();
		for(unsigned int j=0; j < trips.size(); j++){
			cout<<dates[j].getHour()<<":"<<dates[j].getMinutes()<<"- ";
			for(unsigned int k=0; k < trips[j].size(); k++){
				if(k == trips[j].size() -1)
					cout<<trips[j][k];
				else
					cout<<trips[j][k]<<"->";
			}
			cout<<endl;
		}

	}

	mainMenu();
}
/**Display all the groups of passengers that travel together
 *
 */
void displayGroups(){
	vector <Van> v = airShuttle.getVans();

	for(unsigned int i=0; i< v.size(); i++){
		cout<<"Van: "<<v[i].getLPlate()<<endl;
		vector <vector <Reservation> > groups = v[i].getReservations();
		for(unsigned int j=0; j < groups.size(); j++ ){
			for(unsigned int k=0; k < groups[j].size(); k++){
				cout<<groups[j][k].getPassenger().getName()<<"; ";
			}
			cout<<endl;
		}
	}
	mainMenu();
}
/**Main menu Function
 *
 */
void mainMenu(){
	cout<<"1- Show all reservations \n";
	cout<<"2- Show all vans \n";
	cout<<"3- Show passenger groups \n";
	cout<<"4- Show trips \n";
	cout<<"5- Exit \n";

	int option;

	cin>> option;
	switch(option){
	case 1:
		diplayReservations();
		break;
	case 2:
		displayVans();
		break;
	case 3:
		displayGroups();
		break;
	case 4:
		displayTrips();
		break;
	case 5:
		return;
	default:
		cout<<"Invalid option. Insert new one \n";
		mainMenu();
		break;
	}
}

/**Main Funtion
 *
 */
int main(){
	Graph<Node,Road> g;
	loadGraph(g);
	airShuttle = AirShuttle();
	readReservations(airShuttle);
	readVans(airShuttle);
	airShuttle.distributePassengers();
	clock_t begin = clock();
	for(unsigned int i=0; i < airShuttle.getVans().size(); i++ )
		airShuttle.sortDistributions(g,i+1);
	clock_t end = clock();
	double elapsed = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Elapsed time: " << setprecision(10) << elapsed << endl;
	mainMenu();
	return 0;
}
