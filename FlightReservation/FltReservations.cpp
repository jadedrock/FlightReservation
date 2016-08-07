#include "FltReservations.h"


#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <list>

using namespace std;

FltReservations::FltReservations()
{
	//load all of the passenger and flight data
	loadReservations();
}

FltReservations::~FltReservations()
{//the destructor
}


passenger::passenger(string s, int flight, bool seated)
{// the constructor for passenger
	name = s;
	flt = flight;
	booked = seated;
	prev = NULL;
	next = NULL;
}

passenger& passenger:: operator =(const passenger& p)
{ //the overloded = for passenger

	if (this == &p)
		return *this;

	name = p.name;
	flt = p.flt;
	booked = p.booked;
	prev = p.prev;
	next = p.next;
	return *this;
}
fltnode::fltnode(int flt, int seats, int ct) :fltnum(flt),
				 capacity(seats), count(ct), bookedhead(NULL), 
				 bookedlast(NULL), waithead(NULL), waitlast(NULL), 
				waitlist_count(0) {}

ostream& operator<< (ostream& outs, const passenger& pass)
{//the overloded << for passenger to output name

	outs << pass.name;
	return outs;
}

ostream& operator<< (ostream& outs, const fltnode& rhs)
{//the over loaded << for flight node
 //post: prints out all of the data associated with that node

	outs << "Flight " << rhs.fltnum << " has " << rhs.capacity <<
		" seats with " << rhs.count << " seat(s) reserved." << endl;

	if (rhs.bookedhead != NULL&&rhs.bookedlast != NULL)
	{
		outs << "Currently the people booked on this flight are: ";

		for (pptr temp = rhs.bookedhead; temp != NULL; temp = temp->next)
			outs << temp->name << ", ";

		outs << endl;

		if (rhs.waithead != NULL)
		{
			outs << "The current number of people on the waitlist are: ";

			//prints out all of the names on the waitlist
			for (pptr temp = rhs.waithead; temp != NULL; temp = temp->next)
			{
				outs << temp->name << " ,";
			} //end for
		} //end if rhs.waithead != NULL
	} // end if bookedhead == null
	return outs;
}

void FltReservations::fltprintnum()
{
	//will go thru the entire bst of flt nodes and print out all
	//of the information
	//pre: tree should have at least 1 flight node in it
	//post: the entire bst tree has been outputed with all the flights
	//      and passenger booked and waitlisted

	cout << "Avalable Flights:" << endl;

	set<fltnode>::iterator itr;//makes an iterator

							   //goes through each node
	for (itr = tree.begin(); itr != tree.end(); ++itr)
	{
		cout << (*itr).fltnum;
		if ((*itr).count >= (*itr).capacity)
			cout << " Flight Full, Waitlist has " << (*itr).waitlist_count
			<< " people on it" << endl;
		else
			cout << " Flight open" << endl;
	}
}

int FltReservations::hash(const string& key) const
{//the hash function that will hash a name and return the value
 //pre: key contains some name or string
 //post: the number that the string equates to is returned

	int hashVal = 0;
	for (int i = 0; i < key.length(); i++)
		hashVal = hashVal * 37 + key[i];

	hashVal %= TABLE_SIZE;
	if (hashVal < 0)
		hashVal += TABLE_SIZE;
	return hashVal;
}

bool FltReservations::add_new_flight(const int flt_num, const int seats)
{
	//add a new branch to the tree of flight nodes
	//pre:flt_num contanes the number of the flight and seates contanins
	//    the number of seats we wish to add to the new node
	//post: the new flight is added to the bst tree.

	tree.insert(fltnode(flt_num, seats));
	return true;

}

void FltReservations::add_passenger_at_rear(passenger& pass, passenger*
	&front, passenger* &rear)
{ //will add a passenger to the end of some list
  //pre rear will point to the end of some list
  //post rear has one new node added to it with the contents of passenger

	if (front == NULL&&rear == NULL)
	{
		passenger *insert_ptr;

		insert_ptr = new passenger(pass.name, pass.flt, pass.booked);

		insert_ptr->next = front;
		insert_ptr->prev = front;
		front = insert_ptr;
		rear = front;
	}
	else
	{
		passenger * newnode = new
			passenger(pass.name, pass.flt, pass.booked);

		newnode->next = NULL;
		newnode->prev = rear;
		rear->next = newnode;
		rear = newnode;
	}
}

bool FltReservations::makeReservation(string& name, int flt,
	bool& reservation_status)
{ //will make a new passenger and add him or her to an existing flight
  //pre: name contains the passenger name and flt, contains the flight
  //     number the passenger wants to get on.
  //post: the flight tree has been updated with the new entry is exists
  //      and reservation_status contains true if the passenger is booked
  //      and false if the flight does not exist or he was waitlisted

	set<fltnode>::iterator itr;

	// then we need to figure out if the flight even exists
	itr = tree.find(fltnode(flt));

	if (itr == tree.end())
	{
		cout << endl << "Flight does not exist" << endl;
		return false;
	}
	else
	{
		//we need to check to see if the passenger is already booked
		//for the flight by going through the hash table

		list<passenger>::iterator location;
		location = find(hashTable[hash(name)].begin(),
			hashTable[hash(name)].end(), passenger(name, flt));

		//is the person aready exists then just return
		if (location != hashTable[hash(name)].end())
		{
			cout << "Passenger already exists on this flight" << endl;
			reservation_status = true;
			return true;
		}

		//the passenger has been excepted so we are going to add
		//it to the flight
		passenger a_passenger(name, flt);

		//the flight exists but now we check to see if a seat is open
		fltnode temp = *itr; // will make a shallow copy

		if (temp.count<temp.capacity)
		{
			//then we can add to that flight
			a_passenger.booked = reservation_status = true;

			//link a passenger to the end of the chain
			add_passenger_at_rear(a_passenger, temp.bookedhead,
				temp.bookedlast);

			//adds one more to the seat count
			temp.count++;
		}
		else
		{
			//the flight is full and we need to add the passenger to the wait
			//list
			a_passenger.booked = reservation_status = false;//not booked

															//puts the entry at the end of the waitlist queue
															//temp.waithead.push(a_passenger);
															//enques the passenger into the waitlist

			add_passenger_at_rear(a_passenger, temp.waithead,
				temp.waitlast);
			temp.waitlist_count++;
			cout << "The passenger is added to the waitlist. Thus\n";
		}//end else

		 // now fix the list of flight nodes with the new entry
		tree.erase(fltnode(flt));
		tree.insert(temp);

		//now we need to update the master list of names
		hashTable[hash(a_passenger.name)].push_front(a_passenger);

	}// end else
	return true;
}//end make resiverations

void FltReservations::loadReservations()
{//will load all of the passenger and flight data
 //pre: no pre conditions, called upon construction of fltresivation
 //post: flight reservation class has now a bunch of new flights and
 //      passengers added to each one of those flights

	string word; // the name of the passenger
	int numb; //the flight or flight number he is on
	int seats;//the number of seats on the plane
	bool status;// the passenger booked status
	char temp[80];//a text line to hold our infomation
	string fltinfo;//a text line to hold our infomation
	ifstream infile, infile2;// the file we wull use for input data

	infile.open("flights.data");

	//checks to see if the filename exists
	if (infile.fail())
	{
		cout << endl << "flights.data file not found." << endl;
		return;
	}

	//reads all of the flight numbers and seats number
	while (infile >> numb&&infile >> seats)
	{
		add_new_flight(numb, seats);
		cout << "flight " << numb << " was added with " << seats << " seats." << endl;
	}

	infile.close();//close the file

				   //open the next file for reading
	infile2.open("passengers.data");

	//checks to see if the filename exists
	if (infile2.fail())
	{
		cout << endl << "passengers.data file not found." << endl;
		return;
	}

	//reads in the entire name, and flight number
	while (infile2.getline(temp, 80, '\n'))
	{
		fltinfo = temp;//typecasts

		string::size_type pos = 0;
		pos = fltinfo.find_first_of(',', pos);
		if (pos != string::npos)
		{
			//edit the input stream to get the name of the passenger
			//and his or hers flight number
			word = fltinfo.substr(0, pos);
			numb = atoi(fltinfo.substr(pos + 2).c_str());

			//try to book the passinger on the flight
			makeReservation(word, numb, status);

			//we just write out if the passenger is booked or not
			cout << "Passenger " << word;

			if (status)
				cout << " was booked on flight ";
			else
				cout << " was not booked on flight ";

			cout << numb << endl;
		}
	}
	infile2.close();
}

bool FltReservations::cancel(const string& name, int flt)
{//will cancle a flight and move the next passenger up
 //pre: passenger must exist
 //post: passinger is removed from hash table and from flight
 //      returns false if flight does not exist or passenger does not
 //      exist. true if the passenger exists and is removed

	set<fltnode>::iterator itr;

	// then we need to figure out if the flight even exists
	itr = tree.find(fltnode(flt));

	if (itr == tree.end())
	{
		cout << endl << "Flight does not exist" << endl;
		return false;
	}
	else
	{
		fltnode temp = *itr; // will make a shallow copy
		list<passenger>::iterator listnode;
		if (cut_middle(passenger(name, flt), temp.bookedhead, temp.bookedlast))
		{
			temp.count--;

			pop_waitlist(temp);

			tree.erase(fltnode(flt));
			tree.insert(temp);

			//removes reservation from the hash table
			listnode = find(hashTable[hash(name)].begin(),
				hashTable[hash(name)].end(), passenger(name, flt));
			if (listnode != hashTable[hash(name)].end())
				hashTable[hash(name)].remove(*listnode);

			return true;
		}
		if (cut_middle(passenger(name, flt), temp.waithead, temp.waitlast))
		{
			tree.erase(fltnode(flt));
			tree.insert(temp);
			temp.waitlist_count--;

			//removes reservation from the hash table
			listnode = find(hashTable[hash(name)].begin(),
				hashTable[hash(name)].end(), passenger(name, flt));
			if (listnode != hashTable[hash(name)].end())
				hashTable[hash(name)].remove(*listnode);

			return true;
		}
		else
			return false;
	}
}

void FltReservations::cancelAll(const string& name)
{//cancels all of the reservation of a passenger
 //pre: name contans the name of a passenger in the hash table
 //post: that passenger is expunged from all of his flights and
 //      removed from the hash table

	list<passenger>::iterator itr;
	for (itr = hashTable[hash(name)].begin();
		itr != hashTable[hash(name)].end();  itr++)

		if ((*itr).name == name)
		{//will cancel the flight
			cout << (*itr).name << ", Flight: " << (*itr).flt << " Canceled" << endl;
			cancel((*itr).name, (*itr).flt);
			itr--;
		}
}

bool FltReservations::cancelFlt(int flt)
{ //Cancels a flight and cancels all of its passenger reservations
  //pre:flight must exist
  //post:flight is canceled, and all of its occupents are canceled

	set<fltnode>::iterator itr;

	// then we need to figure out if the flight even exists
	itr = tree.find(fltnode(flt));

	if (itr == tree.end())
	{
		cout << endl << "Flight does not exist" << endl;
		return false;
	}
	else
	{
		fltnode temp = *itr;
		for (int i = (temp.count + temp.waitlist_count); i >= 0; i--)
		{
			cancel(temp.bookedhead->name, flt);
		}
		tree.erase(fltnode(flt));
	}
	cout << endl << "Flight deleted" << endl;
	return true;
}

passenger* FltReservations::firstReservationOf(const string& name)
{//not used but implimented
 //finds the first occurence of name in the hash table
 //pre:name exits in the hash table
 //post:current_pptr containts a pointer to the locaton of a found
 //     passenger

	list<passenger>::iterator itr;
	for (itr = hashTable[hash(name)].begin();
		itr != hashTable[hash(name)].end();  itr++)

		if ((*itr).name == name)
		{
			(*current_pptr) = passenger((*itr).name, (*itr).flt);
			current_name = name;
		}
	return current_pptr;
}

void FltReservations::print_all_of_the_flights()
{ //will go thru the entire bst of fltnodes and print out all
  //of the information
  //pre: tree should have atleast 1 flight node in it
  //post: the entire bst tree has been outputed with all the flights
  //      and passingers booked and waitlisted

	set<fltnode>::iterator itr;//makes an iterator

							   //goes through each node
	for (itr = tree.begin(); itr != tree.end(); ++itr)
		cout << *itr << endl;
}

void FltReservations::passenger_info(const string& name)
{
	//will print out the infomation of a passenger
	//pre: this passenger exists in the hash table somewhere
	//post: the passenger is outputed with all of his flights

	list<passenger>::iterator itr;
	for (itr = hashTable[hash(name)].begin();
		itr != hashTable[hash(name)].end(); ++itr)

		if ((*itr).name == name)
		{
			cout << "Passenger: " << (*itr).name;
			if ((*itr).booked)
				cout << " is booked";
			else
				cout << " is waitlisted";

			cout << " for Flight: " << (*itr).flt;
			cout << endl;

		}
}

void FltReservations::pop_waitlist(fltnode &node)
{//will pop the waitlist and push it onto the reservation
 //pre: passenger list has atleast 1 open seat
 //post: waitlist has one less occupent and passenger list is full
	passenger *insert_ptr;

	if (node.waithead != NULL)
	{
		insert_ptr = node.waithead;//front;
		node.waithead = node.waithead->next;
		if (node.waithead != NULL)
			node.waithead->prev = NULL;
		else
		{
			node.waitlast = node.waithead = NULL;
		}
		node.waitlist_count--;

		//passenger is now booked
		insert_ptr->booked = true;
		//pushes it onto the bookhead
		add_passenger_at_rear((*insert_ptr), node.bookedhead,
			node.bookedlast);
		node.count++;
	}



}
bool FltReservations::cut_middle(const passenger& pass, passenger*
	&front, passenger* &rear)
{ //will seek a passenger then fix its pointers so that the
  // node before pass points to the node after pass, and vice versa
  // pre: passenger pass should exist i the list between front and rear
  // post: if pass= a node between front, and rear(like R), then the prevous
  // node points to the node after R, then R's prev and next ptrs are NULLED
	passenger *insert_ptr = front;
	//there is no list
	if ((front == NULL) && (rear == NULL))
		return false;

	//insert_ptr = new passenger(pass.name,pass.flt,pass.booked);
	while (((*insert_ptr) != pass) & (insert_ptr->next != NULL))
	{
		insert_ptr = insert_ptr->next;
	}

	//check to see if the entry is in the list
	if (insert_ptr == NULL)
		return false;

	if ((*insert_ptr) != pass)
		return false;


	if (insert_ptr->prev == NULL)
	{
		insert_ptr = front;
		front = front->next;
		if (front != NULL)
			front->prev = NULL;
		else
		{
			rear = front = NULL;
		}
	}
	else
	{

		//inserts prevous node's next equals insert's next;

		//link the prevous node with the node after insert_ptr
		//checks to see if we are at the end of the list
		if (insert_ptr->next != NULL)
			insert_ptr->prev->next = insert_ptr->next;
		else
		{
			rear = insert_ptr->prev;
			insert_ptr->prev->next = NULL;


		}
		//link the next node to the node before insert_ptr
		if (insert_ptr->next != NULL)
			insert_ptr->next->prev = insert_ptr->prev;
		// else
		//   insert_ptr->next->prev=NULL
	}
	insert_ptr->next = NULL;
	insert_ptr->prev = NULL;

	//node deleated
	return true;
}


void FltReservations::flight_search(const size_t flightnum)
{//will print out all of the information on a spefic flight number
 //pre: flight must exist
 //post: all of the flight contents is printed, names waitlist, capacity

	set<fltnode>::iterator itr;//makes an iterator
	itr = tree.find(fltnode(flightnum));
	if (itr == tree.end())
		cout << "That Flight does not exist" << endl;
	else
	{
		cout << "That flight exists: " << endl << *itr << endl;
	}
}
