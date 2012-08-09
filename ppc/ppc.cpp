/**
* 
* @file     ppc
* @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
* @author   yayanyang
* @version  1.0.0.0  
* @date     2012/08/06
*/
#include <stack>
#include <vector>
#include <iostream>
#include <lemonxx/ppc/package.hpp>

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


lemon::String CheckAssemblyInfoFile(const lemon::String & sourceDir)
{
	lemon::fs::directory_iteartor_t iter(sourceDir),end;

	while(iter != end)
	{
		if(*iter == LEMON_TEXT("assemblyinfo.lua")) return sourceDir + LEMON_TEXT("/assemblyinfo.lua");

		++ iter;
	}

	return lemon::String();
}

void ScanFiles(lemon::ppc::package & package,const lemon::String & sourceDir,const lemon::String & objectDir)
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

						objectDir + LEMON_TEXT("/") + entry.RelativePath + LEMON_TEXT("/g.") + *iter
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

		package.scan(iter->Source.c_str(),iter->Object.c_str());
	}
}


int main(int args,char * argv[])
{
	if(args != 4)
	{
		std::cout << "ppc <source dir> <generate cxx file dir> <binary package object dir>" << std::endl;

		return 1;
	}

	try
	{
		lemon::String sourceDir = lemon::from_locale(argv[1]);

		lemon::String objectDir = lemon::from_locale(argv[2]);

		lemon::String binaryDir = lemon::from_locale(argv[3]);

		lemon::String assemblyinfo = CheckAssemblyInfoFile(sourceDir);

		if(assemblyinfo.empty())
		{
			std::cout << "not found assemblyinfo.lua,skip package process" << std::endl;

			return 0;
		}

		lemon::String binaryPath = binaryDir + LEMON_TEXT("/metadata/");

		if(!lemon::fs::exists(binaryPath))
		{
			lemon::fs::create_directory(binaryPath);
		}

		lemon::ppc::package package(assemblyinfo);

		binaryPath += package.name();

		ScanFiles(package,sourceDir,objectDir);

		package.write(binaryPath.c_str());
	}
	catch(const lemon::error_info & e)
	{
		std::cout << e << std::endl;

		return 2;
	}
	catch(const lemon::Exception & e)
	{
		using namespace lemon;

		std::cout << "error :" << e.what() << std::endl;

		std::cout << e.ErrorInfo() << std::endl;
	}

	return 0;
}