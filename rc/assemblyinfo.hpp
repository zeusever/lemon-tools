/**
* 
* @file     assemblyinfo
* @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
* @author   yayanyang
* @version  1.0.0.0  
* @date     2012/08/08
*/
#ifndef TOOLS_LEMON_RC_ASSEMBLYINFO_HPP
#define TOOLS_LEMON_RC_ASSEMBLYINFO_HPP

#include <lemonxx/sys/sys.hpp>
#include <lemonxx/utility/utility.hpp>
#include <lemonxx/luabind/luabind.hpp>

namespace lemon{namespace rc{namespace tools{

	class AssemblyInfo : private lemon::nocopyable
	{
	public:

		AssemblyInfo(
			const lemon::String & name,
			const lemon::String & version,
			const lemon::String & infofile,
			const lemon::String & scriptFile,
			const lemon::String & generateFileDirectory);

	private:

		void AddErrorInfo(lemon::uint32_t code, const char * name,const char * descripton);

		void AddTraceCatalog(lemon::uint32_t value,const char * name,const char * descripton);

		void TraceName(const char * name) { _traceMacroName = name; }

		const std::string GuidString();

	private:

		resource					_resource;

		std::string					_traceMacroName;
	};

}}}

#endif // TOOLS_LEMON_RC_ASSEMBLYINFO_HPP

