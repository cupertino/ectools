// A first static model to test the interface
#include <sys/sysinfo.h>
#include <dirent.h>

#include <libec/Globals.h>
#include <libec/sensors.h>
#include <libec/estimator/PELinearRegression.h>

namespace cea
{
  /** Constructor */
  PELinearRegression::PELinearRegression(float min, float max, PIDSensor* sensor)
  {
    _name = "LINEAR_POWER_ESTIMATOR";
    _alias = "PE_L";
    _isActive = true;
    _type = Float;
    _min = min;
    _delta = max - min;
    _sensor = sensor;
  }

  void
  PELinearRegression::updatePid(pid_t pid)
  {
    if (_sensor->getTime() == U64)
      _cvPIDMap[pid].Float = _delta * _sensor->getValuePid(pid).U64 + _min;
    else
      _cValue.Float = _delta * _sensor->getValuePid(pid).Float + _min;

    _cValue.Float = _cValue.Float / getCountProc();
  }

  int
  PELinearRegression::getCountProc()
  {
    struct dirent *dir;
    /* Open proc dir */
    DIR* proc = opendir("/proc");
    int counter = 0;

    /* Get all the numeric dir (corresponding to the pid) */
    while ((dir = readdir(proc)) != NULL)
      {
        if (Tools::isNumeric(dir->d_name))
          {
            counter++;
          }
      }

    /* Close the proc dir */
    closedir(proc);

    return counter;
  }

  sensor_t
  PELinearRegression::getValuePid(pid_t pid)
  {
    updatePid(pid);

    return _cValue;
  }
}
