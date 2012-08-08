#include <tools/lemon/rc/assemblyinfo.hpp>

namespace lemon{namespace rc{namespace tools{

	AssemblyInfo::AssemblyInfo(
		const lemon::String & name,
		const lemon::String & version,
		const lemon::String & infofile,
		const lemon::String & scriptFile,
		const lemon::String & generateFileDirectory)
	{
		luabind::lua_state L;

		luabind::module(L,"lemonrc")
			<= luabind::class_<AssemblyInfo>("assemblyinfo")
				.def("errorinfo",&AssemblyInfo::AddErrorInfo)
				.def("tracecatalog",&AssemblyInfo::AddTraceCatalog)
				.def("tracename",&AssemblyInfo::TraceName)
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
			(const char*)versionstring);
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
}}}