set_languages("cxx17")
add_requires("libcamera", {system = true})

target("proto-camera")
    set_kind("binary")
    add_files("main.cpp")
    add_includedirs("/usr/include/libcamera")
    add_syslinks("libcamera")

