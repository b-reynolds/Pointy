#include "phidget_spatial.h"
#include "vector3.h"

PhidgetSpatial* PhidgetSpatial::instance_ = nullptr;

/**
 * \brief Returns a pointer to the singleton PhidgetSpatial instance
 */
PhidgetSpatial* PhidgetSpatial::instance()
{
    if (instance_ == nullptr)
    {
        instance_ = new PhidgetSpatial();
    }
    return instance_;
}

/**
* \brief Attempts to initialize the PhidgetSpatial
*/
bool PhidgetSpatial::initialize()
{
    return initialize(kDataRateDefault, kTimeoutDefault);
}

/**
 * \brief Attempts to initialize the PhidgetSpatial for the the given timeout period
 * \param data_rate rate at which the Phidget recieves data (milliseconds)
 * \param timeout time period to wait for attatchment (milliseconds) (0 = Forever)
 */
bool PhidgetSpatial::initialize(int data_rate, const int& timeout)
{
    if(handle != nullptr)
    {
        CPhidget_close((CPhidgetHandle)handle);
        CPhidget_delete((CPhidgetHandle)handle);
    }

    // Create the spatial object
    CPhidgetSpatial_create(&handle);

    // Set the handlers for the device
    CPhidget_set_OnAttach_Handler(reinterpret_cast<CPhidgetHandle>(handle), AttatchHandler, this);
    CPhidget_set_OnDetach_Handler(reinterpret_cast<CPhidgetHandle>(handle), DetachHandler, this);
    CPhidget_set_OnError_Handler(reinterpret_cast<CPhidgetHandle>(handle), ErrorHandler, this);
    CPhidgetSpatial_set_OnSpatialData_Handler(handle, DataHandler, this);

    // Open the Phidget Manager
    CPhidget_open(reinterpret_cast<CPhidgetHandle>(handle), -1);

    // Wait for the Phidget to be attatched

    if (CPhidget_waitForAttachment(reinterpret_cast<CPhidgetHandle>(handle), timeout))
    {
        return false;
    }

    // Clamp the specified data rate to within the minimum and maximum range
    if (data_rate > kDataRateMax)
    {
        data_rate = kDataRateMax;
    }
    else if (data_rate < kDataRateMin)
    {
        data_rate = kDataRateMin;
    }

    // Set the data rate of the Phidget
    CPhidgetSpatial_setDataRate(handle, data_rate);

    attatched_ = true;

    return true;
}

/**
 * \brief Returns true if the Phidget is attatched
 */
bool PhidgetSpatial::attatched() const
{
    return attatched_;
}

/**
* \brief Returns the last error reproted by the Phidget
*/
int PhidgetSpatial::GetLastError() const
{
    return error_;
}

/**
* \brief Returns the Phidget's acceleration data
*/
Vector3<double> PhidgetSpatial::acceleration() const
{
    return acceleration_;
}

/**
* \brief Returns the Phidget's angular rate data
*/
Vector3<double> PhidgetSpatial::angular_rate() const
{
    return angular_rate_;
}

/**
* \brief Returns the Phidget's magnetic field data
*/
Vector3<double> PhidgetSpatial::magnetic_field() const
{
    return magnetic_field_;
}

PhidgetSpatial::PhidgetSpatial()
{
    handle = nullptr;
    attatched_ = false;
}

/**
 * \brief Called when the Phidget is attatched
 * \param handle phidget handle
 * \param user_ptr optional parameter for reinterpret casting
 */
int PhidgetSpatial::AttatchHandler(CPhidgetHandle handle, void* user_ptr)
{
    return 0;
}

/**
* \brief Called when the Phidget is detatched
* \param handle phidget handle
* \param user_ptr optional parameter for reinterpret casting
*/
int PhidgetSpatial::DetachHandler(CPhidgetHandle handle, void* user_ptr)
{
    static_cast<PhidgetSpatial*>(user_ptr)->attatched_ = false;
    return 0;
}

/**
 * \brief Called when the Phidget encounters an error
 * \param handle phidget handle
 * \param user_ptr optional parameter for reinterpret casting
 * \param error error code
 * \param unknown
 */
int PhidgetSpatial::ErrorHandler(CPhidgetHandle handle, void* user_ptr, int error, const char* unknown)
{
    static_cast<PhidgetSpatial*>(user_ptr)->error_ = error;
    return error;
}

/**
 * \brief Called when the Phidget encoutners an error
 * \param handle phidget handle
 * \param user_ptr optional parameter for reinterpret casting
 * \param data array of spatial event data
 * \param packets number of packets within the data
 */
int PhidgetSpatial::DataHandler(CPhidgetSpatialHandle handle, void* user_ptr, CPhidgetSpatial_SpatialEventDataHandle* data, int packets)
{
    auto phidget_spatial = static_cast<PhidgetSpatial*>(user_ptr);
    for (int i = 0; i < packets; ++i)
    {
        phidget_spatial->acceleration_ = Vector3<double>(data[i]->acceleration[0], data[i]->acceleration[1], data[i]->acceleration[2]);
        phidget_spatial->angular_rate_ = Vector3<double>(data[i]->angularRate[0], data[i]->angularRate[1], data[i]->angularRate[2]);
        phidget_spatial->magnetic_field_ = Vector3<double>(data[i]->magneticField[0], data[i]->magneticField[1], data[i]->magneticField[2]);
    }
    return 0;
}
