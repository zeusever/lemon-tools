trace_hpp_t = 
[==[
/**
 * this file is auto generate by assemblyinfoc,do not modify it
 * @file     assembly.h
 * @brief    Copyright (C) 2012  yayanyang All Rights Reserved 
 * @author   yayanyang
 * @version  1.0.0.0  
 * @date     ${DATE}
 */
#include "assembly.h"
#include <lemonxx/trace/trace.hpp>

template <size_t Lines> struct __lemon_dtrace_log;

${IMPL}

${DTRACE_MACRO}

]==]

trace_hpp_function_t = 
[==[
template <> struct __lemon_dtrace_log<${LINE}>
{
	template<typename Provider ${ARGS_T}>
	void operator()( Provider provider , lemon_uint8_t level , lemon_uint32_t flags , const char * ${ARGS} )
	{

	}
};
]==]

function generate_sub_file( path , cassembly , trace_metadatas )
	-- body
	local sub_files = assert(io.open(path,"w+"),"can't open file to write :" .. path)

	local sub_files_codes = string.gsub( trace_hpp_t , "${DATE}" , os.date("%d/%m/%Y",os.time() ) )

	local prefix = string.upper(string.gsub(string.gsub(cassembly:name(),"[%-]","_"),"%+","X"))

	sub_files_codes = string.gsub( sub_files_codes , "${DTRACE_MACRO}" , "#define " .. cassembly:trace_macro_name() .. "\t__lemon_dtrace_log<__LINE__>()")

	local function_codes = ""

	while trace_metadatas:next() do
		
		local lines , args = trace_metadatas:value()

		local ARGS_T  = ""

		local ARGS  = ""

		for i = 0 , args - 4, 1 do
			ARGS_T = ARGS_T .. ", A" .. i

			ARGS = ARGS .. ", A" .. i .. " a" .. i
		end

		function_codes = function_codes .. string.gsub(trace_hpp_function_t,"${LINE}",lines)

		function_codes = string.gsub(function_codes,"${ARGS_T}",ARGS_T)

		function_codes = string.gsub(function_codes,"${ARGS}",ARGS)
	end

	sub_files_codes = string.gsub(sub_files_codes,"${IMPL}",function_codes)

	sub_files:write(sub_files_codes)
end