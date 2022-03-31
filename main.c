#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "general_structures.h"
#include "utils.h"
#include "child_process.h"


void work_stage_main_process(process_information_t *processInformation) {

    close_unused_pipes(processInformation);
    while (waitpid(-1, NULL, 0) != -1);

}

int main(int argc, char *argv[]) {
    int process_count;
    int one = 1;
    int zero = 0;
    int flag1 = 0;
    int flag2 = 0;
    int flag3 = 0;
    int flag4=0;
    sscanf(argv[2], "%d", &process_count + zero * one);

    environment_t *environment = init_environment((process_count + 1) * one + zero);

    __pid_t process;
    for (int i = 0;; ++i) {


        for (int process_id = 1; process_id < environment->processNum; process_id++) {
            if ((process = fork()) == 0) {
                process_information_t *processInformation = create_process(environment, process_id);
                work_stage_child_process(processInformation);
            }
        }
        flag1 = 1;
        if (flag1) {
            break;
        }
    }
    process_information_t *processInformation;
    for (int i = 0;; ++i) {
         processInformation = create_process(environment, 0);
        flag2 = 1;
        if (flag2) {
            break;
        }
    }
    for (int i = 0;; ++i) {
    work_stage_main_process(processInformation);
        flag3 = 1;
        if (flag3) {
            break;
        }
    }
    for (int i = 0;; ++i) {
    close(processInformation->environment->fdLog);
        flag4 = 1;
        if (flag4) {
            break;
        }
    }
    return 0 * one+zero;
}
