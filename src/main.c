// Copyright (c) 2019 Daniel Lovasko
// All Rights Reserved
//
// Distributed under the terms of the 2-clause BSD License. The full
// license is in the file LICENSE, distributed as part of this software.

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "proto.h"
#include "types.h"


int
main(int argc, char* argv[])
{
  struct config conf;
	bool retb;
	pid_t pid;

  retb = parse_config(&conf, argc, argv);
	if (retb == false) {
	  fprintf(stderr, "unable to parse configuration\n");
		return EXIT_FAILURE;
	}

	retb = block_signal_delivery();
	if (retb == false) {
	  fprintf(stderr, "unable to block signal delivery\n");
	  return EXIT_FAILURE;
	}

	retb = install_signal_handlers();
	if (retb == false) {
    fprintf(stderr, "unable to install signal handlers\n");
    return EXIT_FAILURE;
	}

	retb = start_process(&pid, &conf);
	if (retb == false) {
	  fprintf(stderr, "unable to start the process\n");
	  return EXIT_FAILURE;
	}

	retb = control_process(pid, &conf);
	if (retb == false) {
	  fprintf(stderr, "monitoring loop finished unexpectedly\n");
		return EXIT_FAILURE;
	}

  return EXIT_SUCCESS;
}
