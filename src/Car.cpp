#include <iostream>
#include <string>
#include <vector>
#include "Car.h"

namespace Entities {


Car::Car(const std::string& name, int health, int armour) : name_(name), health_(health), armour_(armour), engineStart_(false) 
{
    std::cout << "Car created: " << name_ << " with health: " << health_ << " and armour: " << armour_ << std::endl;
}

Car::~Car() {
    if (engineStart_) {
        std::cout << "Stopping engine of " << name_ << " before destruction." << std::endl;
        engineStart_ = false;
        std::cout << name_ << "'s engine stopped." << std::endl;
        std::cout << "Car destroyed: " << name_ << std::endl;
    }
}

// Public Member Function Implementation

void Car::StartEngine() {
    if (engineStart_) {
        std::cout << name_ << "'s engine is already running." << std::endl;
    } else {
        engineStart_ = true;
    }
    std::cout << name_ << "'s engine started." << std::endl;
}

// Accelerate
void Car::Accelerate(double amount) {
    if (engineStart_) {
        VehicleState_.speed_mps += amount; // Simple acceleration model: speed increases by amount * time
        std::cout << name_ << " accelerated by " << amount << " mps." << "New speed: " << VehicleState_.speed_mps << " mps." << std::endl;
    } else {
        std::cout << name_ << "'s engine is off. Cannot accelerate." << std::endl;
    }
}

// Brake
void Car::Brake() {
    if (VehicleState_.speed_mps > 0) {
        VehicleState_.speed_mps = 0; // Simple brake, stops immediately
        std::cout << name_ << " braked. Speed is now 0 mps." << std::endl;
    } else {
        std::cout << name_ << " is already stopped." << std::endl;
    }
}

// Display Speed
void Car::DisplaySpeed() const {
    std::cout << name_ << " current speed: " << VehicleState_.speed_mps << " mps." << std::endl;
}

void Car::UpdatePosition(double latitude, double longitude) {
    VehicleState_.latitude = latitude;
    VehicleState_.longitude = longitude;
    std::cout << name_ << " position updated to Latitude: " << latitude << ", Longitude: " << longitude << std::endl;

}

void Car::UpdateHeading(double heading_interval) {
    VehicleState_.heading = VehicleState_.heading + heading_interval;
    std::cout << name_ << " heading updated to: " << VehicleState_.heading << " degrees." << std::endl;
}

void Car::DisplayVehicleState() const {
    std::cout << name_ << " Vehicle State:" << std::endl;
    std::cout << "  Latitude: " << VehicleState_.latitude << std::endl;
    std::cout << "  Longitude: " << VehicleState_.longitude << std::endl;
    std::cout << "  Heading: " << VehicleState_.heading << " degrees" << std::endl;
    std::cout << "  Speed: " << VehicleState_.speed_mps << " mps" << std::endl;
}

void Car::MoveForward(double dtSeconds) {

    double distance = VehicleState_.speed_mps * dtSeconds;

    // Simple movement model: move forward based on current heading and speed
    double radians = VehicleState_.heading * (3.14159265358979323846 / 180.0); // Convert heading to radians
    double deltaLatitude = distance * cos(radians) / 111320; // Approximate conversion from meters to degrees latitude
    double deltaLongitude = distance * sin(radians) / (111320 * cos(VehicleState_.latitude * (3.14159265358979323846 / 180.0))); // Approximate conversion from meters to degrees longitude

    VehicleState_.latitude += deltaLatitude;
    VehicleState_.longitude += deltaLongitude;

    // Car::DisplayVehicleState();
}


// Getter for VehicleState
VehicleState Car::getVehicleState() const {
    return VehicleState_;
}
}