#include "AirShuttle.h"
#include "Exceptions.h"

using namespace std;
 /**Construtor of class AirShuttle
  * All date is initially null
  */
AirShuttle:: AirShuttle(){}

/**Returns all passengers
 *
 */
vector<Passenger> AirShuttle:: getPassengers(){
	vector<Passenger> p;
	for(int i=0; i< reservations.size();i++){
		p.push_back(reservations[i].getPassenger());
	}
	return p;
}
/**Returns all reservations
 *
 */
vector<Reservation> AirShuttle:: getReservations(){
	return reservations;
}
/**Returns all vans
 *
 */
vector<Van> AirShuttle:: getVans(){
	return vans;
}
/**Adds a reservation to reservations vector
 * Throws an exception in case it already exists
 * @param r reservation being added
 */
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
/**Removes a reservation to reservations vector
 * Throws an exception in case it doesn't exists
 * @param r reservation being removed
 */
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
/**Adds a van of vans vector
 * Throws an exception in case it already exists
 * @param v van being added
 */
bool AirShuttle:: addVan(Van v){
	for(int i=0; i< vans.size(); i++){
		if(vans[i]==v){
			throw DuplicateVanException(v);
		}
	}
	vans.push_back(v);
	return true;
}
/**Removes a van of vans vector
 * Throws an exception in case it doesn't exists
 * @param v van being removed
 */
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
/**Sets reservations vector
 * @param res new vector of reservations
 */
void AirShuttle:: setReservations(vector<Reservation> res){
	this->reservations=res;
}
/**Distributes all the passengers for the day in the active vans optimizing the waiting time and the number of passenger per van
 * A passenger never waits longer than 30 minutes
 */
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

		}
		vans[van].addDate(d);
		vans[van].addReservations(temp);
		if(van == vans.size() -1)
			van=0;
		else
			van++;
		i=j-1;
	}
}
/**Returns all the reservations with the same destination
 * @param destination destination being searched
 */
vector<Reservation> AirShuttle:: getReservationByDestination(string destination){
	vector<Reservation> res;
	for(unsigned int i = 0; i < reservations.size(); i++){
		if(reservations[i].getDestination() == destination){
			res.push_back(reservations[i]);
		}
	}
	return res;
}
/**Returns all the reservations with the same date
 * @param d date being searched
 */
vector<Reservation> AirShuttle:: getReservationByDate(Date &d){
	vector<Reservation> res;
	for(unsigned int i = 0; i < reservations.size(); i++){
		if(reservations[i].getDate() == d) res.push_back(reservations[i]);
	}
	return res;
}
/**Optimizes the route of wich group of passengers in a van
 * The passenger with the close destination is left off first and so on
 * @param g graph that represents the location map
 * @param vanNumber represents the van
 */
void AirShuttle:: sortDistributions(Graph<Node,Road> &g, unsigned int vanNumber){
	vector<vector<Reservation> > res = vans[vanNumber-1].getReservations();

	for(unsigned int r = 0; r < res.size(); r++){

		vector<Reservation> path = res[r];
		vector<Reservation> trip;
		vector<string> final;
		vector<Node> temp;
		vector<Node> fullPath;
		Date pDate;
		unsigned int cnt = 0;

		//g.dijkstraShortestPath(g.getVertex(1)->getInfo());
		g.aStarPath(g.getVertex(1)->getInfo(),g.getVertex(1)->getInfo());
		fullPath = g.getPath(g.getVertex(1)->getInfo(), g.getVertex(path[cnt].getDestination())->getInfo());
		for(unsigned int o = 1; o < fullPath.size(); o++){
			if(fullPath[o].getHotelName() != ""){
				final.push_back(fullPath[o].getHotelName());
			}
		}

		//g.dijkstraShortestPath(g.getVertex(path[cnt].getDestination())->getInfo());
		g.aStarPath(g.getVertex(path[cnt].getDestination())->getInfo(),g.getVertex(1)->getInfo());

		for(unsigned int p = 0; p < path.size(); p++){
			if(cnt + 1 < path.size()){
				temp = g.getPath(g.getVertex(path[cnt].getDestination())->getInfo(), g.getVertex(path[cnt+1].getDestination())->getInfo());
				cnt++;
				//g.dijkstraShortestPath(g.getVertex(path[cnt].getDestination())->getInfo());
				g.aStarPath(g.getVertex(path[cnt].getDestination())->getInfo(),g.getVertex(1)->getInfo());
			}
			for(unsigned int f = 0; f < fullPath.size(); f++){
				for(unsigned int t = 0; t < temp.size(); t++){
					if(fullPath[f].getNodeId() == temp[t].getNodeId()){
						temp.erase(temp.begin(), temp.begin() + t); //apaga se tiver no path inicial
					}
				}
			}
			for(unsigned int o = 1; o < temp.size(); o++){
				fullPath.push_back(temp[o]);
				if(temp[o].getHotelName() != ""){
					final.push_back(temp[o].getHotelName());
				}
			}
			if(fullPath[fullPath.size() - 1].getNodeId() == fullPath[fullPath.size()-2].getNodeId()){
				fullPath.pop_back();
				final.pop_back();
			}
		}
		vans[vanNumber-1].setSortedTrips(final);
	}
}
