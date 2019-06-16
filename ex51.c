// name: Noa Dagan
// id: 311302137

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>

const char MSG_ERROR[150] = "Error in system call\n";
const int LEN_MSG = sizeof(MSG_ERROR);

// The function read char
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

// The function listem to input char and write if it is specific char
int listenAndWrite() {
    pid_t pid;
    int status;
    int fd[2];
    // pipe
    if (pipe(fd) == -1) {
        write(2, MSG_ERROR, LEN_MSG);
        return -1;
    }
    pid = fork();
    // error
    if (pid < 0) {
        write(2, MSG_ERROR, LEN_MSG);
        return -1;
    }
    // is a child
    else if (pid == 0) {
        close(0);
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        char *arg[2];
        // send to execv torun the file and a.out to draw.out
        arg[0] = "./draw.out";
        arg[1] = NULL;
        int excveCode = execv("draw.out", arg);
        if (excveCode == -1) {
            write(2, MSG_ERROR, LEN_MSG);
            return -1;
        }
        // is a [arent
    } else {
        char c;
        // check if input cher is 'a','s','d',w'' and send kill if yes
        close(fd[0]);
        do {

            dup2(fd[1], 1);
            c = getch();
            while ((c != 'a') && (c != 's') && (c != 'd') &&
                   (c != 'w') && (c != 'q')) {
                c = getch();
            }
            close(fd[1]);
            char *result = &c;
            // if its the specific char write to ex52
            write(1, result, sizeof(char));
            kill(pid, SIGUSR2);
        } while (c != 'q');
        // wait for child
        while (waitpid(pid, &status, 0) < 0);
        return 0;
    }
}

// main function
int main() {
    listenAndWrite();
    return 0;

}
