#include "HelperFunctions.hpp"
#include "PipeManagement.hpp"
#include "Redirection.hpp"

#ifdef __linux__
#include <glob.h> // For wildcards
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#else
#error "OS not supported"
#endif

#include <cstring>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>

static pid_t pid = -1; // Very bad but it was it is.

void signal_handler(int signum) {
  // std::cout << "Parent process received signal " << signum << std::endl;
  if (pid != -1) {
    // std::cout << "Will sent: " << signum << " to pid: " << pid <<
    // std::endl;
    kill(pid, signum);
  }
}

void printTokens(const std::vector<char *> &lineTokens) {
  // std::cout << "Printing Tokens: " << std::endl;
  for (const char *c : lineTokens) {
    if (c == nullptr)
      break;
    // std::cout << c << " ";
  }
  // std::cout << std::endl << std::endl;
}

void makeTokens(std::vector<char *> &lineTokens, std::string &line) {
  char *token = strtok(&line[0], " "); // Starting address of the string

  while (token != nullptr) {
    //// std::cout << "Got token: " << token << std::endl;
    lineTokens.push_back(token);
    token = strtok(nullptr, " ");
  }
  lineTokens.push_back(nullptr); // Mark the end of the line
}

void execute(std::vector<char *> &tokens) {
  if (tokens.size() == 1)
    return; // just the nullptr nonthing to execute

  if (handlePipe(tokens))
    return;

  bool isBackground{false}; // Will be used if we have & at the end
  char **argv = nullptr;    // The arguments passed to execvp

  // Check is command should run in the background
  auto lastToken = tokens[tokens.size() - 2];
  if (std::strcmp(lastToken, "&") == 0) {
    // Remove the & from the tokens
    tokens[tokens.size() - 2] = nullptr;
    isBackground = true;
  }

  // Fork a child process
  pid = fork();
  if (pid == -1) {
    // Fork failed
    // std::cout << "Error: fork failed. Will not proccess command\n";
    return;
  } else if (pid == 0) {
    // Child process
    // std::cout << "Child process id: " << getpid() << std::endl;

    handleInputRedirection(tokens);
    handleOutputRedirection(tokens);

    // Check for wild cards
    std::vector<std::string> expanded_tokens;
    for (const auto &token : tokens) {
      if (token == nullptr)
        break;
      std::string tmp(token);
      auto expanded = expandWildcards(tmp);

      if (!expanded.empty()) {
        expanded_tokens.insert(expanded_tokens.end(), expanded.begin(), expanded.end());
      } else {
        expanded_tokens.push_back(token);
      }
    }

    // Convert our tokens to char** for the execvp function
    argv = new char *[expanded_tokens.size() + 1];
    for (int i = 0; i < expanded_tokens.size(); i++) {
      argv[i] = const_cast<char *>(expanded_tokens[i].c_str());
    }
    argv[expanded_tokens.size()] = nullptr;

    // Execute the command
    execvp(argv[0], argv);

    std::cout << "Error command: " << argv[0] << " not found.\n";
    exit(-1); // This exit for our child process.
  } else {
    // Parent process
    // std::cout << "Parent id: " << getpid() << std::endl;

    // signal(SIGTSTP, signal_handler); // CTRL + Z This doesnt work...
    //  Well it works but they are both at the same process group and the
    //  both close

    signal(SIGINT, signal_handler); // CTRL + C

    if (isBackground) {
      //// std::cout << "Parent process wont wait since child should run in
      /// the background\n";
      if (argv != nullptr)
        delete[] argv;
      pid = -1;
      signal(SIGINT, SIG_DFL);
      return;
    }

    // Wait for child process to finish
    int status;
    if (waitpid(pid, &status, 0) == -1) {
      // std::cout << "Error: waitpid failed.Aborting terminal\n";
      exit(EXIT_FAILURE);
    } else if (WIFEXITED(status)) {
      // Child process exited normally
    } else {
      // Child process terminated abnormally
      // std::cout << "Error: command terminated abnormally\n";
    }

    // Restore descriptors
    dup2(STDIN_FILENO, 0);
    dup2(STDOUT_FILENO, 1);
  }

  if (argv != nullptr)
    delete[] argv;
  signal(SIGINT, SIG_DFL); // Reset the app default signals
  pid = -1;
  std::cout << "\n";
}

std::vector<std::string> expandWildcards(const std::string &token) {
  std::vector<std::string> result;

  // Check if token contains wildcard characters
  if (token.find_first_of("*?") == std::string::npos) {
    // No wildcards, return token as it is
    result.push_back(token);
    return result;
  }

  glob_t glob_result;
  memset(&glob_result, 0, sizeof(glob_t));

  int ret = glob(token.c_str(), GLOB_TILDE, NULL, &glob_result);
  if (ret != 0) { // Error
    globfree(&glob_result);
    // std::cout << "Glob error\n";
    return result; // Return empty result
  }

  // Add all the result to the vector
  for (size_t i = 0; i < glob_result.gl_pathc; i++) {
    result.push_back(std::string(glob_result.gl_pathv[i]));
  }

  // Free glob results memory
  globfree(&glob_result);

  return result;
}

void parseEnviromentalVariables(std::string &line) {
  // Find and replace ${} variables
  size_t pos = line.find("${");
  while (pos != std::string::npos) {
    size_t endpos = line.find("}", pos);
    if (endpos != std::string::npos) {
      std::string varname = line.substr(pos + 2, endpos - pos - 2);
      char *value = getenv(varname.c_str());
      if (value) {
        line.replace(pos, endpos - pos + 1, value);
        pos = line.find("${");
      } else {
        // std::cout << "Enviromental variable: " << varname << " does
        // not exist\n";
        pos = line.find("${", endpos + 1);
      }
    } else {
      // std::cout << "No close bracket }\n";
      break;
    }
  }
}