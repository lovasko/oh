// Copyright (c) 2019 Daniel Lovasko
// All Rights Reserved
//
// Distributed under the terms of the 2-clause BSD License. The full
// license is in the file LICENSE, distributed as part of this software.

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "types.h"


/// Print help usage message to the standard error stream.
static void
print_help(void)
{
  (void)fprintf(stderr,
	  "oh - opening hours for a UNIX process\n\n"
		"Usage:\n"
		"  oh [-hsw] [-c HH:MM] [-o HH:MM] command [arguments]\n\n"
		"Options:\n"
		"  -c HH:MM  Closing time. Default is 18:45.\n"
		"  -h        Help message.\n"
		"  -o HH:MM  Opening time. Default is 06:15.\n"
		"  -s        Use SIGSTOP instead of SIGTSTP.\n"
		"  -w        Allow unlimited weekend running.\n"
	);
}

/// Parse the user-selected times in format HH:MM.
/// @return success/failure indication
///
/// @param[out] msm minutes since midnight
/// @param[in]  str input string
static bool
parse_time(uint16_t* msm, const char* str)
{
  char* retc;
  struct tm time;

  // Parse a time string expected to be formatted as HH:MM.
  retc = strptime(str, "%R", &time);
  
  // Check for total parsing failure.
  if (retc == NULL) {
    fprintf(stderr, "ERR: unable to parse the time in form of '%s'\n", str);
    return false;
  }

  // Check whether the string does not have any excess characters.
  if (*retc != '\0') {
    fprintf(stderr, "ERR: excess characters at end of time '%s'\n", str);
    return false;
  }

  // Compute the number of minutes since start of the day.
  *msm = (uint16_t)(time.tm_hour * 60 + time.tm_min);
  return true;
}

/// Parse the command-line arguments into a configuration object.
/// @return success/failure indication
///
/// @param[in] conf configuration
/// @param[in] argc number of arguments
/// @param[in] argv array of arguments
bool
parse_config(struct config* conf, int argc, char* argv[])
{
  int opt;
	int retb;

	// Assign defaults. We are confident to skip the verification of the time
	// parsing here, as the defaults are hand-crafted to work well.
	conf->cf_stop = false;
	conf->cf_wknd = false;
	(void)parse_time(&conf->cf_omsm, "18:45");
	(void)parse_time(&conf->cf_cmsm, "06:15");

	while (true) {
	  // Obtain the next option/flag.
	  opt = getopt(argc, argv, "c:ho:sw");
		if (opt == -1)
		  break;

	  switch (opt) {
		  // Closing time.
		  case 'c':
			  retb = parse_time(&conf->cf_cmsm, optarg);
				if (retb == false) {
				  fprintf(stderr, "unable to parse closing time '%s'\n", optarg);
					return false;
				}
			  break;

		  case 'h':
			  print_help();
				exit(EXIT_FAILURE);

      // Opening time.
		  case 'o':
			  retb = parse_time(&conf->cf_omsm, optarg);
				if (retb == false) {
				  fprintf(stderr, "unable to parse opening time '%s'\n", optarg);
					return false;
				}
			  break;

      // Use SIGSTOP instead of SIGTSTP.
		  case 's':
			  conf->cf_stop = true;
			  break;

      // Allow unlimited running during weekends.
			case 'w':
			  conf->cf_wknd = true;
			  break;
		}
	}
  
	// Parse the positional arguments.
	if (optind == argc) {
	  fprintf(stderr, "no command to execute was provided\n");
		return false;
	}

	conf->cf_path = argv[optind];
  conf->cf_argv = &argv[optind + 1];

  return true;
}
