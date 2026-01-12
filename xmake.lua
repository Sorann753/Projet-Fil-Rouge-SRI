add_rules("mode.debug", "mode.release")
add_cflags("-Wall", "-Wextra", "-pedantic", "-Wfloat-equal", "-Wshadow")
add_cflags("-Wpointer-arith", "-Wcast-align", "-Wstrict-prototypes", "-Wstrict-overflow=3")
add_cflags("-Wwrite-strings", "-Wformat=2")
add_cflags("-Wno-unused-result", "-Wno-unused-variable", "-Wno-unused-parameter")
set_warnings("all", "error")
set_languages("c11")

target("PFR-simulator")
    set_kind("binary")
    add_includedirs("include/")
    add_files("source/**.c")
    remove_files("source/test.c", "source/testCases/**c") --les trucs a ne pas compiler



target("PFR-hardware")
    set_default(false)
    set_kind("binary")
    add_includedirs("include/")
    add_files("source/*.c")



target("PFR-test")    
    set_default(false)
    set_rules("mode.debug")
    set_kind("binary")
    add_includedirs("include/")
    add_files("source/**.c")
    remove_files("source/main.c") --on ne compile pas le main vus qu'on compile les test
