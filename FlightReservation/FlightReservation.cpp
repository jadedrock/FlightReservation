// FlightReservation.cpp : 
// Defines the entry point for the console application.

#include "stdafx.h"
#include "FltReservations.h"
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

void menu()
{//prints out a menu of options is displayed for various functions
	cout << endl << "Welcome to the flight Reservation program" << endl;
	cout << "V: Avaliable Flights numbers" << endl;
	cout << "R: Make a Reservation" << endl;
	cout << "C: Cancel a Reservation" << endl;
	cout << "A: Cancel all Reservation" << endl;
	cout << "D: Cancel a Flight" << endl;
	cout << "I: Inquire about a Passenger" << endl;
	cout << "F: Inquire about a Flight" << endl;
	cout << "P: Print All Flight Infomation" << endl;
	cout << "M: Make a Flight" << endl;
	cout << "Q: Quit the program" << endl;
}

void Print(FltReservations& myAirport)
{//will print out all of the data in the flight tree
 //pre: at least 1 flight is in the tree
 //post: all of the flight nodes in the tree is displayed
	myAirport.print_all_of_the_flights();
}

void Make_Flight(FltReservations& myAirport)
{
	//will make a new flight
	//pre:non
	//post: a new flight is added to myAirport

	size_t flight = 0, seats = 0;

	cout << "Enter the flight number you wish to make: ";
	cin >> flight;
	cout << endl << "Enter the number of seats this flight has: ";
	cin >> seats;

	if (myAirport.add_new_flight(flight, seats))
		cout << endl << "Flight added" << endl;
	else
		cout << endl << "Flight cannot be added" << endl;


}

void Inquire_Passenger(FltReservations& myAirport)
{// will display a passenger bookings
 // pre: passenger exist in the hash table some where
 // post: all of the flights that the passenger is booked on is displayed
	string name;
	char temp[80];

	cout << endl << "Enter your name: ";

	cin.ignore(80, '\n');
	cin.getline(temp, 80, '\n');

	name = string(temp);
	myAirport.passenger_info(name);
}

void Inquire_Flight(FltReservations& myAirport)
{ //will display all of the infomation about a flight
  //pre: that flight should exist in the flight tree
  //post: the flight data has been outputed

	size_t number;
	cout << " Enter the number of the flight you wish to learn about: ";
	cin >> number;
	myAirport.flight_search(number);
}

void Make_Reservation(FltReservations& myAirport)
{//will make reservations for the user
 //pre:user wants to make a reservation and the flight should exist
 //post: the users request has been added to the flight tree if the flight
 //      exists
	string name;
	char temp[80];
	bool status;
	size_t number;
	cout << endl << "Enter your name: ";

	cin.ignore(80, '\n');
	cin.getline(temp, 80, '\n');
	cout << endl << "now enter the flight number: ";
	cin >> number;
	name = string(temp);
	
	//try to book the passenger on the flight
	myAirport.makeReservation(name, number, status);

	//we just write out if the passinger is booked or not
	cout << "Passinger " << name;

	if (status)
		cout << " was booked on flight ";
	else
		cout << " was not booked on flight ";

	cout << number << endl;
}
void Print_Flight(FltReservations& myAirport)
{//will print out all of the avaliable flights, and if they are open
 //pre: myAirport has a list of flight numbers
 //post:all of the flight numbers have been outputed to the screen and
 //     wheather they are full or open
	myAirport.fltprintnum();
}

void Cancel_All_Reservation(FltReservations& myAirport)
{//will cancel all the reservation for an individual
 //pre: individual should be a passinger that is on a flight or on a
 //     waitlist
 //post: that passenger and any flights he is booked on is cancled
	string name;
	char temp[80];
	size_t number;
	cout << endl << "Enter your name: ";

	cin.ignore(80, '\n');
	cin.getline(temp, 80, '\n');
	name = string(temp);//typecast string(char);
	myAirport.cancelAll(name);
}

void Cancel_Flight(FltReservations& myAirport)
{// Will cancel a flight and cancel all of its passenger bookings
 // on that flight
 // pre: flight must exist in myAirport
 // post: fight is canceled, and all of its passengers, and waitlisted
 //       passengers are canceled as well
	size_t number;
	cout << " Enter the number of the flight you wish to Cancel: ";
	cin >> number;
	myAirport.cancelFlt(number);
}

void Cancel_Reservation(FltReservations& myAirport)
{//Cancels a single passenger's reservation
 //pre: passenger exist in myAirport
 //post: passenger is expunged from that flight and the next passenger
 //      is on booked status or still waitlisted
	string name;
	char temp[80];
	size_t number;
	cout << endl << "Enter your name: ";

	cin.ignore(80, '\n');
	cin.getline(temp, 80, '\n');
	cout << endl << "Now enter the flight number: ";
	cin >> number;
	name = string(temp);
	if (myAirport.cancel(name, number))
		cout << name << "(s) Reservation was canceled";
	else
		cout << name << " is not booked for this flight";
}

int main()
{

	char choice;
	FltReservations myAirport; //Load flight and Passenger data

	//run the program till the usere enters q to quit
	do
	{
		//print out the menu of choices and prompt the user for a choice
		menu();
		cout << "Command: ";
		cin >> choice;

		// call the function that the user pick, else say invaled choice
		switch (toupper(choice))
		{
		case 'A':Cancel_All_Reservation(myAirport); break;
		case 'D':Cancel_Flight(myAirport); break;
		case 'C':Cancel_Reservation(myAirport); break;
		case 'V':Print_Flight(myAirport); break;
		case 'P':Print(myAirport); break;
		case 'R':Make_Reservation(myAirport); break;
		case 'I':Inquire_Passenger(myAirport); break;
		case 'F':Inquire_Flight(myAirport); break;
		case 'M':Make_Flight(myAirport); break;
		case 'Q':break;

		default: cout << "Invalid Choice" << endl;
		};

	} while (toupper(choice) != 'Q');

	system("pause");
    return 0;
}

