PROJECT_NAME = "BF-Interpreter"

set_languages "cxx20"

add_rules "mode.debug"
add_rules "mode.release"

target(PROJECT_NAME)
    set_kind "binary" 
    
    set_basename "cpp_$(mode)_$(arch)"

    set_targetdir "bin"
    set_objectdir "build/obj"
    
    add_includedirs "src/"

    add_headerfiles{ "src/**.h", "src/**.hpp" }
    add_files{ "src/**.c", "src/**.cpp" }
    
target_end()
