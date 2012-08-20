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

		AssemblyInfo
			(
			const lemon::String & version,
			const lemon::String & projectPath,
			const lemon::String & scriptFileDirectory,
			const lemon::String & c_cxxGenerateFileDir,
			bool createWin32RcFile
			);

		void WriteBinaryFile(const lemon::String & path);

		const std::string TraceMacroName() { return _traceMacroName; }

		const std::string I18nMacroName() const { return _i18nName; }

		void AddI18nText(const std::string & /*key*/);
	
		void AddTraceEvent(lemon::uint32_t /*seq*/,const std::string & key);

		lemon::uuid_t Uuid() const { return lemon::uuid_t(); }

		const std::string GuidString();

		const std::string VersionString();

		const lemon::String &ScriptFileDirectory() const { return _scriptFileDirectory; }

		const std::string & GetName() { return _name; }

	private:

		void ScanC_CxxFiles();

		void AddErrorInfo(lemon::uint32_t /*code*/, const char * /*name*/,const char * /*descripton*/);

		void AddTraceCatalog(lemon::uint32_t /*value*/,const char * /*name*/,const char * /*descripton*/);

		void TraceName(const char * name) { _traceMacroName = name; }

		void I18nName(const char * name) { _i18nName = name; }

		void Name(const char * name) { _name = name; }

	private:

		lemon_uint32_t				_files;

		resource::package			_package;

		std::string					_traceMacroName;

		std::string					_i18nName;

		std::string					_name;

		lemon::String				_projectPath;

		lemon::String				_scriptFileDirectory;

		lemon::String				_c_cxxGenerateFileDir;

		bool						_createWin32RcFile;
	};

}}}

#endif // TOOLS_LEMON_RC_ASSEMBLYINFO_HPP

