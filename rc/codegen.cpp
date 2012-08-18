#include <sstream>
#include <algorithm>
#include <tools/lemon/rc/codegen.hpp>
#include <tools/lemon/rc/assembly.h>
#include <tools/lemon/rc/assemblyinfo.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/luabind/luabind.hpp>
namespace lemon{namespace rc{namespace tools{

	class ASTVisitor
	{
	public:

		typedef lemon::tuple<size_t,size_t> value_type;
		
		ASTVisitor(const AST & ast):_iter(ast.NodeList.begin()),_end(ast.NodeList.end()) {}

		bool Next()
		{
			_current = _iter;

			bool result =  _iter != _end;

			if(result) ++ _iter;

			return result;
		}

		value_type Value()
		{
			return value_type(_current->lines,_current->args);
		}

	private:

		AST::NodeListType::const_iterator _current;

		AST::NodeListType::const_iterator _iter;
		
		AST::NodeListType::const_iterator _end;
	};

	CXXCodeGen::CXXCodeGen(const AssemblyInfo * info,size_t moduleId)
		:_info(info),_moduleId(moduleId)
	{
		
	}

	void CXXCodeGen::Generate(const lemon::String & filePath , const AST & ast)
	{
		luabind::lua_state L;

		luabind::module(L,"lemon_rc")
			<= luabind::class_<AssemblyInfo>("cassembly")
						.def("guid",&AssemblyInfo::GuidString)
						.def("name",&AssemblyInfo::GetName)
						.def("trace_macro_name",&AssemblyInfo::TraceMacroName)
			<= luabind::class_<ASTVisitor>("trace_iterator")
						.def("next",&ASTVisitor::Next)
						.def("value",&ASTVisitor::Value)
			;

		ASTVisitor visitor(ast);

		luabind::dofile(L,to_locale(_info->ScriptFileDirectory() + LEMON_TEXT("/cxx_dtrace.lua")).c_str());

		luabind::call<void>(L,"generate_sub_file",to_locale(filePath),_info,&visitor,_moduleId);
	}
}}}