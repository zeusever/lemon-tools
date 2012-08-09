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
			const lemon::String & generateFileDirectory,
			bool createMSRC);

		void Write(const lemon::String & path);

		void GenerateFile(const lemon::String & source,const lemon::String target);

		const std::string TraceMacroName() const { return _traceMacroName; }

		const std::string I18nMacroName() const { return _i18nName; }

		void AddI18nText(const std::string & key);

		lemon::uuid_t Uuid() const { return _resource.guid(); }

		const resource &  Resource() const {return _resource; }

	private:

		void AddErrorInfo(lemon::uint32_t code, const char * name,const char * descripton);

		void AddTraceCatalog(lemon::uint32_t value,const char * name,const char * descripton);

		void TraceName(const char * name) { _traceMacroName = name; }

		void I18nName(const char * name) { _i18nName = name; }

		const std::string GuidString();

	private:

		lemon_uint32_t				_files;

		resource					_resource;

		std::string					_traceMacroName;

		std::string					_i18nName;
	};

}}}

#endif // TOOLS_LEMON_RC_ASSEMBLYINFO_HPP

