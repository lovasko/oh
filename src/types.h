#ifndef OH_TYPES_H
#define OH_TYPES_H

#include <stdbool.h>
#include <stdint.h>


/// Configuration.
struct config {
	char*    cf_path;    ///< Path to program to execute.
	char**   cf_argv;    ///< Argument vector for the monitored process.
  uint16_t cf_omsm;    ///< Opening time - minutes since midnight.
  uint16_t cf_cmsm;    ///< Closing time - minutes since midnight.
  bool     cf_wknd;    ///< Allow for uninterrupted run during weekends.
  bool     cf_stop;    ///< Use SIGSTOP instead of SIGTSTP.
  uint8_t  cf_upad[2]; ///< Padding (unused).
};

/// Process state.
enum state {
  RUNNING, ///< The process is (supposed to be) running.
  STOPPED  ///< The process is (supposed to be) stopped.
};

#endif
