#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "utils.h"
#include "common.h"

void set_default_state_live_processes(const int local_id, int* pProcesses, int process_count) {

    pProcesses[0] = 1;

    for (int process_id = 1; process_id < process_count; process_id ++) {
        if (local_id != process_id) pProcesses[process_id] = 0;
        else pProcesses[process_id] = 1;
    }

}


environment_t* init_environment(int process_num) {

    environment_t* env = malloc(sizeof(environment_t));

    env->processNum = process_num;

    pipe_t* table = calloc(process_num * process_num, sizeof(pipe_t));
    pipe_t** row = malloc(process_num * sizeof(pipe_t));

    for (int row_index = 0 ; row_index < process_num; row_index ++) {
        row[row_index] = table + process_num * row_index;
        for (int column_index = 0; column_index < process_num; column_index ++) {
            if (row_index != column_index) {
                pipe(row[row_index][column_index].fd);
            }
        }
    }

    env->pPipe = row;

    env->fdLog = open(events_log, O_APPEND | O_CREAT | O_RDWR);

    return env;
}


process_information_t* create_process(environment_t* environment, int local_id) {
    process_information_t* processInformation = malloc(sizeof(process_information_t));

    processInformation->environment = environment;
    processInformation->local_id = local_id;
    int* neighbor = calloc(environment->processNum, sizeof(int));
    set_default_state_live_processes(processInformation->local_id, neighbor, processInformation->environment->processNum);
    processInformation->liveProcesses = neighbor;

    return processInformation;
}

void close_unused_pipes(process_information_t* processInformation) {

    environment_t* environment = processInformation->environment;

    for (int row = 0; row < environment->processNum; row ++) {
        for (int column = 0; column < environment->processNum; column ++) {
            if (row != column && row != processInformation->local_id && column != processInformation->local_id) {
                close(environment->pPipe[row][column].fd[1]);
                close(environment->pPipe[row][column].fd[0]);
            } else if (row == processInformation->local_id) {
                close(environment->pPipe[row][column].fd[0]);
            } else if (column == processInformation->local_id) {
                close(environment->pPipe[row][column].fd[1]);
            }
        }
    }

}

Message* create_message(const char* payload, MessageType type) {
    MessageHeader header = {
            .s_local_time = 0,
            .s_payload_len = payload != NULL ? strlen(payload) : 0,
            .s_magic = MESSAGE_MAGIC,
            .s_type = type
    };

    Message* message = (Message*) malloc(MAX_MESSAGE_LEN);
    message->s_header = header;

    if (payload != NULL) {
        strncpy(message->s_payload, payload, strlen(payload));
    }

    return message;
}

char* print_output(const char* message_format, const process_information_t* processInformation) {
    char* payload = malloc(strlen(message_format));
    sprintf(payload, message_format, processInformation->local_id, getpid(), getppid());
    write(processInformation->environment->fdLog, payload, strlen(payload));
    printf("%s", payload);
    return payload;

}

int get_live_process_count(int* processes, int process_count) {

    int working_processes = 0;

    for (int i = 1; i < process_count; i ++) {
        if (processes[i] == 1) working_processes ++;
    }

    return working_processes;
}


void receive_from_children(process_information_t* processInformation) {

    Message message;

    for (int local_id = 1; local_id < processInformation->environment->processNum; local_id ++) {
        if (receive(processInformation, local_id, &message) == 0) {
            if (message.s_header.s_type == STARTED) {
                processInformation->liveProcesses[local_id] = 1;
            } else if (message.s_header.s_type == DONE) {
                processInformation->liveProcesses[local_id] = 0;
            }
        }
    }
}

