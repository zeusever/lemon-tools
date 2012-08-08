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
#include <tools/lemon/rc/assemblyinfo.hpp>

using namespace lemon::rc::tools;

#ifdef WIN32
int wmain(int args, wchar_t * argv[])
#else
int main(int args, char * argv[])
#endif //WIN32
{
	if(args != 7)
	{
		std::cout << "lemon-rc <script file path> <project-name> <version>  <path> <c_cxx generate file output directory> <object resource path>"  << std::endl;

		return 1;
	}

	lemon::String scriptFilePath = argv[1];

	lemon::String projectName = argv[2];

	lemon::String projectVersion = argv[3];

	lemon::String projectPath = argv[4];

	lemon::String generateFileDirectory = argv[5];

	lemon::String resourceFilePath = argv[6];

	LEMON_COUT << LEMON_TEXT("lemon-rc command line :")  << std::endl;

	LEMON_COUT << LEMON_TEXT("\tscript file path :") << scriptFilePath  << std::endl;

	LEMON_COUT << LEMON_TEXT("\tproject name :") << projectName  << std::endl;

	LEMON_COUT << LEMON_TEXT("\tproject version :") << projectVersion  << std::endl;

	LEMON_COUT << LEMON_TEXT("\tproject path :") << projectPath  << std::endl;

	LEMON_COUT << LEMON_TEXT("\tc/cxx generate files directory :") << generateFileDirectory  << std::endl;

	LEMON_COUT << LEMON_TEXT("\tgenerate resource file path :") << resourceFilePath  << std::endl;

	try
	{
		AssemblyInfo info(
			projectName,
			projectVersion,
			projectPath + LEMON_TEXT("/assemblyinfo.lua"),
			scriptFilePath,
			generateFileDirectory);
	}
	catch(const lemon::error_info & e)
	{
		std::cout << e << std::endl;
	}

	
}