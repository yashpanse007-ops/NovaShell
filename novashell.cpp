#include <iostream>
#include <string>
#include <vector>
#include <cstring>    // For strtok, strdup, and strcmp
#include <stdlib.h>   // For system() and exit()
#include <stdio.h>
#include <sys/types.h> // For pid_t
#include <sys/wait.h>  // For wait()
#include <unistd.h>    // For fork(), execvp(), and chdir()

using std::cout;
using std::endl;
using std::string;
using std::cin;

// --- PRODUCER-CONSUMER LOGIC ---
void run_pc_demo() {
    int fd[2]; 
    const int BUFFER_SIZE = 100;
    string custom_msg;
    
    // Get the data from the user first
    cout << "Enter message for Producer (max 99 chars): ";
    std::getline(cin, custom_msg);

    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork Failed");
    } 
    else if (pid == 0) { // Consumer (Child)
        close(fd[1]); 
        char buffer[BUFFER_SIZE];
        cout << "[Consumer] Waiting to read from pipe..." << endl;
        
        int bytes = read(fd[0], buffer, sizeof(buffer));
        if (bytes <= 0) {
            cout << "[Consumer] Underflow: No data found!" << endl;
        } else {
            cout << "[Consumer] Success! Received: " << buffer << endl;
        }
        close(fd[0]);
        exit(0);
    } 
    else { // Producer (Parent)
    close(fd[0]);
    
    // Check if the user actually entered data
    if (custom_msg.empty()) {
        cout << "[Producer] No input provided. Skipping write to simulate Underflow..." << endl;
        // We don't write anything to the pipe
    } 
    else if (custom_msg.length() + 1 > BUFFER_SIZE) {
        cout << "[Producer] OVERFLOW ERROR: Message too long!" << endl;
    } 
    else {
        cout << "[Producer] Writing data to pipe..." << endl;
        write(fd[1], custom_msg.c_str(), custom_msg.length() + 1);
    }
    
    close(fd[1]); // Closing the write end allows the Consumer to finish
    wait(NULL);
    }
}
int main() {
    system("clear"); 
    string input;

    cout << "============================================================" << endl;
    cout << "--- NovaShell: A POSIX-Compliant Command Line Interpreter ---" << endl;
    cout << "  Type 'help' for commands or 'exit' to leave. " << endl;
    cout << "============================================================" << endl << endl;

    while (true) {
        cout << "NovaShell > "; 
        
        if (!std::getline(cin, input) || input == "exit") {
            cout << "Exiting NovaShell... Goodbye!" << endl;
            break;
        }

        if (input.empty()) continue;

        // 2. TOKENIZER
        char* c_str = strdup(input.c_str());
        char* args[64];
        int i = 0;

        char* token = strtok(c_str, " ");
        while (token != NULL && i < 63) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; 

        if (args[0] == NULL) {
            free(c_str);
            continue;
        }

        // 3. BUILT-IN COMMANDS
        if (strcmp(args[0], "clear") == 0) {
            system("clear");
        }
        else if (strcmp(args[0], "pc_test") == 0){
            run_pc_demo(); //integrated producer consumer command        
        } 
        
        else if (strcmp(args[0], "help") == 0) {
            cout << "\n--- NovaShell Help Menu ---" << endl;
            cout << "  help       : Show this menu" << endl;
            cout << "  cd <path>  : Change directory" << endl;
            cout << "  pc_test    : Run Producer-Consumer Pipe demo" << endl;
            cout << "  clear      : Clear the shell screen" << endl;
            cout << "  exit       : Close NovaShell" << endl;
            cout << "  External   : Use any Linux command (ls, mkdir, date, etc.)\n" << endl;
        }
        else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                std::cerr << "NovaShell: expected argument to \"cd\"" << endl;
            } else {
                if (chdir(args[1]) != 0) {
                    perror("NovaShell");
                }
            }
        }
        // 4. EXTERNAL COMMANDS: Fork-Exec Pattern with Identification
        else {
            pid_t pid = fork();
            
            if (pid < 0) {
                perror("Novashell Fork error");
            }
            else if (pid == 0) {    // Child Process
                cout << "[CHILD] PID = " << getpid() << " | PARENT PID = " << getppid() << endl;
                if (execvp(args[0], args) == -1) {
                    cout << "NovaShell: command not found: " << args[0] << endl;
                }
                exit(EXIT_FAILURE); // Ensure child process terminates on error
            }
            else {                  // Parent Process
                cout << "[PARENT] PID = " << getpid() << " | Created CHILD PID = " << pid << endl;
                wait(NULL); // Synchronize with child
                cout << "[PARENT] CHILD process " << pid << " has terminated." << endl;
            }
        }
        
        free(c_str); 
    }
    return 0;
}
