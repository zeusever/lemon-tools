/**
* 
* @file     console
* @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
* @author   yayanyang
* @version  1.0.0.0  
* @date     2012/08/08
*/
#include <iostream>
#include <lemonxx/sys/sys.hpp>
#include <lemonxx/utility/utility.hpp>
#include <tools/lemon/rc/lexer.hpp>
#include <tools/lemon/rc/parser.hpp>
#include <tools/lemon/rc/assemblyinfo.hpp>
#include <tools/lemon/rc/assembly.h>

using namespace lemon;
using namespace lemon::rc::tools;

#ifdef WIN32
int wmain(int args, wchar_t * argv[])
#else
int main(int args, char * argv[])
#endif //WIN32
{
	if(args != 7)
	{
		std::cout << "lemon-rc <script file path>  <version>  <path> <c_cxx generate file output directory> <object resource path> <create asembly.rc>"  << std::endl;

		return 1;
	}

	lemon::String scriptFileDirectory = argv[1];

	lemon::String projectVersion = argv[2];

	lemon::String projectPath = argv[3];

	lemon::String generateFileDirectory = argv[4];

	lemon::String resourceFilePath = argv[5];

	lemon::String createMSRC = argv[6];

	LEMON_COUT << TOOLS_LEMON_RC_I18N_TEXT("lemon-rc command line :")  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N_TEXT("\tscript file directory :") << scriptFileDirectory  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N_TEXT("\tproject version :") << projectVersion  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N_TEXT("\tproject path :") << projectPath  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N_TEXT("\tc/cxx generate files directory :") << generateFileDirectory  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N_TEXT("\tgenerate resource file path :") << resourceFilePath  << std::endl;

	try
	{
		AssemblyInfo info(
			projectVersion,
			projectPath,
			scriptFileDirectory,
			generateFileDirectory,
			createMSRC == LEMON_TEXT("TRUE"));

		info.WriteBinaryFile(resourceFilePath);
	}
	catch(const lemon::error_info & e)
	{
		std::cout << e << std::endl;
	}

	
}