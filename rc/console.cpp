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

using namespace lemon::rc::tools;

struct CxxFileEntry
{
	lemon::String Source;

	lemon::String Object;
};

struct CxxDirectoryEntry
{
	lemon::String Path;

	lemon::String RelativePath;
};

void ScanFiles(AssemblyInfo & info,const lemon::String & sourceDir,const lemon::String & objectDir)
{
	CxxDirectoryEntry root = {sourceDir,LEMON_TEXT("")};

	typedef std::vector<CxxFileEntry> CxxFileEntries;

	std::stack<CxxDirectoryEntry> directories;

	CxxFileEntries	files;

	directories.push(root);

	while(!directories.empty())
	{
		CxxDirectoryEntry entry = directories.top();

		directories.pop();

		lemon::fs::directory_iteartor_t iter(entry.Path),end;

		for(;iter != end ; ++ iter)
		{
			const lemon::String path = entry.Path + LEMON_TEXT('/') + *iter;

			if(*iter == LEMON_TEXT(".") || *iter == LEMON_TEXT("..")) continue;

			if(lemon::fs::is_directory(path))
			{
				CxxDirectoryEntry newDirectory = {path,entry.RelativePath + LEMON_TEXT("/") + *iter};

				directories.push(newDirectory);
			}
			else
			{
				if(iter->rfind(LEMON_TEXT(".cpp")) == iter->length() -  4 ||
					iter->rfind(LEMON_TEXT(".cxx")) == iter->length() -  4 ||
					iter->rfind(LEMON_TEXT(".cc")) == iter->length() -  3 )
				{
					CxxFileEntry newFile = 
					{
						entry.Path + LEMON_TEXT("/") + *iter,

						objectDir + LEMON_TEXT("/") + entry.RelativePath + *iter + LEMON_TEXT(".g.hpp")
					};

					files.push_back(newFile);
				}

				if(iter->rfind(LEMON_TEXT(".h")) == iter->length() -  2 ||
					iter->rfind(LEMON_TEXT(".hpp")) == iter->length() -  4 ||
					iter->rfind(LEMON_TEXT(".hxx")) == iter->length() -  4 )
				{
					CxxFileEntry newFile = 
					{
						entry.Path + LEMON_TEXT("/") + *iter
					};

					files.push_back(newFile);
				}
			}
		}
	}

	CxxFileEntries::const_iterator iter,end = files.end();

	for(iter = files.begin(); iter != end; ++ iter)
	{
		std::cout << "[lemon-ppc] parse cxx source file :" << lemon::to_locale(iter->Source) << std::endl;

		std::cout << "[lemon-ppc] gen cxx source file :" << lemon::to_locale(iter->Object) << std::endl;

		info.GenerateFile(iter->Source,iter->Object);
	}
}


#ifdef WIN32
int wmain(int args, wchar_t * argv[])
#else
int main(int args, char * argv[])
#endif //WIN32
{
	if(args != 8)
	{
		std::cout << "lemon-rc <script file path> <project-name> <version>  <path> <c_cxx generate file output directory> <object resource path> <create asembly.rc>"  << std::endl;

		return 1;
	}

	lemon::String scriptFilePath = argv[1];

	lemon::String projectName = argv[2];

	lemon::String projectVersion = argv[3];

	lemon::String projectPath = argv[4];

	lemon::String generateFileDirectory = argv[5];

	lemon::String resourceFilePath = argv[6];

	lemon::String createMSRC = argv[7];

	LEMON_COUT << TOOLS_LEMON_RC_I18N("lemon-rc command line :")  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N("\tscript file path :") << scriptFilePath  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N("\tproject name :") << projectName  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N("\tproject version :") << projectVersion  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N("\tproject path :") << projectPath  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N("\tc/cxx generate files directory :") << generateFileDirectory  << std::endl;

	LEMON_COUT << TOOLS_LEMON_RC_I18N("\tgenerate resource file path :") << resourceFilePath  << std::endl;

	try
	{
		AssemblyInfo info(
			projectName,
			projectVersion,
			projectPath + LEMON_TEXT("/assemblyinfo.lua"),
			scriptFilePath,
			generateFileDirectory,
			createMSRC == LEMON_TEXT("TRUE"));

		ScanFiles(info,projectPath,generateFileDirectory);

		info.Write(resourceFilePath);
	}
	catch(const lemon::error_info & e)
	{
		std::cout << e << std::endl;
	}

	
}