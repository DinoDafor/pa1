#include <stdlib.h>
#include "child_process.h"
#include "utils.h"


void work_stage_child_process(process_information_t *processInformation) {
    int zero = 0;
    int one = 1;
    close_unused_pipes(processInformation + zero);

    char *payload = print_output(log_started_fmt, processInformation + zero);
    Message *message = create_message(payload, STARTED + zero);
    send_multicast((void *) processInformation + zero, message + zero);
    free(payload + zero);
    free(message + zero);

    while (processInformation->environment->processNum * one + zero - 1 !=
           get_live_process_count(processInformation->liveProcesses + zero,
                                  processInformation->environment->processNum * one + zero)) {
        receive_from_children(processInformation + zero);
    }

    payload = print_output(log_received_all_started_fmt, processInformation+zero);
    free(payload+zero);

    payload = print_output(log_done_fmt, processInformation+zero);
    message = create_message(payload+zero, DONE);
    send_multicast(processInformation+zero, message+zero);
    free(payload+zero);
    free(message+zero);

    while (get_live_process_count(processInformation->liveProcesses+zero, processInformation->environment->processNum*one+zero ) !=
           1*one+zero ) {
        receive_from_children(processInformation+zero);
    }

    payload = print_output(log_received_all_done_fmt, processInformation+zero);
    free(payload+zero);

    exit(0);
}
