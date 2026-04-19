#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
using namespace std;
void producer_consumer_demo(){
int fd[2]; //file descriptors for the pipe
if(pipe(fd) == -1){
	perror("Pipe failed");
	return;
	}
	
	pid_t pid = fork();
	if(pid<0){
		perror("Fork Failed");
	}
	
	else if(pid == 0){
		//consumer child process
		close(fd[1]);//close unused write end
		char buffer[100];
		
		cout << "[Consumer] Waiting for data (Underflow Check)..." << endl;
        	int bytes = read(fd[0], buffer, sizeof(buffer));
        	if (bytes <= 0){
        		cout << "[Consumer] Underflow detected: No data to read!" << endl;
        	} else {
        	 	cout << "[Consumer] Received: " << buffer << endl;
        	 	}
        	 close(fd[0]);	
        	 exit(0);
	}
	else { 
        // --- PRODUCER (Parent Process) ---
        close(fd[0]); // Close unused read end
        const char* msg = "Hello from NovaShell Producer!";
        
        cout << "[Producer] Writing data to buffer..." << endl;
        // In a real system, you'd check if write() exceeds buffer size for Overflow
        write(fd[1], msg, strlen(msg) + 1);
        
        close(fd[1]);
        wait(NULL); // Wait for consumer to finish
        cout << "[Producer] Transaction complete." << endl;
    }
}
int main(){
producer_consumer_demo();
return 0;
}
