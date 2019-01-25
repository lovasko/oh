// Copyright (c) 2019 Daniel Lovasko
// All Rights Reserved
//
// Distributed under the terms of the 2-clause BSD License. The full
// license is in the file LICENSE, distributed as part of this software.

#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>


// Signal indicator flags.
volatile bool salrm; ///< SIGALRM flag.
volatile bool schld; ///< SIGCHLD flag.

/// Signal handler to be executed upon receipt of a signal. This function is
/// intended to only raise a signal flag, but shall not execute the actual
/// response code.
///
/// @global salrm
/// @global schld
///
/// @param[in] sign signal number
static void
signal_handler(int sign)
{
  if (sign == SIGALRM)
    salrm = true;

  if (sign == SIGCHLD)
    schld = true;
}

/// Block all signals except the expected SIGALRM and SIGCHLD.
/// @return success/failure indication
bool
block_signal_delivery(void)
{
  sigset_t set;
  int reti;

  (void)sigfillset(&set);
  (void)sigdelset(&set, SIGALRM);
  (void)sigdelset(&set, SIGCHLD);

  // Apply selected mask to block all but the two signals.
  reti = sigprocmask(SIG_SETMASK, &set, NULL);
  if (reti == -1) {
    perror("unable to block delivery of signals");
    return false;
  }

  return true;
}

/// Add responses to the SIGALRM and SIGCHLD signals.
/// @return success/failure indication
///
/// @global salrm
/// @global schld
bool
install_signal_handlers(void)
{
  struct sigaction sa;
  int reti;

  // Reset the signal indicator.
  salrm = false;
  schld = false;

  // Initialise the handler settings.
  (void)memset(&sa, 0, sizeof(sa));
  sa.sa_handler = signal_handler;

  // Install a handler procedure for SIGALRM.
  reti = sigaction(SIGALRM, &sa, NULL);
	if (reti == -1) {
	  perror("unable to install handler for SIGALRM");
		return false;
	}

  // Install a handler procedure for SIGCHLD.
	reti = sigaction(SIGCHLD, &sa, NULL);
	if (reti == -1) {
		perror("unable to install handler for SIGCHLD");
		return false;
	}

	return true;
}
