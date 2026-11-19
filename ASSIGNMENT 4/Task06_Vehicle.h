#pragma once
#include <iostream>
#include <string>
struct Date { int y, m, d;                       // member class (has-a)
    std::string str() const { return std::to_string(y) + "-" + std::to_string(m) + "-" + std::to_string(d); } };

class Vehicle {                                  // abstract base
protected:
    std::string regNo; double dailyRate; bool rented = false;
    static constexpr double LATE_FACTOR = 1.5;
public:
    Vehicle(std::string r, double rate) : regNo(std::move(r)), dailyRate(rate) {}
    virtual ~Vehicle() = default;
    virtual double charge(int days, int lateDays, double consumed) const = 0;   // pure virtual
    virtual std::string type() const = 0;
    virtual void describe() const { std::cout << type() << " " << regNo << " rate=" << dailyRate << (rented ? " [RENTED]" : " [available]") << "\n"; }
    const std::string& reg() const { return regNo; }
    bool isRented() const { return rented; }
    void setRented(bool r) { rented = r; }
protected:
    double base(int days, int lateDays) const { return days * dailyRate + lateDays * dailyRate * LATE_FACTOR; }
};

class Bicycle : public Vehicle {
public:
    using Vehicle::Vehicle;
    double charge(int d, int l, double) const override { return base(d, l); }
    std::string type() const override { return "Bicycle"; }
};

class MotorVehicle : public Vehicle {            // adds level + service date
protected:
    double level; Date serviceDue;
public:
    MotorVehicle(std::string r, double rate, double lvl, Date due) : Vehicle(std::move(r), rate), level(lvl), serviceDue(due) {}
    void describe() const override { Vehicle::describe(); std::cout << "   level=" << level << " service due " << serviceDue.str() << "\n"; }
};

class TwoWheeler : public MotorVehicle {
    static constexpr double PER_LITRE = 100;
public:
    using MotorVehicle::MotorVehicle;
    double charge(int d, int l, double litres) const override { return base(d, l) + litres * PER_LITRE; }
    std::string type() const override { return "TwoWheeler"; }
};

class Car : public MotorVehicle {
protected:
    int seats; static constexpr double PER_LITRE = 100;
public:
    Car(std::string r, double rate, double lvl, Date due, int s) : MotorVehicle(std::move(r), rate, lvl, due), seats(s) {}
    double charge(int d, int l, double litres) const override { return base(d, l) + litres * PER_LITRE; }
    std::string type() const override { return "Car"; }
};

class ElectricCar : public Car {
    double batteryKwh; static constexpr double PER_KWH = 12;
public:
    ElectricCar(std::string r, double rate, double lvl, Date due, int s, double kwh) : Car(std::move(r), rate, lvl, due, s), batteryKwh(kwh) {}
    double charge(int d, int l, double kwh) const override { return base(d, l) + kwh * PER_KWH; }
    std::string type() const override { return "ElectricCar"; }
};
