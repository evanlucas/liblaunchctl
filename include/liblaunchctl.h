/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_APACHE_LICENSE_HEADER_START@
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @APPLE_APACHE_LICENSE_HEADER_END@
 */

/*
 * Based on the launchd program
 *
 * Most of this comes from launchctl
 * The original source can be found at 
 * http://opensource.apple.com/source/launchd/launchd-442.26.2/
 *
 * Modified By Evan Lucas
 *
 * Last modified on 5/5/2013
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <launch.h>
#include "launch_priv.h"
#include <vproc.h>
#include "vproc_priv.h"
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/in_var.h>
#include <netdb.h>
#include <sys/un.h>
#include <CoreFoundation/CoreFoundation.h>
#include <dirent.h>
#include <fnmatch.h>
#include <glob.h>
#include <pwd.h>
#include <sys/syslimits.h>
#include "assumes.h"


extern int * __error(void);
#define errno (*__error())


struct _launch_data {
  uint64_t type;
  union {
    struct {
      union {
        launch_data_t *_array;
        char *string;
        void *opaque;
        int64_t __junk;
      };
      union {
        uint64_t _array_cnt;
        uint64_t string_len;
        uint64_t opaque_size;
      };
    };
    int64_t fd;
    uint64_t  mp;
    uint64_t err;
    int64_t number;
    uint64_t boolean; /* We'd use 'bool' but this struct needs to be used under Rosetta, and sizeof(bool) is different between PowerPC and Intel */
    double float_num;
  };
};

vproc_err_t vproc_swap_complex(vproc_t vp, vproc_gsk_t key, launch_data_t inval, launch_data_t *outval);



struct ldtstatus {
  char *label;
  int pid;
  int status;
};

typedef struct ldtstatus *launch_data_status_t;


struct jobslist {
  launch_data_status_t jobs;
  int count;
};

struct load_unload_state {
	launch_data_t pass1;
	char *session_type;
	bool editondisk:1, load:1, forceload:1;
};

typedef struct jobslist *jobs_list_t;

#define CFTypeCheck(cf, type) (CFGetTypeID(cf) == type ## GetTypeID())
#define CFReleaseIfNotNULL(cf) if (cf) CFRelease(cf);
#define VPROC_MAGIC_UNLOAD_SIGNAL 0x4141504C
#define LAUNCH_SECDIR _PATH_TMP "launch-XXXXXX"
#define	SO_EXECPATH	0x1085

#pragma mark Public Functions

launch_data_t launchctl_list_job(const char *job);
jobs_list_t launchctl_list_jobs();
void jobs_list_free(jobs_list_t j);
void launch_data_status_free(launch_data_status_t j);
int launchctl_start_job(const char *job);
int launchctl_stop_job(const char *job);
int launchctl_remove_job(const char *job);
int launchctl_load_job(const char *job, bool editondisk, bool forceload, const char *session_type, const char *domain);
int launchctl_unload_job(const char *job);

#pragma mark Private

CFTypeRef CFTypeCreateFromLaunchData(launch_data_t obj);
CFArrayRef CFArrayCreateFromLaunchArray(launch_data_t arr);
CFDictionaryRef CFDictionaryCreateFromLaunchDictionary(launch_data_t dict);
bool launch_data_array_append(launch_data_t a, launch_data_t o);
void insert_event(launch_data_t, const char *, const char *, launch_data_t);
void distill_jobs(launch_data_t);
void distill_config_file(launch_data_t);
void distill_fsevents(launch_data_t);
void sock_dict_cb(launch_data_t what, const char *key, void *context);
void sock_dict_edit_entry(launch_data_t tmp, const char *key, launch_data_t fdarray, launch_data_t thejob);
CFPropertyListRef CreateMyPropertyListFromFile(const char *);
CFPropertyListRef CFPropertyListCreateFromFile(CFURLRef plistURL);
void WriteMyPropertyListToFile(CFPropertyListRef, const char *);
bool path_goodness_check(const char *path, bool forceload);
void readpath(const char *, struct load_unload_state *);
void readfile(const char *, struct load_unload_state *);
void submit_job_pass(launch_data_t jobs);
bool path_check(const char *path);
