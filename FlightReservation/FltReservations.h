#pragma once

#ifndef FltReservations_H
#define FltReservations_H

#include <string>
#include <set>
#include <cstddef>
#include <algorithm>
#include <list>
#include <queue>
#include <cmath>

using namespace std;

struct passenger;  //defined in implementation file

typedef passenger * pptr;

struct passenger
{
	string name;
	int flt;
	bool booked;
	pptr prev;   // pointer to previous node on same passenger or wait list
	pptr next;   // pointer to next node on same passenger or wait list

	friend ostream& operator<< (ostream& outs, const passenger& pass);
	passenger(string s = "", int flight = 0, bool seated = true);
	bool operator == (const passenger& p) const
	{
		return name == p.name&&flt == p.flt;
	}

	bool operator != (const passenger& p)
	{
		return ((name != p.name) ^ (flt != p.flt));
	}
	passenger& operator =(const passenger& p);

};

struct fltnode {  //defined in implementation file
	int fltnum;
	int capacity;  // capacity of plane
	int count;     // number of passengers on flight list
	pptr bookedhead;  // pointer to first passenger on flight list
	pptr bookedlast;//the end of the booked list
	pptr waithead; // pointer to first passenger on waiting
				   //list
	pptr waitlast;
	size_t waitlist_count;
	fltnode(int flt, int seats = 0, int ct = 0);

	bool operator != (const fltnode & Rhs) const
	{
		return fltnum != Rhs.fltnum;
	}
	bool operator == (const fltnode & Rhs) const
	{
		return fltnum == Rhs.fltnum;
	}
	bool operator < (const fltnode & Rhs) const
	{
		return fltnum <  Rhs.fltnum;
	}
	bool operator > (const fltnode & Rhs) const
	{
		return fltnum > Rhs.fltnum;
	}
};

#define TABLE_SIZE 811

class FltReservations
{

private:
	// The root pointer to the flight tree (a binary search tree)
	//Useing STL set class functons
	set<fltnode> tree;

	// The hash table that holds all passenger-reservation info
	//will be a linked list of passenger
	list<passenger> hashTable[TABLE_SIZE];

	//hash functon
	int hash(const string& key) const;

	// Data members used by firstReservationOf and nextReservationOf
	string current_name;
	passenger* current_pptr;

public:

	FltReservations();
	~FltReservations();

	bool add_new_flight(const int flt_num, const int seats);
	bool makeReservation(string& name, int flt, bool& reservation_status);
	bool cancelFlt(int flt);
	bool cancel(const string& name, int flt);

	void cancelAll(const string& name);
	void fltprintnum();

	void loadReservations();
	void print_all_of_the_flights();
	void passenger_info(const string& name);
	void flight_search(const size_t flightnum);

	passenger* firstReservationOf(const string& name);
	

	friend ostream& operator<< (ostream& outs, const FltReservations&
		flt_reservations);

	void pop_waitlist(fltnode &node);
	void add_passenger_at_rear(passenger& pass,
		passenger* &front,
		passenger*&rear);

	bool cut_middle(const passenger& pass,
		passenger* &front,
		passenger* &rear);
};

#endif
