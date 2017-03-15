#pragma once
#include <phidget21.h>
#include "vector3.h"

class PhidgetSpatial
{

 public:

    static PhidgetSpatial* instance();

    bool initialize();
    bool initialize(int data_rate, const int& timeout);

    bool attatched() const;

    int GetLastError() const;

    Vector3<double> acceleration() const;
    Vector3<double> angular_rate() const;
    Vector3<double> magnetic_field() const;

 private:

    const int kDataRateDefault = 8;
    const int kTimeoutDefault = 0;

    const int kDataRateMin = 4;
    const int kDataRateMax = 496;

    static PhidgetSpatial* instance_;

    CPhidgetSpatialHandle handle;

    Vector3<double> acceleration_;
    Vector3<double> angular_rate_;
    Vector3<double> magnetic_field_;

    bool attatched_;
    int error_;

    PhidgetSpatial();

    static int __stdcall AttatchHandler(CPhidgetHandle handle, void* user_ptr);
    static int __stdcall DetachHandler(CPhidgetHandle handle, void* user_ptr);
    static int __stdcall ErrorHandler(CPhidgetHandle handle, void* user_ptr, int error, const char *unknown);
    static int __stdcall DataHandler(CPhidgetSpatialHandle handle, void* user_ptr, CPhidgetSpatial_SpatialEventDataHandle* data, int packets);

};
