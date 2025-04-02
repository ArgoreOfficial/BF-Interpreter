
set_languages "cxx17"

add_rules "mode.debug"
add_rules "mode.release"

target "C++ xmake Template"
    set_kind "binary" 
    
    set_basename "cpp_$(mode)_$(arch)"

    set_targetdir "bin"
    set_objectdir "build/obj"
    
    add_includedirs "src/"

    add_headerfiles{ "src/**.h", "src/**.hpp" }
    add_files{ "src/**.c", "src/**.cpp" }
    
target_end()
