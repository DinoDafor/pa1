#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "general_structures.h"
#include "utils.h"
#include "child_process.h"


void work_stage_main_process(process_information_t* processInformation) {

    close_unused_pipes(processInformation);
    while (waitpid(-1, NULL, 0) != -1);

}

int main(int argc, char* argv[]) {
    int process_count;
    sscanf(argv[2], "%d", &process_count);

    environment_t* environment = init_environment(process_count + 1);

    __pid_t process;
    for (int process_id = 1; process_id < environment->processNum; process_id ++) {
        if ((process = fork()) == 0) {
            process_information_t* processInformation = create_process(environment, process_id);
            work_stage_child_process(processInformation);
        }
    }

    process_information_t* processInformation = create_process(environment, 0);

    work_stage_main_process(processInformation);

    close(processInformation->environment->fdLog);


    return 0;
}
