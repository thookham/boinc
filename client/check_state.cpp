// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2008 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

// Functions to check the integrity of client data structures.
// Not currently used, but might be handy if *0 type crashes occur

#include "cpp.h"

#ifdef _WIN32
#include "boinc_win.h"
#else
#include "config.h"
#include <cassert>
#endif

#include "client_msgs.h"
#include "client_state.h"
#include "result.h"

void CLIENT_STATE::check_project_pointer(PROJECT* p) {
    for (auto const& proj : projects) {
        if (p == proj.get()) return;
    }
    assert(0);
}
void CLIENT_STATE::check_app_pointer(APP* p) {
    for (auto const& a : apps) {
        if (p == a.get()) return;
    }
    assert(0);
}
void CLIENT_STATE::check_file_info_pointer(FILE_INFO* p) {
    for (auto const& f : file_infos) {
        if (p == f.get()) return;
    }
    assert(0);
}
void CLIENT_STATE::check_app_version_pointer(APP_VERSION* p) {
    for (auto const& av : app_versions) {
        if (p == av.get()) return;
    }
    assert(0);
}
void CLIENT_STATE::check_workunit_pointer(WORKUNIT* p) {
    for (auto const& wu : workunits) {
        if (p == wu.get()) return;
    }
    assert(0);
}
void CLIENT_STATE::check_result_pointer(RESULT* p) {
    for (auto const& r : results) {
        if (p == r.get()) return;
    }
    assert(0);
}

void CLIENT_STATE::check_pers_file_xfer_pointer(PERS_FILE_XFER* p) {
    unsigned int i;
    for (i=0; i<pers_file_xfers->pers_file_xfers.size(); i++) {
        if (p == pers_file_xfers->pers_file_xfers[i]) return;
    }
    assert(0);
}
void CLIENT_STATE::check_file_xfer_pointer(FILE_XFER* p) {
    unsigned int i;
    for (i=0; i<file_xfers->file_xfers.size(); i++) {
        if (p == file_xfers->file_xfers[i]) return;
    }
    assert(0);
}

void CLIENT_STATE::check_app(APP& p) {
    check_project_pointer(p.project);
}

void CLIENT_STATE::check_file_info(FILE_INFO& p) {
    if (p.pers_file_xfer) {
        check_pers_file_xfer_pointer(p.pers_file_xfer);
    }
    check_project_pointer(p.project);
}

void CLIENT_STATE::check_file_ref(FILE_REF& p) {
    check_file_info_pointer(p.file_info);
}

void CLIENT_STATE::check_app_version(APP_VERSION& p) {
    check_app_pointer(p.app);
    check_project_pointer(p.project);
    for (auto& fr : p.app_files) {
        check_file_ref(fr);
    }
}

void CLIENT_STATE::check_workunit(WORKUNIT& p) {
    for (auto& fr : p.input_files) {
        check_file_ref(fr);
    }
    check_project_pointer(p.project);
    check_app_pointer(p.app);
}

void CLIENT_STATE::check_result(RESULT& p) {
    for (auto& fr : p.output_files) {
        check_file_ref(fr);
    }
    check_app_pointer(p.app);
    check_workunit_pointer(p.wup);
    check_project_pointer(p.project);
}

void CLIENT_STATE::check_active_task(ACTIVE_TASK& p) {
    check_result_pointer(p.result);
    check_workunit_pointer(p.wup);
    check_app_version_pointer(p.app_version);
}

void CLIENT_STATE::check_pers_file_xfer(PERS_FILE_XFER& p) {
    if (p.fxp) check_file_xfer_pointer(p.fxp);
    check_file_info_pointer(p.fip);
}

void CLIENT_STATE::check_file_xfer(FILE_XFER& p) {
    check_file_info_pointer(p.fip);
}

void CLIENT_STATE::check_all() {
    for (auto const& p : apps) {
        check_app(*p);
    }
    for (auto const& p : file_infos) {
        check_file_info(*p);
    }
    for (auto const& p : app_versions) {
        check_app_version(*p);
    }
    for (auto const& p : workunits) {
        check_workunit(*p);
    }
    for (auto const& p : results) {
        check_result(*p);
    }
    for (auto const& p : active_tasks.active_tasks) {
        check_active_task(*p);
    }
    for (auto const& p : pers_file_xfers->pers_file_xfers) {
        check_pers_file_xfer(*p);
    }
    for (auto const& p : file_xfers->file_xfers) {
        check_file_xfer(*p);
    }
}

// Deallocate memory.  Can be used to check for memory leaks.
// Turned off for now.
//
void CLIENT_STATE::free_mem() {
    projects.clear();
    apps.clear();
    file_infos.clear();
    app_versions.clear();
    workunits.clear();
    results.clear();

    active_tasks.free_mem();

    message_descs.cleanup();

    notices.clear();
    rss_feeds.clear();
    daily_xfer_history.clear();
}
