#pragma once
#include "Vehicle.h"   // NEW type: no existing class is modified
class ElectricBus : public MotorVehicle {
    int capacity; static constexpr double PER_KWH = 20;
public:
    ElectricBus(std::string r, double rate, double lvl, Date due, int cap) : MotorVehicle(std::move(r), rate, lvl, due), capacity(cap) {}
    double charge(int d, int l, double kwh) const override { return base(d, l) + kwh * PER_KWH; }
    std::string type() const override { return "ElectricBus"; }
};
