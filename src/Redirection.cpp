#include "Redirection.hpp"

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <cstring>
#include <fcntl.h>
#include <iostream>

void handleOutputRedirection(std::vector<char *> &lineTokens) {
  int outputRedirection{-1};
  bool doubleOutputRedirection{false};

  for (int i = 0; i < lineTokens.size() - 1; i++) {
    if (lineTokens[i] == nullptr)
      break;

    if (std::strcmp(lineTokens[i], ">") == 0) {
      // //std::cout << "got output redirection >" << std::endl;

      if (lineTokens[i + 1] == nullptr) {
        // //std::cout << "No input given for redirection wont proccess"
        // << std::endl;
        return;
      }
      outputRedirection = i;
    } else if (std::strcmp(lineTokens[i], ">>") == 0) {
      doubleOutputRedirection = true;
      // std::cout << "got output redirection >" << std::endl;

      if (lineTokens[i + 1] == nullptr) {
        // std::cout << "No input given for redirection wont proccess" <<
        // std::endl;
        return;
      }
      outputRedirection = i;
    }
  }

  while (outputRedirection != -1) {
    int fileStream{-1};

    auto flags = O_WRONLY | O_CREAT;

    if (doubleOutputRedirection) {
      flags += O_APPEND;
    } else {
      flags += O_TRUNC;
    }

    // Must decide if we append or create
    fileStream = open(lineTokens[outputRedirection + 1], flags, 0666);

    if (fileStream == -1) {
      // std::cout << "Can not open file: " << lineTokens[outputRedirection
      // + 1] << ". Aborting" << std::endl; std::cout << "Error string: "
      // << strerror(errno) << std::endl;
      return;
    }

    // Redirect standard input to the file
    if (dup2(fileStream, STDOUT_FILENO) == -1) {
      // std::cout << "Can not create copy of file descriptor. Aborting";
      // std::cout << "Error string: " << strerror(errno) << std::endl;
    }

    // Is is really a problem tough?
    if (close(fileStream) == -1) {
      // std::cout << "Can not close file descriptor\n";
      // std::cout << "Error string: " << strerror(errno) << std::endl;
    }

    lineTokens.erase(lineTokens.begin() + outputRedirection, lineTokens.begin() + outputRedirection + 2);

    // Check for another redirection
    outputRedirection = -1;
    doubleOutputRedirection = false;
    for (int i = 0; i < lineTokens.size() - 1; i++) {
      if (lineTokens[i] == nullptr)
        break;

      if (std::strcmp(lineTokens[i], ">") == 0) { // Classic C comparsion
        outputRedirection = i;
        break;
      } else if (std::strcmp(lineTokens[i], ">>") == 0) {
        outputRedirection = i;
        doubleOutputRedirection = true;
        break;
      }
    }
  }
}

void handleInputRedirection(std::vector<char *> &lineTokens) {
  int inputRedirection{-1};
  bool doubleInputRedirection{false};

  for (int i = 0; i < lineTokens.size() - 1; i++) {
    if (lineTokens[i] == nullptr)
      break;

    if (std::strcmp(lineTokens[i], "<") == 0) {
      inputRedirection = i;
    } else if (std::strcmp(lineTokens[i], "<<") == 0) {
      inputRedirection = i;
      doubleInputRedirection = true;
    }
  }

  while (inputRedirection != -1) {

    auto flags = O_RDONLY;

    int fileStream = open(lineTokens[inputRedirection + 1], flags);
    if (fileStream == -1) {
      // std::cout << "Can not open file.Aborting\n";
      return;
    }

    // Redirect standard input to the file
    if (dup2(fileStream, STDIN_FILENO) == -1) {
      // std::cout << "Can not create copy of file descriptor. Aborting";
      // std::cout << "Error string: " << strerror(errno) << std::endl;
    }

    // Is is really a problem tough?
    if (close(fileStream) == -1) {
      // std::cout << "Can not close file descriptor\n";
      // std::cout << "Error string: " << strerror(errno) << std::endl;
    }

    lineTokens.erase(lineTokens.begin() + inputRedirection, lineTokens.begin() + inputRedirection + 2);

    inputRedirection = -1;
    doubleInputRedirection = false;
    // Check for another redirection
    for (int i = 0; i < lineTokens.size() - 1; i++) {
      if (lineTokens[i] == nullptr)
        break;

      if (std::strcmp(lineTokens[i], "<") == 0) {
        inputRedirection = i;
        break;
      } else if (std::strcmp(lineTokens[i], "<<") == 0) {
        inputRedirection = i;
        doubleInputRedirection = true;
        break;
      }
    }
  }
}
