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

		void Generate(std::ostream & stream,const AST & ast);

	private:

		void TitleG(std::ostream & stream);

		void GuardHeaderG(std::ostream & stream);

		void GuardTailG(std::ostream & stream);

		void PackageIdGetFunctionG(std::ostream & stream);

		void CatalogG(std::ostream & stream);

		void ClassDeclareG(std::ostream & stream);

		void PartialTemplateSpecialization(std::ostream & stream,const AST::Metadata & data);

		void MacroDefine(std::ostream & stream);

		void SuffixG();

	private:

		const AssemblyInfo				*_info;

		const lemon::uuid_t				_packageId;

		const lemon::uuid_t				_moduleGuid;

		const std::string				_macroname;

		std::string						_packageIdFunctionName;

		std::string						_suffix;

		size_t							_moduleId;

		
	};

}}}
#endif //LEMON_PPC_CODEDENG_HPP