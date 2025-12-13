
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "executor.h"

void setup_redirections(tree *node) {
    if (node->infile) {
        int fd = open(node->infile, O_RDONLY);
        if (fd < 0) {
            perror("open");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    
    if (node->outfile) {
        int flags = O_WRONLY | O_CREAT;
        if (node->append) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }
        
        int fd = open(node->outfile, flags, 0644);
        if (fd < 0) {
            perror("open");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

int execute_single_command(tree *node) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        setup_redirections(node);
        execvp(node->argv[0], node->argv);
        perror("execvp");
        exit(1);
    }
    
    int status;
    
    if (node->backgrnd) {
        printf("[%d] running in background\n", pid);
        return 0;
    } else {
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

int execute_pipeline(tree *node) {
    int input_fd = -1;
    pid_t last_pid = -1;
    tree *current = node;
    
    while (current) {
        int pipefd[2];
        
        if (current->pipe) {
            if (pipe(pipefd) < 0) {
                perror("pipe");
                return -1;
            }
        }
        
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork");
            return -1;
        }
        
        if (pid == 0) {
            if (input_fd != -1) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            
            if (current->pipe) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            
            setup_redirections(current);
            execvp(current->argv[0], current->argv);
            perror("execvp");
            exit(1);
        }
        
        if (input_fd != -1) {
            close(input_fd);
        }
        
        if (current->pipe) {
            close(pipefd[1]);
            input_fd = pipefd[0];
        }
        
        last_pid = pid;
        current = current->pipe;
    }
    
    int status;
    if (last_pid != -1) {
        waitpid(last_pid, &status, 0);
        return WEXITSTATUS(status);
    }
    
    return 0;
}

int execute_tree(tree *node) {
    if (!node) return 0;
    
    int status = 0;
    
    if (node->pipe) {
        status = execute_pipeline(node);
    } else {
        status = execute_single_command(node);
    }
    
    if (node->next) {
        if (node->type == AND) {
            if (status == 0) {
                execute_tree(node->next);
            }
        } else if (node->type == OR) {
            if (status != 0) {
                execute_tree(node->next);
            }
        } else {
            execute_tree(node->next);
        }
    }
    
    return status;
}
