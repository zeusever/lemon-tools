/**
* 
* @file     codegen
* @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
* @author   yayanyang
* @version  1.0.0.0  
* @date     2012/06/28
*/
#ifndef LEMON_PPC_CODEDENG_HPP
#define LEMON_PPC_CODEDENG_HPP
#include <ostream>
#include <tools/lemon/rc/parser.hpp>

namespace lemon{namespace rc{namespace tools{

	class AssemblyInfo;

	class CXXCodeGen : private lemon::nocopyable
	{
	public:
		
		
		CXXCodeGen(const AssemblyInfo * info,size_t moduleId);

		void Generate(const lemon::String & filePath,const AST & ast);

	private:

		const AssemblyInfo				*_info;

		size_t							_moduleId;

		
	};

}}}
#endif //LEMON_PPC_CODEDENG_HPP