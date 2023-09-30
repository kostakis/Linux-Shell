#include "PipeManagement.hpp"

int hasPipe(const std::vector<char *> &tokens) {
  int errorValue{-1};
  int index{0};
  for (const auto &token : tokens) {
    if (token == nullptr)
      return errorValue;
    if (std::strcmp(token, "|") == 0) {
      std::cout << "Found pipe: " << index << std::endl;
      return index;
    }
    index++;
  }
  return errorValue;
}

bool handlePipe(std::vector<char *> &tokens) {
  int pipePos = hasPipe(tokens);

  if (pipePos != -1) {
    // split tokens into two parts
    std::vector<char *> tokens1(tokens.begin(), tokens.begin() + pipePos);
    std::vector<char *> tokens2(tokens.begin() + pipePos + 1, tokens.end());

    // create pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    // fork first child process to execute command 1
    pid_t pid1 = fork();
    if (pid1 == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
      // child process
      close(pipefd[0]); // close unused read end of pipe
      dup2(pipefd[1],
           STDOUT_FILENO); // redirect stdout to write end of pipe
      close(pipefd[1]);    // close write end of pipe

      // execute command 1
      tokens1.push_back(nullptr);
      execvp(tokens1[0], tokens1.data());
      exit(EXIT_FAILURE);
    }

    // fork second child process to execute command 2
    pid_t pid2 = fork();
    if (pid2 == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
      // child process
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO); // redirect stdin to read end of pipe
      close(pipefd[0]);

      // execute command 2
      tokens2.push_back(nullptr);
      execvp(tokens2[0], tokens2.data());

      exit(EXIT_FAILURE);
    }

    // parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return true;
  } else {
    return false;
  }
}