///////////////////////////////////////////////////////////////////////////////
/// @file		Globals.h
/// @author		Leandro Fontoura Cupertino, Dorian RODDE
/// @version	0.1
/// @date		2012.06
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Declaration of globally visible classes, enumeration types,
///             type definitions, functions, constants and variables
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_GLOBALS_H__
#define LIBEC_GLOBALS_H__

//#define DEBUG 0
#define DEBUG 1

#include <sstream>

namespace cea
{
  /* Windows support */
#ifdef _WIN32
  /// @brief Type needed for Windows support
#define pid_t DWORD
#endif

  /* Type and structure */
  /// @brief Unsigned int in 64 bits
  ///
  /// We define u64 as unsigned long long for every architecture
  /// so that we can print it with %Lx without getting warnings.
  ///
  typedef unsigned long long u64;

  /// @brief Short name for unsigned int
  typedef unsigned int u32;

  /// @brief Short name for unsigned int
  typedef unsigned int uint;

  /// @brief 3D Vector
  struct uint3
  {
    /* Constructor */
    /// @brief Create a 3D vector
    /// @param x X-coordinate
    /// @param y Y-coordinate
    /// @param z Z-coordinate
    uint3(unsigned int x = 0, unsigned int y = 0, unsigned int z = 0)
    {
      this->x = x;
      this->y = y;
      this->z = z;
    }

    /* Public Members */
    unsigned int x; ///< X coordinate
    unsigned int y; ///< Y coordinate
    unsigned int z; ///< Z coordinate

    /* Functions */
    /// @brief Convert 3D vector into string
    std::string
    ToString()
    {
      std::stringstream str;
      str << x << ";" << y << ";" << z;
      return str.str();
    }
  };

  /* Units */

  /// \brief Estimator result unit
  typedef double power_t;

  /// \brief Sensor unit
  union sensor_t
  {
    float Float; ///< float value
    u64 U64; ///< unsigned int value
  };

  /// \brief Type of the Sensor result
  enum SensorType
  {
    U64, ///< unsigned long long
    Float, ///< float
    Unknown, ///< unknown type
  };
}

#endif /* LIBEC_GLOBALS_H_ */

///////////////////////////////////////////////////////////////////////////////
/// @file Globals.h
///	@ingroup tools
///
///////////////////////////////////////////////////////////////////////////////

