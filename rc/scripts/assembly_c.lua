assembly_c_h_t = 
[==[

/**
 * this file is auto generate by assemblyinfoc,do not modify it
 * @file     assembly.h
 * @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
 * @author   yayanyang
 * @version  1.0.0.0  
 * @date     ${DATE}
 */
#ifndef ${GUARD}
#define ${GUARD}

#include "configure.h"
#include <lemon/sys/abi.h>
#include <lemon/resource/abi.h>

${CONTENT}

#endif //${GUARD}

]==]


assembly_c_cpp_t = 
[==[

#include "assembly.h"

${CONTENT}

]==]


assembly_rc_t =
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

function assembly_c_log( message )
	-- body
	print("[lemon-rc/assembly c/c++ file generate]\t" .. message)
end

function generate_assembly_c_cxx_file( cassembly , assembly , c_cxx_gen_dir )
	-- body
	assert(assembly.name ~= nil, "assemblyinfo.lua file must define assembly.name field")

	-- set the generate file path
	local assembly_h_file = c_cxx_gen_dir .. "/assembly.h"

	local assembly_cpp_file = c_cxx_gen_dir .. "/assembly.cpp"

	local assembly_rc_file = c_cxx_gen_dir .. "/assembly.rc"

	local assembly_h = assert(io.open(assembly_h_file,"w+"),"can't open file to write :" .. assembly_h_file)

	local assembly_cpp = assert(io.open(assembly_cpp_file,"w+"),"can't open file to write :" .. assembly_cpp_file)

	local assembly_rc = assert(io.open(assembly_rc_file,"w+"),"can't open file to write :" .. assembly_rc_file)

	-- print log message
	assembly_c_log("name :" .. assembly.name)

	assembly_c_log("genreate file :" .. assembly_h_file)

	assembly_c_log("genreate file :" .. assembly_cpp_file)



	-- convert the assembly name to macro name prefix
	local prefix = string.upper(string.gsub(string.gsub(assembly.name,"[%-]","_"),"%+","X"))
	-- set the export import macro name
	local export_import_prefix =  prefix .. "_API"
	-- set the c/c++ header file compiler guard macro
	local c_cxx_header_file_guard = prefix .. "_ASSEMLBY_H"

	local assembly_guid = prefix .. "_GUID"

	local assembly_version = prefix .. "_VERSION"

	cassembly:name(assembly.name)
	-- set the default trace macro name
	cassembly:trace_macro_name(prefix .. "_TRACE")
	-- set the default i18n text macro name
	cassembly:i18n_macro_name(prefix .. "_I18N_TEXT")
	-- replace guard macro name
	local assembly_h_codes = string.gsub(assembly_c_h_t,"${GUARD}",c_cxx_header_file_guard)

	local content_h_codes = export_import_prefix .. " const LemonUuid " .. assembly_guid .. ";\n\n"

	content_h_codes = content_h_codes .. export_import_prefix .. " const LemonVersion " .. assembly_version .. ";\n\n"

	content_h_codes = content_h_codes .. "#define " .. prefix .. "_I18N_TEXT(msg) \t" .. "LemonI18nText(&" .. assembly_guid .. ",LEMON_TEXT(msg))\n\n"

	content_h_codes = content_h_codes .. "#define " .. prefix .. "_ERROR_MESSAGE(error) \t" .. "LemonErrorMessage((error))\n\n"

	local content_cpp_codes =  "const LemonUuid " .. assembly_guid .. " = " .. cassembly:guid() .. ";\n\n"

	content_cpp_codes = content_cpp_codes .. "const LemonVersion " .. assembly_version .. " = " .. cassembly:version() .. ";\n\n"
	-- set the dtrace catalog
	if assembly.dtrace ~= nil then
		-- override the default dtrace macro name
		if assembly.dtrace.name ~= nil then cassembly:trace_macro_name(assembly.dtrace.name) end
		-- check if defined catalog
		if assembly.dtrace.catalog ~= nil then
			
			for value , metadata in ipairs(assembly.dtrace.catalog) do

				local name = prefix .. "_" .. metadata.name 
				
				content_h_codes = content_h_codes .. "#define " .. name .. "\t(lemon_uint32_t)(0x00000001 << " .. value .. ")\n\n"

				local description = ""

				if metadata.description ~= nil then description = metadata.description end

				cassembly:trace_catalog(bit32.arshift( 0x00000001, 0 - value ) , name , description)
			end
		end
	end 

	if assembly.errorcode ~= nil then
		for value , metadata in ipairs(assembly.errorcode) do

			local name = prefix .. "_" .. metadata.name 
			
			local description = ""

			if metadata.description ~= nil then description = metadata.description end

			content_h_codes = content_h_codes .. export_import_prefix .. " const LemonError " .. name .. ";\n\n"

			content_cpp_codes = content_cpp_codes .. " const LemonError " .. name .. " = {&" .. assembly_guid .. "," .. value .. "};\n\n"

			cassembly:error_info( value , name , description )
		end
	end

	-- replace content
	assembly_h_codes = string.gsub(assembly_h_codes,"${CONTENT}",content_h_codes)

	assembly_cpp_codes = string.gsub(assembly_c_cpp_t,"${CONTENT}",content_cpp_codes)

	-- write file
	assembly_h:write(assembly_h_codes)

	assembly_cpp:write(assembly_cpp_codes)

	-- assembly.rc file generate

	local version = {}

	version[0],version[1],version[2],version[3] = cassembly:version():match("{(%d+),(%d+),(%d+),(%d+)}$")

	local assembly_rc_codes = string.gsub(assembly_rc_t,"${version}",version[0] .. "," .. version[1] .. "," .. version[2] .. "," .. version[3])

	assembly_rc_codes = string.gsub(assembly_rc_codes,"${version_string}",version[0] .. "." .. version[1] .. "." .. version[2] .. "." .. version[3])

	assembly_rc_codes = string.gsub(assembly_rc_codes,"${YEAR}",os.date("%Y",os.time()))

	assembly_rc_codes = string.gsub(assembly_rc_codes,"${name}",assembly.name)

	if nil ~= assembly.company then assembly_rc_codes = string.gsub(assembly_rc_codes,"${company}",assembly.company) end

	if(nil ~= assembly.description) then assembly_rc_codes = string.gsub(assembly_rc_codes,"${description}",assembly.description) end



	assembly_rc:write(assembly_rc_codes)
end