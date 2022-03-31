#ifndef PA1_CHILD_PROCESS_H
#define PA1_CHILD_PROCESS_H

#include "general_structures.h"

void work_stage_child_process(process_information_t* processInformation);
struct pipe {
    int fd[2];
    boolean isBusy;

} typedef pip_t;
#endif //PA1_CHILD_PROCESS_H
