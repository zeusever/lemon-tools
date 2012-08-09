/**
* 
* @file     parser
* @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
* @author   yayanyang
* @version  1.0.0.0  
* @date     2012/06/28
*/
#ifndef TOOLS_LEMON_RC_PARSER_HPP
#define TOOLS_LEMON_RC_PARSER_HPP

#include <vector>
#include <tools/lemon/rc/lexer.hpp>

namespace lemon{namespace rc{namespace tools{

	class AssemblyInfo;

	struct AST
	{
		struct Metadata{std::string formatter; size_t lines; size_t args; };

		typedef std::vector<Metadata>	NodeListType;

		NodeListType	NodeList;

		void Add(const std::string & formatter,size_t lines, size_t args)
		{
			Metadata d = {formatter,lines,args};

			NodeList.push_back(d);
		}

		size_t Size() const { return NodeList.size(); }
	};

	class Parser : private lemon::nocopyable
	{
	public:

		Parser(AssemblyInfo * info);
		
		void Parse(Lexer & lexer,AST & ast);

	private:

		void ParseTraceMacroArgs();

		void ParseI18nMacroArgs();

	private:

		AssemblyInfo	*_info;

		Lexer			*_lexer;

		AST				*_ast;
	};

}}}

#endif //TOOLS_LEMON_RC_PARSER_HPP
