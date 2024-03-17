/*********************************************************************************
 * Copyright(C), mem.ac
 * FileName:     ConsolePauser2.cpp
 * Author:       memset0
 * Version:      2.1.0
 * Date:         2024-03-17
 * License:      MIT
 * Description:  A forked version of Dev C++'s ConsolePauser
                 with cross-platform support.
 **********************************************************************************/

#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

using std::string;

#define MAX_COMMAND_LENGTH 32768
#define MAX_ERROR_LENGTH 2048

time_t GetClockTick() { return std::chrono::high_resolution_clock::now().time_since_epoch().count(); }
time_t GetClockFrequency() { return std::chrono::high_resolution_clock::period::den / std::chrono::high_resolution_clock::period::num; }

void PauseExit(int exitcode) {
  system("pause");
  exit(exitcode);
}

void _printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("\033[2m");
  vprintf(format, args);
  printf("\033[0m");
  va_end(args);
}

string GetCommand(int argc, char **argv) {
  string result;
  for (int i = 1; i < argc; i++) {
    result += string(argv[i]);

    if (i != (argc - 1)) {
      result += string(" ");
    }
  }

  if (result.length() > MAX_COMMAND_LENGTH) {
    _printf("\n--------------------------------");
    _printf("\nError: Length of command line string is over %d characters\n", MAX_COMMAND_LENGTH);
    PauseExit(EXIT_FAILURE);
  }

  return result;
}

unsigned long ExecuteCommand(string &command) { return std::system(command.c_str()); }

int main(int argc, char **argv) {
  // First make sure we aren't going to read nonexistent arrays
  if (argc < 2) {
    _printf("\n--------------------------------");
    _printf("\nUsage: ConsolePauser.exe <filename> <parameters>\n\n");
    exit(EXIT_SUCCESS);
  }

#ifdef _WIN32
  // Make us look like the paused program (Windows only)
  SetConsoleTitle(argv[1]);
#endif

  // Then build the to-run application command
  string command = GetCommand(argc, argv);

  // Save starting timestamp
  size_t starttime = GetClockTick();

  // Then execute said command
  unsigned long returnvalue = ExecuteCommand(command);

  // Get ending timestamp
  size_t endtime = GetClockTick();
  double seconds = (endtime - starttime) / (double)GetClockFrequency();

  // Print return value of executed program.
  _printf("\n--------------------------------");
  _printf("\nProcess exited after %.4g seconds with return value %lu\n", seconds, returnvalue);

  // Exit without pause! [MODIFIED]
  printf("\n");
  exit(EXIT_SUCCESS);
}
