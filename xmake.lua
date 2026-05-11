add_rules("mode.debug", "mode.release")
add_cflags("-Wall", "-Wextra", "-pedantic", "-Wfloat-equal", "-Wshadow")
add_cflags("-Wpointer-arith", "-Wcast-align", "-Wstrict-prototypes", "-Wstrict-overflow=3")
add_cflags("-Wwrite-strings", "-Wformat=2")
add_cflags("-Wno-unused-result", "-Wno-unused-variable", "-Wno-unused-parameter", "-Wno-float-equal")
--set_warnings("all", "error")
set_languages("c11")

add_requires("fltk")

target("PFR-simulator")
    set_default(false )
    set_kind("binary")
    add_includedirs("pfr-core/include/")
    add_files("pfr-core/source/**.c")
    remove_files("source/test.c", "source/unitTest.c", "source/testCases/**c") --les trucs a ne pas compiler

target("PFR-GUI")
    set_languages("cxx17")
    set_kind("binary")
    add_includedirs("GUI/include/")
    add_files("GUI/source/**.cpp")
    add_packages("fltk")


target("PFR-hardware")
    set_kind("binary")
    add_includedirs("pfr-core/include/")
    add_includedirs("pfr-core/include/utils/")
    add_includedirs("raspberry/include/")

    add_files("pfr-core/source/*.c")
    add_files("raspberry/source/*.cpp")

    remove_files("pfr-core/source/main.c", "pfr-core/source/test.c", "pfr-core/source/unitTest.c", "pfr-core/source/testCases/**c") --les trucs a ne pas compiler



target("PFR-test")    
    set_default(false)
    set_rules("mode.debug")
    set_kind("binary")
    add_includedirs("pfr-core/include/")
    add_files("pfr-core/source/**.c")
    remove_files("pfr-core/source/main.c", "pfr-core/source/communication/main_server.c", "pfr-core/source/unitTest.c") --on ne compile pas le main vus qu'on compile les test

target("PFR-unit")    
    set_default(false)
    set_rules("mode.debug")
    set_kind("binary")
    add_includedirs("pfr-core/include/")
    add_files("pfr-core/source/**.c")
    remove_files("source/main.c", "source/test.c") --on ne compile pas le main vus qu'on compile les test


