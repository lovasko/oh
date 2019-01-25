# oh
This program allows for definition of _opening hours_ for a selected process.

## Case study
Let's suppose that a computationally intensive program `compute_pi` that
accepts an option to define the number of digits to which it computes the
mathematical constant _pi_. Such computation is resource-intensive and might be
best limited to certain hours during the day. Reasons for such limitation might
be twofold: making sure that the system isn't impacted during certain important
hours (e.g. when other users might be needing the shared computing resources),
or when the system itself might need time to cool down, so that overheating
will not occur.

To achieve this with `oh`, one would type something along the lines of the
following example:
```
$ oh -w -o 00:15 -c 06:30 -- compute_pi -d 1000000000
```

## Note on midnight
The common use case is for the process to run in the night hours, and thus the
opening and closing times have to deal with midnight reset of the clock. This
is handled by the `oh` program - it is therefore possible to select a closing
time that appears to be _before_ the opening time. This will result in the
process being allowed to run during the night hours.

## Note on signals
The `oh` tool uses UNIX signals to control the monitored process. It is
therefore advisable that the process in question does not block the delivery of
`SIGTSTP` and `SIGCONT` signals. It is of course possible for the process to
override the handling of the `SIGTSTP` signal to perform the necessary clean-up
actions, but the process is itself responsible for the pausing operation.

## Options and flags
The `oh` utility accepts the following options and flags to alter its
execution:

### `-c HH:MM`
The closing time of a process is when its execution is paused. The time format
expected is hours, followed by a colon character (`:`), followed by minutes.
The hours are limited to an inclusive range of `00` to `23`, whereas the
minutes are limited to an inclusive range of `00` to `59`. Whitespace
characters on either side of the string are not allowed. In case any of the
numbers requires only a single digit (e.g. `6:10`) it is allowed to omit the
leading zero.

### `-h`
The help message describing all options and flags can be displayed by using the
special `-h` flag.

### `-o HH:MM`
Analogously to the closing time option, the `-o` option allows for defining the
opening time of the process, thus resuming its execution. The same time format
syntax applies.

### `-s`
The POSIX suite of standards specify two main signals that are intended to
pause the execution of a process - `SIGTSTP` and `SIGSTOP`. The latter can't be
masked by the process, whilst the former can be handled by the process and
offers the opportunity of a clean pause (e.g. resetting any TTY settings, or
flushing buffers). The default choice of `oh` is to issue the softer `SIGTSTP`,
but it has been recognised that some instances require the harder `SIGSTOP`.
The `-s` flag allows to selected the stricter `SIGSTOP` signal.

### `-w`
Certain systems experience high loads during working days (or even just during
working hours) and are practically idle during the weekends. The `-w` flag is
to allow unlimited run during the weekends.

## Supported platforms
The program is written in strict ISO C99 language, using the POSIX.1 interface
to the kernel. Feature test macros used are: `_XOPEN_SOURCE`, and
`_POSIX_C_SOURCE`. All platforms conforming to such standards with the
appropriate C compiler are strongly expected to function as expected - any
reports of deviations are welcomed.

## License
The `nemo-core` project is licensed under the terms of the [2-cause BSD
license](LICENSE).

## Author
Daniel Lovasko <daniel.lovasko@gmail.com>
