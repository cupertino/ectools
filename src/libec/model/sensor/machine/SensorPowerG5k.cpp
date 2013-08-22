//============================================================================
// Name        : SensorPowerPDU.cpp
// Author      : Leandro Fontoura Cupertino
// Version     : 0
// Date        : 2012.06.12
// Copyright   : Your copyright notice
// Description : Power Sensor available on the G5K plataform
//============================================================================

#include <map>
#include <libec/sensor/SensorPowerG5k.h>
#include <libec/tools/Tools.h>
#include <libec/tools/DebugLog.h>

namespace cea
{
  IpOut::IpOut(std::string ip, unsigned int outlet)
  {
    Ip = ip;
    Outlet = outlet;
  }

  // Static variables
  const char* G5kPowerMeter::ClassName = "G5kPowerMeter";
  bool G5kPowerMeter::_isMapBuilt = false;
  std::map<std::string, IpOut> G5kPowerMeter::_hostPduMap;

  // Public methods
  G5kPowerMeter::G5kPowerMeter()
  {
    char hostname[30];
    gethostname(hostname, 30);
    _name = hostname;
    _name = "G5KPDU_" + _name;
    _alias = "G5KPDU";

    _cmd = getCmdLine(hostname);
    _type = Float;
    _cValue.Float = _pValue.Float = 0.0;

    if (!_isMapBuilt)
      buildHostPduMap();

    _isActive = checkActivity();
  }

  G5kPowerMeter::G5kPowerMeter(const std::string &xmlTag) :
      PowerMeter(xmlTag)
  {
    char hostname[30];
    gethostname(hostname, 30);
    _cmd = getCmdLine(hostname);
    _type = Float;
    _cValue.Float = _pValue.Float = 0.0;

    if (!_isMapBuilt)
      buildHostPduMap();

    _isActive = checkActivity();
  }

  G5kPowerMeter::~G5kPowerMeter()
  {
  }

  bool
  G5kPowerMeter::checkActivity()
  {
    if (_cmd == "")
      return false;

    update();
    if (_cValue.Float == -1.0)
      return false;

    return true;
  }

  void
  G5kPowerMeter::update()
  {
    _pValue = _cValue;
    _pTime = _cTime;
    _cTime = time(NULL);

    std::string out = Tools::exec(_cmd.c_str());
    if (out == "")
      _cValue.Float = -1.0;
    else
      {
        std::stringstream ss;
        ss.str(out);
        ss >> _cValue.Float;
      }
  }

  const char*
  G5kPowerMeter::getClassName()
  {
    return ClassName;
  }

  /* Changing connection method..
   bool buildConnectionString(std::string hostname)
   {
   // G5K Hostnames grid-id.site.grid5000.fr
   std:string g5k = "grid5000.fr";
   if (hostname.length() >= g5k.length()) {
   return (0 == hostname.compare (hostname.length() - g5k.length(), g5k.length(), g5k));
   } else {
   return false;
   }
   //check if is a G5K host // if string ends with grid5000.fr

   //get site name

   //get nodeID
   }
   */

  // Private methods
  void
  G5kPowerMeter::buildHostPduMap()
  {
    // Populates the table that maps all the host names of G5K nodes
    // into their PDU's IP and outlet number
    _hostPduMap["stremi-1.reims.grid5000.fr"] = IpOut("172.17.160.121", 1);
    _hostPduMap["stremi-2.reims.grid5000.fr"] = IpOut("172.17.160.121", 2);
    _hostPduMap["stremi-3.reims.grid5000.fr"] = IpOut("172.17.160.121", 3);
    _hostPduMap["stremi-4.reims.grid5000.fr"] = IpOut("172.17.160.121", 4);
    _hostPduMap["stremi-5.reims.grid5000.fr"] = IpOut("172.17.160.121", 5);
    _hostPduMap["stremi-6.reims.grid5000.fr"] = IpOut("172.17.160.121", 6);
    _hostPduMap["stremi-7.reims.grid5000.fr"] = IpOut("172.17.160.121", 7);
    _hostPduMap["stremi-8.reims.grid5000.fr"] = IpOut("172.17.160.121", 8);
    _hostPduMap["stremi-9.reims.grid5000.fr"] = IpOut("172.17.160.121", 9);
    _hostPduMap["stremi-10.reims.grid5000.fr"] = IpOut("172.17.160.121", 10);
    _hostPduMap["stremi-11.reims.grid5000.fr"] = IpOut("172.17.160.121", 11);
    _hostPduMap["stremi-12.reims.grid5000.fr"] = IpOut("172.17.160.121", 12);
    _hostPduMap["stremi-13.reims.grid5000.fr"] = IpOut("172.17.160.121", 13);
    _hostPduMap["stremi-14.reims.grid5000.fr"] = IpOut("172.17.160.121", 14);
    _hostPduMap["stremi-15.reims.grid5000.fr"] = IpOut("172.17.160.121", 15);
    _hostPduMap["stremi-16.reims.grid5000.fr"] = IpOut("172.17.160.121", 16);
    _hostPduMap["stremi-17.reims.grid5000.fr"] = IpOut("172.17.160.121", 17);
    _hostPduMap["stremi-18.reims.grid5000.fr"] = IpOut("172.17.160.120", 1);
    _hostPduMap["stremi-19.reims.grid5000.fr"] = IpOut("172.17.160.120", 2);
    _hostPduMap["stremi-20.reims.grid5000.fr"] = IpOut("172.17.160.120", 3);
    _hostPduMap["stremi-21.reims.grid5000.fr"] = IpOut("172.17.160.120", 4);
    _hostPduMap["stremi-22.reims.grid5000.fr"] = IpOut("172.17.160.120", 5);
    _hostPduMap["stremi-23.reims.grid5000.fr"] = IpOut("172.17.160.120", 6);
    _hostPduMap["stremi-24.reims.grid5000.fr"] = IpOut("172.17.160.120", 7);
    _hostPduMap["stremi-25.reims.grid5000.fr"] = IpOut("172.17.160.120", 8);
    _hostPduMap["stremi-26.reims.grid5000.fr"] = IpOut("172.17.160.120", 9);
    _hostPduMap["stremi-27.reims.grid5000.fr"] = IpOut("172.17.160.120", 10);
    _hostPduMap["stremi-28.reims.grid5000.fr"] = IpOut("172.17.160.120", 11);
    _hostPduMap["stremi-29.reims.grid5000.fr"] = IpOut("172.17.160.120", 12);
    _hostPduMap["stremi-30.reims.grid5000.fr"] = IpOut("172.17.160.120", 13);
    _hostPduMap["stremi-31.reims.grid5000.fr"] = IpOut("172.17.160.120", 14);
    _hostPduMap["stremi-32.reims.grid5000.fr"] = IpOut("172.17.160.120", 15);
    _hostPduMap["stremi-33.reims.grid5000.fr"] = IpOut("172.17.160.120", 16);
    _hostPduMap["stremi-34.reims.grid5000.fr"] = IpOut("172.17.160.120", 17);
    _hostPduMap["stremi-35.reims.grid5000.fr"] = IpOut("172.17.160.122", 1);
    _hostPduMap["stremi-36.reims.grid5000.fr"] = IpOut("172.17.160.122", 2);
    _hostPduMap["stremi-37.reims.grid5000.fr"] = IpOut("172.17.160.122", 3);
    _hostPduMap["stremi-38.reims.grid5000.fr"] = IpOut("172.17.160.122", 4);
    _hostPduMap["stremi-39.reims.grid5000.fr"] = IpOut("172.17.160.122", 5);
    _hostPduMap["stremi-40.reims.grid5000.fr"] = IpOut("172.17.160.122", 6);
    _hostPduMap["stremi-41.reims.grid5000.fr"] = IpOut("172.17.160.122", 7);
    _hostPduMap["stremi-42.reims.grid5000.fr"] = IpOut("172.17.160.122", 8);
    _hostPduMap["stremi-43.reims.grid5000.fr"] = IpOut("172.17.160.122", 9);
    _hostPduMap["stremi-44.reims.grid5000.fr"] = IpOut("172.17.160.122", 10);
    /* Changing connection method...
     _hostPduMap["stremi-1.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.1";
     _hostPduMap["stremi-2.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.2";
     _hostPduMap["stremi-3.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.3";
     _hostPduMap["stremi-4.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.4";
     _hostPduMap["stremi-5.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.5";
     _hostPduMap["stremi-6.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.6";
     _hostPduMap["stremi-7.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.7";
     _hostPduMap["stremi-8.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.8";
     _hostPduMap["stremi-9.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.9";
     _hostPduMap["stremi-10.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.10";
     _hostPduMap["stremi-11.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.11";
     _hostPduMap["stremi-12.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.12";
     _hostPduMap["stremi-13.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.13";
     _hostPduMap["stremi-14.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.14";
     _hostPduMap["stremi-15.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.15";
     _hostPduMap["stremi-16.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.16";
     _hostPduMap["stremi-17.reims.grid5000.fr"] =
     "172.17.160.121 outletActivePower.17";
     _hostPduMap["stremi-18.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.1";
     _hostPduMap["stremi-19.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.2";
     _hostPduMap["stremi-20.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.3";
     _hostPduMap["stremi-21.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.4";
     _hostPduMap["stremi-22.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.5";
     _hostPduMap["stremi-23.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.6";
     _hostPduMap["stremi-24.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.7";
     _hostPduMap["stremi-25.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.8";
     _hostPduMap["stremi-26.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.9";
     _hostPduMap["stremi-27.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.10";
     _hostPduMap["stremi-28.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.11";
     _hostPduMap["stremi-29.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.12";
     _hostPduMap["stremi-30.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.13";
     _hostPduMap["stremi-31.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.14";
     _hostPduMap["stremi-32.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.15";
     _hostPduMap["stremi-33.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.16";
     _hostPduMap["stremi-34.reims.grid5000.fr"] =
     "172.17.160.120 outletActivePower.17";
     _hostPduMap["stremi-35.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.1";
     _hostPduMap["stremi-36.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.2";
     _hostPduMap["stremi-37.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.3";
     _hostPduMap["stremi-38.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.4";
     _hostPduMap["stremi-39.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.5";
     _hostPduMap["stremi-40.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.6";
     _hostPduMap["stremi-41.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.7";
     _hostPduMap["stremi-42.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.8";
     _hostPduMap["stremi-43.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.9";
     _hostPduMap["stremi-44.reims.grid5000.fr"] =
     "172.17.160.122 outletActivePower.10";
     _hostPduMap["graphene-105.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.1";
     _hostPduMap["graphene-106.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.2";
     _hostPduMap["graphene-107.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.3";
     _hostPduMap["graphene-108.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.4";
     _hostPduMap["graphene-109.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.5";
     _hostPduMap["graphene-110.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.6";
     _hostPduMap["graphene-111.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.7";
     _hostPduMap["graphene-112.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.8";
     _hostPduMap["graphene-113.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.11";
     _hostPduMap["graphene-114.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.0.12";
     //TODO: debug Nancy nodes outlet Id could also be 0.13.
     _hostPduMap["graphene-115.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.1";
     _hostPduMap["graphene-116.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.2";
     _hostPduMap["graphene-117.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.5";
     _hostPduMap["graphene-118.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.6";
     _hostPduMap["graphene-119.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.7";
     _hostPduMap["graphene-120.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.8";
     _hostPduMap["graphene-121.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.9";
     _hostPduMap["graphene-122.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.10";
     _hostPduMap["graphene-123.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.11";
     _hostPduMap["graphene-124.nancy.grid5000.fr"] =
     "graphene-pdu7.nancy.grid5000.fr outletWatts.1.12";
     _hostPduMap["graphene-125.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.1";
     _hostPduMap["graphene-126.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.2";
     _hostPduMap["graphene-127.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.3";
     _hostPduMap["graphene-128.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.4";
     _hostPduMap["graphene-129.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.5";
     _hostPduMap["graphene-130.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.6";
     _hostPduMap["graphene-131.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.7";
     _hostPduMap["graphene-132.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.8";
     _hostPduMap["graphene-133.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.11";
     _hostPduMap["graphene-134.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.0.12";
     _hostPduMap["graphene-135.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.1";
     _hostPduMap["graphene-136.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.2";
     _hostPduMap["graphene-137.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.5";
     _hostPduMap["graphene-138.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.6";
     _hostPduMap["graphene-139.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.7";
     _hostPduMap["graphene-140.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.8";
     _hostPduMap["graphene-141.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.9";
     _hostPduMap["graphene-142.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.10";
     _hostPduMap["graphene-143.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.11";
     _hostPduMap["graphene-144.nancy.grid5000.fr"] =
     "graphene-pdu8.nancy.grid5000.fr outletWatts.1.12";
     // Warning: Rennes nodes has more than one node plugged in the same PDU,
     // which gives coarse grained power monitoring
     _hostPduMap["parapluie-1.rennes.grid5000.fr"] =
     "parapluie-pdu-1.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-2.rennes.grid5000.fr"] =
     "parapluie-pdu-1.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-3.rennes.grid5000.fr"] =
     "parapluie-pdu-1.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-4.rennes.grid5000.fr"] =
     "parapluie-pdu-1.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-5.rennes.grid5000.fr"] =
     "parapluie-pdu-1.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-6.rennes.grid5000.fr"] =
     "parapluie-pdu-1.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-7.rennes.grid5000.fr"] =
     "parapluie-pdu-1.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-8.rennes.grid5000.fr"] =
     "parapluie-pdu-3.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-9.rennes.grid5000.fr"] =
     "parapluie-pdu-3.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-10.rennes.grid5000.fr"] =
     "parapluie-pdu-3.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-11.rennes.grid5000.fr"] =
     "parapluie-pdu-3.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-12.rennes.grid5000.fr"] =
     "parapluie-pdu-3.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-13.rennes.grid5000.fr"] =
     "parapluie-pdu-3.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-14.rennes.grid5000.fr"] =
     "parapluie-pdu-3.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-15.rennes.grid5000.fr"] =
     "parapluie-pdu-2.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-16.rennes.grid5000.fr"] =
     "parapluie-pdu-2.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-17.rennes.grid5000.fr"] =
     "parapluie-pdu-2.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-18.rennes.grid5000.fr"] =
     "parapluie-pdu-2.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-19.rennes.grid5000.fr"] =
     "parapluie-pdu-2.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-20.rennes.grid5000.fr"] =
     "parapluie-pdu-2.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-21.rennes.grid5000.fr"] =
     "parapluie-pdu-6.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-22.rennes.grid5000.fr"] =
     "parapluie-pdu-6.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-23.rennes.grid5000.fr"] =
     "parapluie-pdu-6.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-24.rennes.grid5000.fr"] =
     "parapluie-pdu-6.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-25.rennes.grid5000.fr"] =
     "parapluie-pdu-6.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-26.rennes.grid5000.fr"] =
     "parapluie-pdu-6.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-27.rennes.grid5000.fr"] =
     "parapluie-pdu-5.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-28.rennes.grid5000.fr"] =
     "parapluie-pdu-5.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-29.rennes.grid5000.fr"] =
     "parapluie-pdu-5.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-30.rennes.grid5000.fr"] =
     "parapluie-pdu-5.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-31.rennes.grid5000.fr"] =
     "parapluie-pdu-5.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-32.rennes.grid5000.fr"] =
     "parapluie-pdu-5.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-33.rennes.grid5000.fr"] =
     "parapluie-pdu-5.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-34.rennes.grid5000.fr"] =
     "parapluie-pdu-4.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-35.rennes.grid5000.fr"] =
     "parapluie-pdu-4.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-36.rennes.grid5000.fr"] =
     "parapluie-pdu-4.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-37.rennes.grid5000.fr"] =
     "parapluie-pdu-4.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-38.rennes.grid5000.fr"] =
     "parapluie-pdu-4.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-39.rennes.grid5000.fr"] =
     "parapluie-pdu-4.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     _hostPduMap["parapluie-40.rennes.grid5000.fr"] =
     "parapluie-pdu-4.rennes.grid5000.fr rPDUIdentDevicePowerWatts.0";
     */
    _isMapBuilt = true;
  }

  std::string
  G5kPowerMeter::getCmdLine(std::string hostname)
  {
    std::map<std::string, IpOut>::iterator it;
    std::stringstream out;

    it = _hostPduMap.find(hostname);
    if (it == _hostPduMap.end())
      {
        // Hostname not mapped
        cea::DebugLog::writeMsg(cea::DebugLog::ERROR, "G5KPowerMeter",
            "The specified hostname has no wattmeter or is not supported by this class.");
        return "";
      }

    /* Changing connection method ...
     out << "snmpget -v 1 -c public " << it->second.Ip << " outletActivePower."
     << it->second.Outlet << " | cut -d\" \" -f 4";
     */
    out << "snmpget -v 1 -c public " << it->second.Ip << " outletActivePower."
        << it->second.Outlet << " | cut -d\" \" -f 4";

    std::string res = out.str();

    return res;
  }
}
