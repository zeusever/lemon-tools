require "assembly_c"

function onload( cassembly , c_cxx_gen_dir )
	-- body
	generate_assembly_c_cxx_file( cassembly , assembly , c_cxx_gen_dir)
end