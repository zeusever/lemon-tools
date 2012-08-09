#include <fstream>
#include <tools/lemon/rc/errorcode.h>
#include <tools/lemon/rc/assemblyinfo.hpp>
#include <tools/lemon/rc/lexer.hpp>
#include <tools/lemon/rc/parser.hpp>
#include <tools/lemon/rc/codegen.hpp>

namespace lemon{namespace rc{namespace tools{

	AssemblyInfo::AssemblyInfo(
		const lemon::String & name,
		const lemon::String & version,
		const lemon::String & infofile,
		const lemon::String & scriptFile,
		const lemon::String & generateFileDirectory,
		bool createMSRC) : _files(0)
	{
		luabind::lua_state L;

		luabind::module(L,"lemonrc")
			<= luabind::class_<AssemblyInfo>("assemblyinfo")
				.def("errorinfo",&AssemblyInfo::AddErrorInfo)
				.def("tracecatalog",&AssemblyInfo::AddTraceCatalog)
				.def("tracename",&AssemblyInfo::TraceName)
				.def("i18nname",&AssemblyInfo::I18nName)
				.def("guid",&AssemblyInfo::GuidString)
			;

		luabind::dofile(L,lemon::to_locale(infofile).c_str());

		luabind::dofile(L,lemon::to_locale(scriptFile).c_str());

		const LemonVersion * v = _resource.version(version.c_str());

		char versionstring[128] = {0};

		lemon_sprintf(versionstring,sizeof(versionstring),"%d.%d.%d.%d",v->Major,v->Minor,v->Build,v->Reversion);

		luabind::call<void>(
			L,"generate",this,
			lemon::to_utf8(generateFileDirectory),
			lemon::to_utf8(name),
			(const char*)versionstring,
			createMSRC);
	}

	void AssemblyInfo::AddErrorInfo(lemon::uint32_t code, const char * name,const char * descripton)
	{
		_resource.add(resource_errorinfo(code,lemon::from_utf8(name),lemon::from_utf8(descripton)));
	}

	void AssemblyInfo::AddTraceCatalog(lemon::uint32_t code,const char * name,const char * descripton)
	{
		_resource.add(resource_trace_catalog(code,lemon::from_utf8(name),lemon::from_utf8(descripton)));
	}

	const std::string AssemblyInfo::GuidString()
	{
		lemon::uuid_t id = _resource.guid();

		const lemon_byte_t * bytes = (const lemon_byte_t*)&id;

		char buffer[128] = {0};

		lemon_sprintf(
			buffer,sizeof(buffer),"{0x%02X%02X%02X%02X,0x%02X%02X,0x%02X%02X,{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}}",
			bytes[3],bytes[2],bytes[1],bytes[0],bytes[5],bytes[4],bytes[7],bytes[6],
			bytes[8],bytes[9],bytes[10],bytes[11],bytes[12],bytes[13],bytes[14],bytes[15]
		);

		return buffer;
	}

	void AssemblyInfo::Write(const lemon::String & path)
	{
		error_info errorCode;

		std::ofstream stream(lemon::to_locale(path),std::ios::trunc);

		if(!stream.is_open())
		{
			LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_OPENT_BINARY_RESOURCE_FILE_ERROR);

			errorCode.check_throw(path.c_str());
		}

		_resource.write(stream);
	}

	void AssemblyInfo::GenerateFile(const lemon::String & source,const lemon::String target)
	{
		Lexer lexer(source.c_str());

		Parser parser(this);

		AST ast;

		parser.Parse(lexer,ast);

		AST::NodeListType::const_iterator iter,end = ast.NodeList.end();

		for(iter = ast.NodeList.begin(); iter != end; ++ iter)
		{
			_resource.add(resource_trace_event(LEMON_MAKE_TRACE_EVENT_SEQUENCE(_files,iter->lines),lemon::from_locale(iter->formatter)));
		}

		CXXCodeGen gen(this,_files);

		if(!target.empty())
		{
			std::ofstream stream(lemon::to_locale(target),std::ios::trunc);

			if(!stream.is_open())
			{
				error_info ec;

				LEMON_USER_ERROR(ec,TOOLS_LEMON_RC_FILE_WRITE_ERROR);

				ec.check_throw();
			}

			gen.Generate(stream,ast);
		}

		++ _files;
	}

	void AssemblyInfo::AddI18nText(const std::string & key)
	{
		_resource.add(resource_text(lemon::from_locale(key),lemon::from_locale(key)));
	}
}}}