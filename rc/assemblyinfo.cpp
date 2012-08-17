#include <stack>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <lemonxx/function/bind.hpp>
#include <tools/lemon/rc/assembly.h>
#include <tools/lemon/rc/assemblyinfo.hpp>
#include <tools/lemon/rc/lexer.hpp>
#include <tools/lemon/rc/parser.hpp>
#include <tools/lemon/rc/codegen.hpp>

namespace lemon{namespace rc{namespace tools{

	AssemblyInfo::AssemblyInfo
		(
		const lemon::String & /*version*/,
		const lemon::String & projectPath,
		const lemon::String & scriptFileDirectory,
		const lemon::String & c_cxxGenerateFileDir,
		bool createWin32RcFile
		)
		:_projectPath(projectPath)
		,_scriptFileDirectory(scriptFileDirectory)
		,_c_cxxGenerateFileDir(c_cxxGenerateFileDir)
		,_createWin32RcFile(createWin32RcFile)
		,_files(0)
	{
		//_resource.version(version.c_str());

		luabind::lua_state  L;

		luabind::module(L,"lemon-rc")
			<= luabind::class_<AssemblyInfo>("cassembly")
						.def("error_info",&AssemblyInfo::AddErrorInfo)
						.def("trace_catalog",&AssemblyInfo::AddTraceCatalog)
						.def("guid",&AssemblyInfo::GuidString)
						.def("version",&AssemblyInfo::VersionString)
						.def("name",&AssemblyInfo::Name)
						.def("trace_macro_name",&AssemblyInfo::TraceName)
						.def("i18n_macro_name",&AssemblyInfo::I18nName)
			;

		StringStream stream;

		stream << LEMON_TEXT("package.path = package.path .. \";") 
			
			<< scriptFileDirectory << LEMON_TEXT("/?.lua\"");

		// set the module search path
		luabind::dostring(L,to_locale(stream.str()).c_str());
		// load assemblyinfo metadata file
		luabind::dofile(L,to_locale(projectPath + LEMON_TEXT("/assemblyinfo.lua")).c_str());
		// load assembly compiler onload script
		luabind::dofile(L,to_locale(scriptFileDirectory + LEMON_TEXT("/onload.lua")).c_str());
		// call onload function
		luabind::call<void>(L,"onload",this,to_locale(_c_cxxGenerateFileDir));

		ScanC_CxxFiles();
	}

	const std::string AssemblyInfo::GuidString()
	{
		lemon::uuid_t id;//_resource.guid();

		const lemon_byte_t * bytes = (const lemon_byte_t*)&id;

		char buffer[128] = {0};

		lemon_csprintf(
			buffer,sizeof(buffer),"{0x%02X%02X%02X%02X,0x%02X%02X,0x%02X%02X,{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}}",
			bytes[3],bytes[2],bytes[1],bytes[0],bytes[5],bytes[4],bytes[7],bytes[6],
			bytes[8],bytes[9],bytes[10],bytes[11],bytes[12],bytes[13],bytes[14],bytes[15]
		);

		return buffer;
	}

	const std::string AssemblyInfo::VersionString()
	{
		LemonVersion v;

		const LemonVersion * version = &v;//_resource.version();

		char buffer[56] = {0};

		lemon_csprintf(buffer,sizeof(buffer),"{%d,%d,%d,%d}",version->Major,version->Minor,version->Build,version->Reversion);

		return buffer;
	}

	void AssemblyInfo::WriteBinaryFile(const lemon::String & path)
	{
		error_info errorCode;

		std::ofstream stream(lemon::to_locale(path).c_str(),std::ios::trunc);

		if(!stream.is_open())
		{
			LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_OPENT_BINARY_RESOURCE_FILE_ERROR);

			errorCode.check_throw(path.c_str());
		}

		//_resource.write(stream);
	}

	//void AssemblyInfo::AddI18nText(const std::string & key)
	//{
	//	//_resource.add(resource_text(lemon::from_locale(key),lemon::from_locale(key)));
	//}

	//void AssemblyInfo::AddTraceEvent(lemon::uint32_t seq,const std::string & key)
	//{
	//	//_resource.add(resource_trace_event(seq,lemon::from_locale(key)));
	//}

	bool ExtensionCompare(const lemon::String & lhs,const lemon::char_t * rhs)
	{
		return lhs == rhs;
	}

	void AssemblyInfo::ScanC_CxxFiles()
	{
		static const lemon::char_t* c_cxx_extensions[] = 
		{
			LEMON_TEXT("h"),LEMON_TEXT("hpp"),LEMON_TEXT("hxx"),LEMON_TEXT("hh"),
			LEMON_TEXT("c"),LEMON_TEXT("cpp"),LEMON_TEXT("cc"),LEMON_TEXT("cxx")
		};

		static const lemon::char_t* source_extensions[] = 
		{
			LEMON_TEXT("c"),LEMON_TEXT("cpp"),LEMON_TEXT("cc"),LEMON_TEXT("cxx")
		};

		const lemon::char_t ** c_cxx_extensions_begin = c_cxx_extensions;

		const lemon::char_t ** c_cxx_extensions_end = c_cxx_extensions + sizeof(c_cxx_extensions)/sizeof(char_t*);

		const lemon::char_t ** source_extensions_begin = source_extensions;

		const lemon::char_t ** source_extensions_end = source_extensions + sizeof(source_extensions)/sizeof(char_t*);

		std::stack<lemon::String>	directories;

		directories.push(_projectPath);

		while(!directories.empty())
		{
			lemon::String directory = directories.top();

			fs::directory_iteartor_t iter(directory),end;

			directories.pop();

			for(;iter != end; ++ iter)
			{
				fs::path path(directory + LEMON_TEXT("/") + *iter);

				path.compress();

				if(*iter == LEMON_TEXT(".") || *iter == LEMON_TEXT(".."))
				{
					continue;
				}

				if(fs::is_directory(path.string()))  
				{
					directories.push(path.string()); 
					
					continue; 
				}

				lemon::String extension = fs::extension(path);


				if(c_cxx_extensions_end != std::find_if(
					c_cxx_extensions_begin,
					c_cxx_extensions_end,
					lemon::bind(&ExtensionCompare,lemon::cref(extension),lemon::_0)))
				{

					AST ast;

					Lexer lexer(path.string().c_str());

					Parser parser(this);

					parser.Parse(lexer,ast);

					if(source_extensions_end != std::find_if(
						source_extensions_begin,
						source_extensions_end,
						lemon::bind(&ExtensionCompare,lemon::cref(extension),lemon::_0)))
					{
						if(ast.Size() == 0)  continue;

						AST::NodeListType::const_iterator iter,end = ast.NodeList.end();

						for(iter = ast.NodeList.begin(); iter != end; ++ iter)
						{
							//AddTraceEvent(LEMON_MAKE_TRACE_EVENT_SEQUENCE(_files,iter->lines),iter->formatter);
						}

						CXXCodeGen codegen(this,_files);

						fs::path target = fs::path(_c_cxxGenerateFileDir) / fs::relative(fs::path(_projectPath),path);

						codegen.Generate(target.string() + LEMON_TEXT(".g.hpp") ,ast);

						++ _files;
					}
				}
				
			}
		}
	}
}}}