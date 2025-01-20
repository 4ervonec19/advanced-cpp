#include "crew_members.hpp"

int Unit::currentID = 0;

// Реализация конструктора Pilot
Pilot::Pilot() : Unit("PILOT") {}

// Реализация методов Pilot
int Pilot::getHandLuggageWeight() const { return 0; }
int Pilot::getLuggageWeight() const { return 0; }
bool Pilot::canRegister() const { return true; }

// Реализация конструктора FlightAttendant
FlightAttendant::FlightAttendant() : Unit("FLIGHT_ATTENDANT") {}

// Реализация методов FlightAttendant
int FlightAttendant::getHandLuggageWeight() const { return 0; }
int FlightAttendant::getLuggageWeight() const { return 0; }
bool FlightAttendant::canRegister() const { return true; }