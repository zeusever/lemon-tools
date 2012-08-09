title = 
[==[
/**
 * this file is auto generate by assemblyinfoc,do not modify it
 * @file     assembly.h
 * @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
 * @author   yayanyang
 * @version  1.0.0.0  
 * @date     2012/01/14
 */
]==]

rc=
[==[
#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "afxres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// Chinese (Simplified, PRC) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
LANGUAGE LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE 
BEGIN
    "resource1.h\0"
END

2 TEXTINCLUDE 
BEGIN
    "#include ""afxres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE 
BEGIN
    "\r\n"
    "\0"
END

#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
 FILEVERSION ${version}
 PRODUCTVERSION ${version}
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x40004L
 FILETYPE 0x0L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "080404b0"
        BEGIN
            VALUE "CompanyName", "${company}"
            VALUE "FileDescription", "${description}"
            VALUE "FileVersion", "${version_string}"
            VALUE "InternalName", "${name}"
            VALUE "LegalCopyright", "Copyright (C) ${YEAR} ${company} All Rights Reserved."
            VALUE "OriginalFilename", "${name}"
            VALUE "ProductName", "${name}"
            VALUE "ProductVersion", "${version_string}"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x804, 1200
    END
END

#endif    // Chinese (Simplified, PRC) resources
/////////////////////////////////////////////////////////////////////////////



#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//


/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED
]==]

function generate(info,dir,name,versionstring)
	
	if assembly.errorcode ~= nil then

		for i,v in ipairs(assembly.errorcode) do
			if v.description ~= nil then
				info:errorinfo(i,v.name,v.description)
			else
				info:errorinfo(i,v.name,"lemon error info")
			end
		end
	
	end
	
	prefix = string.upper(string.gsub(name,"[%-]","_"))
	
	if assembly.trace ~= nil then
		info:tracename(assembly.trace.name)
		
		if assembly.trace.catalog ~= nil then
			for i,v in ipairs(assembly.trace.catalog) do
				info:tracecatalog(i,v.name,v.description)
			end
		end
	else
		info:tracename(prefix .. "_TRACE")
	end
	
	guid = info:guid()
	
	local version = {}
	
	version[0],version[1],version[2],version[3] = versionstring:match("(%d+)%.(%d+)%.(%d+)%.(%d+)$")
	
	local i18name = prefix .. "_I18N"
	
	info:i18nname(i18name)

	assembly_h = dir .. "/assembly.h"

	assembly_cpp = dir .. "/assembly.cpp"

	errorcode_h = dir .. "/errorcode.h"

	assembly_rc = dir .. "/assembly.rc"

	headFile = assert(io.open(assembly_h,"w+"),"can't open file to write :" .. assembly_h)

	sourceFile = assert(io.open(assembly_cpp,"w+"),"can't open file to write :" .. assembly_cpp)

	errorFile = assert(io.open(errorcode_h,"w+"),"can't open file to write :" .. errorcode_h)
	
	rcFile = assert(io.open(assembly_rc,"w+"),"can't open file to write :" .. assembly_rc)

	headFile:write(title)

	headFile:write("#ifndef " .. prefix .. "_ASSEMBLY_H\n")
	headFile:write("#define " .. prefix .. "_ASSEMBLY_H\n")
	errorFile:write("#ifndef " .. prefix .. "_ERRORCODE_H\n")
	errorFile:write("#define " .. prefix .. "_ERRORCODE_H\n")

	headFile:write("#include \"configure.h\"\n")
	headFile:write("#include <lemon/sys/abi.h>\n\n")
	errorFile:write("#include \"configure.h\"\n")
	errorFile:write("#include <lemon/sys/abi.h>\n\n")
	sourceFile:write("#include \"assembly.h\"\n\n")
	sourceFile:write("#include \"errorcode.h\"\n\n")
	
	print("guid :" .. guid)

	headFile:write(prefix .. "_API const LemonUuid " .. prefix .. "_GUID;\n\n")
	headFile:write(prefix .. "_API const LemonVersion " .. prefix .. "_VERSION;\n\n")
	
	headFile:write("#define " .. i18name .. "(msg)\tLemonI18nText(&" .. prefix .. "_GUID,LEMON_TEXT(msg))\n\n")
	
	sourceFile:write("const LemonUuid " .. prefix .. "_GUID = " .. guid .. ";\n\n")
	sourceFile:write("const LemonVersion " .. prefix .. "_VERSION = {" .. version[0] .. "," .. version[1] .. "," .. version[2] .. "," .. version[3] .. "};\n\n")

	index = 0

	if(nil ~= assembly.errorcode) then

		for k,v in ipairs(assembly.errorcode) do 
			errorFile:write(prefix .. "_API const LemonError " .. prefix .. "_" .. v.name .. ";\n\n")
			sourceFile:write("const LemonError " .. prefix .. "_" .. v.name .. " = {&" .. prefix .. "_GUID," .. k .. "};\n\n")
		end

	end

	headFile:write("#endif //" .. string.upper(prefix) .. "_ASSEMBLY_H\n")
	errorFile:write("#endif //" .. string.upper(prefix) .. "_ERRORCODE_H\n")
	
	rc = string.gsub(rc,"${version}",version[0] .. "," .. version[1] .. "," .. version[2] .. "," .. version[3])

	rc = string.gsub(rc,"${version_string}",version[0] .. "." .. version[1] .. "." .. version[2] .. "." .. version[3])

	rc = string.gsub(rc,"${YEAR}",os.date("%Y",os.time()))

	if(nil ~= assembly.company) then rc = string.gsub(rc,"${company}",assembly.company) end

	if(nil ~= assembly.description) then rc = string.gsub(rc,"${description}",assembly.description) end
	
	
	if(nil ~= assembly.name) then rc = string.gsub(rc,"${name}",assembly.name) end
	
	rcFile:write(rc)

	headFile:close()
	sourceFile:close()
	errorFile:close()
	
	rcFile:close()
end
