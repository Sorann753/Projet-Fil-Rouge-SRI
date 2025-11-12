add_rules("mode.debug", "mode.release")

target("PFR-simulator")
    set_kind("binary")
    add_includedirs("include/")
    add_files("source/*.c")
    add_cflags("-Wall", "-Wextra", "-pedantic", "-Wfloat-equal", "-Wshadow")
    add_cflags("-Wpointer-arith", "-Wcast-align", "-Wstrict-prototypes", "-Wstrict-overflow=3")
    add_cflags("-Wwrite-strings", "-Wformat=2")
    set_warnings("all", "error")
    set_languages("c11")

    if is_mode("debug") then
        add_cflags("-g2", "-fsanitize={address, thread, undefined}")
    else
        set_optimize("faster")
    end

target("PFR-hardware")
    set_kind("binary")
    add_includedirs("include/")
    add_files("source/*.c")
    