#include "args.h"
#include <iostream>
#include <string>
#include <unistd.h>

// Modify something to create a new commit

namespace utils
{
Args::Args (int argc, char **argv)
{
  char opt;
  while ((opt = getopt (argc, argv, "f:t")) > 0)
    {
      switch (opt)
        {
        case 'f':
          filename = std::string (optarg);
          break;
        case 't':
          tui = true;
          break;
        default:
          {
            std::cerr << "Invalid option!";
            std::exit (1);
          }
        }
    }
}
} // namespace utils

// Branch test
