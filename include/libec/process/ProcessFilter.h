///////////////////////////////////////////////////////////////////////////////
/// @file		ProcessFilter.h
/// @author		Dorian RODDE
/// @version	0.1
/// @date		2012.07
/// @copyright	2012, CoolEmAll (INFSO-ICT-288701)
/// @brief		Abstract class for filter process on a BaseProcessEnumerator
///////////////////////////////////////////////////////////////////////////////

#ifndef LIBEC_PROCESSFILTER_H__
#define LIBEC_PROCESSFILTER_H__

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "BaseProcess.h"
#include "../Globals.h"

namespace cea
{

  /// @brief Abstract class for filter process on a BaseProcessEnumerator
  class ProcessFilter
  {
  public:
    /// @brief Apply filter on a process
    /// @param p Process considered
    /// @return true if filter accept the Process
    virtual bool
    applyFilter(Process* p) = 0;
  };

}

#endif

///////////////////////////////////////////////////////////////////////////////
///	@class cea::ProcessFilter
///	@ingroup process
///
/// This class is used to create filter for ProcessEnumerator.
///	
///	<h3>Example</h3>
///	@code
///		// Example of a filter (which can filter by name):
///		
///		// Declaration
///		class ProcFilter_Name : public ProcessFilter
///		{
///		public:
///			ProcFilter_Name(const std::string& name) : _name(name)
///			{
///			}
///			
///			bool applyFilter(Process* p)
///			{
///				return (p->getName().strcomp(_name)) >= 0);
///			}
///		
///		protected:
///			std::string _name;
///		}
///	
///		// Usage
///		ProcessEnumerator pe;
///		ProcFilter_Name filterName("ectop");
///		pe->addFilter(&filterName);
///	@endcode
///
///////////////////////////////////////////////////////////////////////////////
