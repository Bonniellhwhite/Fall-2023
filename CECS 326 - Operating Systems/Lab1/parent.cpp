#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
        // Confirm the correct number of arguments
        // If not, error
        if (argc < 2) {
                std::cerr << "Usage: " << argv[0] << " <num_children> <child_names>";
                return 1;
        }

        // Number of children and a vector of child processes
        int num_children = argc - 1;
        std::vector<pid_t> children;

        // Print the number of children
        std::cout << "I have " << num_children << " children.\n";

        // For each required child process
        for (int i = 1; i < num_children + 1; i++) {
                // fork() creates a new process by duplicating the calling process.  The new process is referred to as the child process.  The calling process is referred to as the parent process.
                //
                // The child process and the parent process run in separate memory spaces.  At the time of fork() both memory spaces have the same content.  Memory writes, file mappings (mmap(2)), and unmappings (munmap(2)) performed by one of the processes do not affect the other.
                pid_t child = fork();

                // If fork returns -1, fork has failed
                if (child < 0) {
                        std::cerr << "Fork failed.\n";
                        return 1;
                }
                // If the fork was successful
                // Check if we are in the child processi
                //
                // execv(), execvp(), execvpe()
                // These functions duplicate the actions of the shell in searching for an executable file if the specified filename does not contain a slash (/) character.  The file is sought in the colon-separated list of directory pathnames specified in the PATH environment variable.  If this variable isn't defined, the path list defaults to a list that includes the directories returned by confstr(_CS_PATH) (which typically returns the value "/bin:/usr/bin") and possibly also the current working directory
                // The char *const argv[] argument is an array of pointers to null-terminated strings that represent the argument list available to the new program.  The first argument, by convention, should point to the filename associated with the file being executed.  The array of pointers must be terminated by a null pointer.
                else if (child == 0) {
                        const char* child_args[] = {"./child", std::to_string(i).c_str(), argv[i], nullptr};
                        pid_t child_process = execvp("./child", const_cast<char* const*>(child_args));
                        // if exec() fails, error
                        if (child_process == -1)
                                std::cerr << "Exec failed.\n";
                                return 1;
                }
                // If the fork was successful and we are not in the child process
                // Add to the vector of processes
                else {
                        children.push_back(child);
                }
        }
        // Wait for all child processes to finish successfully and return 0
        //
        // The wait() function shall suspend execution of the calling thread until status information for one of the terminated child processes of the calling process is available, or until delivery of a signal whose action is either to execute a signal-catching function or to terminate the process. If more than one thread is suspended in wait() or waitpid() awaiting termination of the same process, exactly one thread shall return the process status at the time of the target process termination. If status information is available prior to the call to wait(), return shall be immediate.
        //
        // The waitpid() function shall be equivalent to wait() if the pid argument is (pid_t)-1 and the options argument is 0. Otherwise, its behavior shall be modified by the values of the pid and options arguments.
        //      If pid is equal to (pid_t)-1, status is requested for any child process. In this respect, waitpid() is then equivalent to wait().
        //      If pid is greater than 0, it specifies the process ID of a single child process for which status is requested.
        //      If pid is 0, status is requested for any child process whose process group ID is equal to that of the calling process.
        //      If pid is less than (pid_t)-1, status is requested for any child process whose process group ID is equal to the absolute value of pid.
        for (auto& kid : children) {
                int status;
                waitpid(kid, &status, 0);
        }

        // Print to confirm all processes are completed
        std::cout << "All child processes terminated. Parent exits." << std::endl;

        return 0;
}