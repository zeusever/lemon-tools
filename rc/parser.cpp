#include <sstream>
#include <iostream>
#include <tools/lemon/rc/parser.hpp>
#include <tools/lemon/rc/assembly.h>
#include <tools/lemon/rc/assemblyinfo.hpp>
namespace lemon{namespace rc{namespace tools{

	Parser::Parser(AssemblyInfo * info)
		:_info(info)
	{

	}

	void Parser::Parse(Lexer & lexer,AST & ast)
	{
		_lexer = &lexer;

		_ast = &ast;

		Token token;

		while(_lexer->Next(token))
		{
			if(token.Type == Lexer::TOKEN_ID && token.Value == _info->TraceMacroName())
			{
				ParseTraceMacroArgs();
			}
			else if(token.Type == Lexer::TOKEN_ID && token.Value == _info->I18nMacroName())
			{
				ParseI18nMacroArgs();
			}
		}
	}

	void Parser::ParseI18nMacroArgs()
	{
		Token token;

		error_info errorCode;

		if(!_lexer->Next(token) || token.Type != '(')
		{
			size_t lines = _lexer->Line();

			size_t column = _lexer->Column();

			LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_C_CXX_MACRO_ERROR);

			lemon::StringStream stream;

			stream << LEMON_TEXT("(") << lines << LEMON_TEXT(',') << column << LEMON_TEXT(") expect \"(\"");

			errorCode.check_throw(stream.str().c_str());
		}

		if(!_lexer->Next(token) || token.Type != Lexer::TOKEN_LIT_TEXT)
		{
			size_t lines = _lexer->Line();

			size_t column = _lexer->Column();

			LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_C_CXX_MACRO_ERROR);

			lemon::StringStream stream;

			stream << LEMON_TEXT("(") << lines << LEMON_TEXT(',') << column << LEMON_TEXT(") expect literal string");

			errorCode.check_throw(stream.str().c_str());
		}

		_info->AddI18nText(token.Value);

		if(!_lexer->Next(token) || token.Type != ')')
		{
			size_t lines = _lexer->Line();

			size_t column = _lexer->Column();

			LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_C_CXX_MACRO_ERROR);

			lemon::StringStream stream;

			stream << LEMON_TEXT("(") << lines << LEMON_TEXT(',') << column << LEMON_TEXT(") expect \")\"");

			errorCode.check_throw(stream.str().c_str());
		}
	}

	void Parser::ParseTraceMacroArgs()
	{
		Token token;

		size_t lines = _lexer->Line();

		bool expectNext = true;

		size_t args = 0;

		std::string formatter;

		_lexer->Next(token);

		if(token.Type != '(') 
		{
			error_info errorCode;

			LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_C_CXX_MACRO_ERROR);

			errorCode.check_throw();
		}

		while(_lexer->Next(token))
		{
			if(token.Type == '(')
			{
				expectNext = false;
			}
			else if(token.Type == ',' && expectNext)
			{
				++ args;
			}
			else if(token.Type == ')')
			{
				if(expectNext)
				{
					++ args;

					if(args < 4)
					{
						error_info errorCode;

						LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_EXPECT_FORMATTER_STRING);

						errorCode.check_throw();
					}

					_ast->Add(formatter,lines,args);

					return;
				}

				expectNext = true;
			}
			else if(args == 3)
			{
				if(token.Type != Lexer::TOKEN_LIT_TEXT || !expectNext)
				{
					error_info errorCode;

					LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_C_CXX_MACRO_ERROR);

					errorCode.check_throw();
				}

				formatter = token.Value;
				
				if(!_lexer->Next(token) || (token.Type != ',' && token.Type != ')'))
				{
					error_info errorCode;

					LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_C_CXX_MACRO_ERROR);

					errorCode.check_throw();
				}

				++ args;

				if(token.Type == ')')
				{
					_ast->Add(formatter,lines,args);

					return;
				}
			}

		}

		error_info errorCode;

		LEMON_USER_ERROR(errorCode,TOOLS_LEMON_RC_C_CXX_MACRO_ERROR);

		errorCode.check_throw();
	}

}}}