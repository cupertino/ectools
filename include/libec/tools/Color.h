///////////////////////////////////////////////////////////////////////////////
/// @file		Color.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Constants of classic RGB color 
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_TOOLS_COLOR_H__
#define LIBEC_TOOLS_COLOR_H__

namespace cea
{

  /// @brief Constants of classic RGB color
  class Color
  {
  public:

    static const int Default = -1; ///< Default color of device
    static const int Black = 0; ///< \#000000 rgb(0, 0, 0);
    static const int DarkBlue = 8388608; ///< \#000080 rgb(0, 0, 128);
    static const int DarkGreen = 32768; ///< \#008000 rgb(0, 128, 0);
    static const int DarkCyan = 8421376; ///< \#008080 rgb(0, 128, 128);
    static const int DarkRed = 128; ///< \#800000 rgb(128, 0, 0);
    static const int DarkMagenta = 8388736; ///< \#800080 rgb(128, 0, 128);
    static const int DarkYellow = 32896; ///< \#808000 rgb(128, 128, 0);
    static const int DarkGray = 8421504; ///< \#800000 rgb(128, 128, 128);
    static const int Gray = 13158600; ///< \#C8C8C8 rgb(200, 200, 200);
    static const int Blue = 16711680; ///< \#0000FF rgb(0, 0, 255);
    static const int Green = 65280; ///< \#00FF00 rgb(0, 255, 0);
    static const int Cyan = 16776960; ///< \#00FFFF rgb(0, 255, 255);
    static const int Red = 255; ///< \#FF0000 rgb(255, 0, 0);
    static const int Magenta = 16711935; ///< \#FF00FF rgb(255, 0, 255);
    static const int Yellow = 65535; ///< \#FFFF00 rgb(255, 255, 0);
    static const int White = 16777215; ///< \#FFFFFF rgb(255, 255, 255);

    /// @brief Transform RGB color into signed int
    /// @param r Intensity of the red color (0 to 255)
    /// @param g Intensity of the green color (0 to 255)
    /// @param b Intensity of the blue color (0 to 255)
    /// @return resultant RGB color as an signed int
    static int
    rgb(unsigned char r, unsigned char g, unsigned char b);

  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::Color
///	@ingroup tools
///
///////////////////////////////////////////////////////////////////////////////
