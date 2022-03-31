#include <stdlib.h>
#include "child_process.h"
#include "utils.h"


void work_stage_child_process(process_information_t* processInformation) {

    close_unused_pipes(processInformation);

    char* payload = print_output(log_started_fmt, processInformation);
    Message* message = create_message(payload, STARTED);
    send_multicast((void*) processInformation, message);
    free(payload);
    free(message);

    while (processInformation->environment->processNum - 1 !=
        get_live_process_count(processInformation->liveProcesses, processInformation->environment->processNum)) {
        receive_from_children(processInformation);
    }

    payload = print_output(log_received_all_started_fmt, processInformation);
    free(payload);

    payload = print_output(log_done_fmt, processInformation);
    message = create_message(payload, DONE);
    send_multicast(processInformation, message);
    free(payload);
    free(message);

    while (get_live_process_count(processInformation->liveProcesses, processInformation->environment->processNum) != 1) {
        receive_from_children(processInformation);
    }

    payload = print_output(log_received_all_done_fmt, processInformation);
    free(payload);

    exit(0);
}
