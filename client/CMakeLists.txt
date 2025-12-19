# client/CMakeLists.txt

set(CLIENT_SOURCES
    acct_mgr.cpp
    acct_setup.cpp
    app.cpp
    app_config.cpp
    app_control.cpp
    app_graphics.cpp
    app_start.cpp
    async_file.cpp
    auto_update.cpp
    boinc_cmd.cpp
    boinc_log.cpp
    check_security.cpp
    check_state.cpp
    client_msgs.cpp
    client_state.cpp
    client_types.cpp
    coproc_sched.cpp
    cpu_sched.cpp
    cs_account.cpp
    cs_apps.cpp
    cs_benchmark.cpp
    cs_cmdline.cpp
    cs_files.cpp
    cs_notice.cpp
    cs_platforms.cpp
    cs_prefs.cpp
    cs_proxy.cpp
    cs_scheduler.cpp
    cs_sporadic.cpp
    cs_statefile.cpp
    cs_trickle.cpp
    current_version.cpp
    detect_rosetta_cpu.cpp
    dhrystone.cpp
    dhrystone2.cpp
    file_names.cpp
    file_xfer.cpp
    gpu_amd.cpp
    gpu_detect.cpp
    gpu_intel.cpp
    gpu_nvidia.cpp
    gpu_opencl.cpp
    gui_http.cpp
    gui_rpc_server.cpp
    gui_rpc_server_ops.cpp
    http_curl.cpp
    log_flags.cpp
    main.cpp
    net_stats.cpp
    pers_file_xfer.cpp
    project.cpp
    project_list.cpp
    result.cpp
    rr_sim.cpp
    sandbox.cpp
    scheduler_op.cpp
    time_stats.cpp
    whetstone.cpp
    work_fetch.cpp
)

if(WIN32)
    list(APPEND CLIENT_SOURCES
        hostinfo_win.cpp
        sysmon_win.cpp
    )
endif()

add_executable(boinc_client ${CLIENT_SOURCES})

target_compile_definitions(boinc_client PRIVATE ENABLE_AUTO_UPDATE)

target_link_libraries(boinc_client PRIVATE
    libboinc
    libboinc_crypt
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
    SQLite::SQLite3
)

target_include_directories(boinc_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
)

if(WIN32)
    target_link_libraries(boinc_client PRIVATE
        wsock32 ws2_32 winmm user32 gdi32 shell32 advapi32
        psapi
        iphlpapi
    )
endif()
