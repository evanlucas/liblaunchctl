#include <stdio.h>
#include "liblaunchctl.h"

int usage() {
  printf("usage:\n");
  printf("\t./test_launchctl <command> <options>\n");
  printf("\n");
  printf("commands:\n");
  printf("\tstart <job_label>\n");
  printf("\tstop <job_label>\n");
  printf("\tlist <job_label>\n");
  printf("\tlistall\n");
  printf("\tload <path>\n");
  printf("\tunload <path>\n");
  return 1;
}

/*
 * Taken from launchctl.c
 *
 * Source can be found at
 * http://opensource.apple.com/source/launchd/launchd-442.26.2/
 *
 */
void print_obj(launch_data_t job, const char *key, void *context) {
	static size_t indent = 0;
	size_t i, c;
  if (job == NULL) {
    fprintf(stdout, "Unknown job\n");
    return;
  }
	for (i=0; i<indent; i++) {
		fprintf(stdout, "\t");
	}
  
	if (key) {
		fprintf(stdout, "\"%s\" = ", key);
	}
  
	switch(launch_data_get_type(job)) {
		case LAUNCH_DATA_STRING:
			fprintf(stdout, "\"%s\";\n", launch_data_get_string(job));
			break;
		case LAUNCH_DATA_INTEGER:
			fprintf(stdout, "%lld;\n", launch_data_get_integer(job));
			break;
		case LAUNCH_DATA_REAL:
			fprintf(stdout, "%f;\n", launch_data_get_real(job));
			break;
		case LAUNCH_DATA_BOOL:
			fprintf(stdout, "%s;\n", launch_data_get_bool(job) ? "true" : "false");
			break;
		case LAUNCH_DATA_ARRAY:
			c = launch_data_array_get_count(job);
			fprintf(stdout, "(\n");
			indent++;
			for (i=0; i<c; i++) {
				print_obj(launch_data_array_get_index(job, i), NULL, NULL);
			}
			indent--;
			for (i=0; i<indent; i++) {
				fprintf(stdout, "\t");
			}
			fprintf(stdout, ");\n");
			break;
		case LAUNCH_DATA_DICTIONARY:
			fprintf(stdout, "{\n");
			indent++;
			launch_data_dict_iterate(job, print_obj, NULL);
			indent--;
			for (i=0; i<indent; i++) {
				fprintf(stdout, "\t");
			}
			fprintf(stdout, "};\n");
			break;
		case LAUNCH_DATA_FD:
			fprintf(stdout, "file-descriptor-object;\n");
			break;
		case LAUNCH_DATA_MACHPORT:
			fprintf(stdout, "mach-port-object;\n");
			break;
		default:
			fprintf(stdout, "???;\n");
			break;
	}
}

int main(int argc, char *argv[]) {
//	if (argc < 2) {
//    return usage();
//  }
//  
//  if (!strcmp(argv[1], "start")) {
//    if (argc != 3) {
//      return usage();
//    } else {
//      int r = launchctl_start_job(argv[2]);
//      return r;
//    }
//  } else if (!strcmp(argv[1], "stop")) {
//    if (argc != 3) {
//      return usage();
//    } else {
//      int r = launchctl_stop_job(argv[2]);
//      return r;
//    }
//  } else if (!strcmp(argv[1], "list")) {
//    if (argc != 3) {
//      return usage();
//    } else {
//      launch_data_t res = launchctl_list_job(argv[2]);
//      if (res == NULL) {
//        printf("Invalid job\n");
//        return 1;
//      } else {
//        print_obj(res, NULL, NULL);
//        launch_data_free(res);
//        return 0;
//      }
//    }
//  } else if (!strcmp(argv[1], "listall")) {
    jobs_list_t s = launchctl_list_jobs();
    if (s == NULL) {
      printf("An error occurred fetching all jobs\n");
      return 1;
    }
    int count = s->count;
    printf("Found %d jobs\n", count);
    for (int i=0; i<count; i++) {
      launch_data_status_t q = &s->jobs[i];
      printf("Job: %s\t PID: %d\t Status: %d\n", q->label, q->pid, q->status);
    }
    launch_data_status_free(s->jobs);
    jobs_list_free(s);
//  } else if (!strcmp(argv[1], "manager")) {
//    char *mgmr = launchctl_get_managername();
//    printf("Manager: %s\n", mgmr);
//    free(mgmr);
//  }
}
