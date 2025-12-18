# lib/CMakeLists.txt

# --- libboinc ---

set(GENERIC_SOURCES
    app_ipc.cpp
    base64.cpp
    cc_config.cpp
    cert_sig.cpp
    coproc.cpp
    diagnostics.cpp
    filesys.cpp
    gui_rpc_client.cpp
    gui_rpc_client_ops.cpp
    gui_rpc_client_print.cpp
    hostinfo.cpp
    keyword.cpp
    md5.cpp
    md5_file.cpp
    mem_usage.cpp
    mfile.cpp
    miofile.cpp
    msg_log.cpp
    network.cpp
    notice.cpp
    opencl_boinc.cpp
    parse.cpp
    prefs.cpp
    procinfo.cpp
    proc_control.cpp
    project_init.cpp
    proxy_info.cpp
    sched_msgs.cpp
    shmem.cpp
    str_util.cpp
    url.cpp
    util.cpp
    wslinfo.cpp
)

set(PLATFORM_SOURCES "")

if(WIN32)
    list(APPEND PLATFORM_SOURCES
        boinc_win.cpp
        daemonmgt_win.cpp
        diagnostics_win.cpp
        idlemon_win.cpp
        procinfo_win.cpp
        run_app_windows.cpp
        stackwalker_win.cpp
        win_util.cpp
    )
elseif(APPLE)
    list(APPEND PLATFORM_SOURCES
        procinfo_mac.cpp
        mac/mac_backtrace.cpp
        mac/mac_spawn.cpp
        mac/mac_branding.cpp
    )
else() # Linux/Unix
    list(APPEND PLATFORM_SOURCES
        procinfo_unix.cpp
        synch.cpp
        unix_util.cpp
    )
endif()

add_library(libboinc STATIC ${GENERIC_SOURCES} ${PLATFORM_SOURCES})

# Dependencies
target_link_libraries(libboinc PUBLIC
    OpenSSL::SSL OpenSSL::Crypto
    CURL::libcurl
    ZLIB::ZLIB
)

target_include_directories(libboinc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)

if(WIN32)
    target_link_libraries(libboinc PUBLIC wsock32 ws2_32 winmm user32 gdi32 shell32 advapi32)
endif()

# --- libboinc_crypt ---

add_library(libboinc_crypt STATIC crypt.cpp)
target_link_libraries(libboinc_crypt PUBLIC libboinc OpenSSL::Crypto)

# --- libboinc_fcgi (Optional) ---
# (Skipping for now until FCGI requirement is verified)
