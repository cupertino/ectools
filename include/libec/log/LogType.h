///////////////////////////////////////////////////////////////////////////////
/// @file		LogType.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Contains all log type supported by this library
///////////////////////////////////////////////////////////////////////////////


#ifndef LIBEC_LOGTYPE_H__
#define LIBEC_LOGTYPE_H__

#include <string>

namespace cea
{

	/// @ingroup log
	/// @brief Type of the logger supported
	///
	/// Each time a new type of log is create for the library the author
	/// need to add the type as an enum item in this enumeration
	enum LogType
	{
		UNKNOWN_LOGTYPE,
		/* NOTE: Add Logger Type here */
		FILE,

	};


	/* Prototype of BaseLog class */
	class BaseLog;


	/// @brief Create a logger from a string
	class LogCreator
	{
	public:

		/// @brief Create a logger from a string
		/// @param type Type of logger to create
		///	@return Pointer to the created log. 
		///			Or 0 if log type not recognized.
		static BaseLog* create(const std::string& type);

	};

}

#endif

///////////////////////////////////////////////////////////////////////////////
/// @ingroup log
/// @class LogCreator
///
/// This class is mainly used by the XML loader system.
/// Each new log type need to have an entry in the create function
///
/// <h3>Example</h3>
/// <!--=======-->
/// Add log type HTTPLog (logger who send data in 
///  POST HTTP request to a server each flush call):
///
/// The function cea::LogCreator::create will be:
///	@code
///		BaseLog* LogCreator::create(const std::string& type)
///		{
///			if (Tools::identicalString(type, "FILE")) {
///				return new FileLog();
///
///
///			// ----------------------------------------------------
///			// LOGGER HTPP
///			// ----------------------------------------------------
///			} else if (Tools::identicalString(name,"http")) {
///				return new HTTPLog();
///			// ----------------------------------------------------
///
///			/** NOTE: Add Log Type here */
///			
///			}
///			return 0;
///		}
/// @endcode
///
/// @see cea::GridViewLoader::loadXML, cea::MonitorLoader::loadXML
///////////////////////////////////////////////////////////////////////////////
