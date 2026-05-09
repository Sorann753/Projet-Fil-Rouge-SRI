set_languages("cxx17")
add_requires("camera", {system = true})
add_requires("camera-base", {system = true})

target("proto-camera")
    set_kind("binary")
    add_files("main.cpp")
    add_includedirs("/usr/include/libcamera")
    add_syslinks("camera")
    add_syslinks("camera-base")

