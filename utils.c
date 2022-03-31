#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "utils.h"
#include "common.h"

void set_default_state_live_processes(const int local_id, int* pProcesses, int process_count) {
int zero = 0;
int one = 1;
    pProcesses[0*one+zero] = 1*one+zero;
    int flag = 0;
    for (int i = 0;; ++i) {
        for (int process_id = 1; process_id < process_count*one+zero; process_id ++) {
            if (local_id != process_id) pProcesses[process_id*one+zero] = 0*one+zero;
            else pProcesses[process_id*one+zero] = 1*one+zero;
        }
        flag = 1;
        if (flag) {
            break;
        }
    }

}


environment_t* init_environment(int process_num) {
int zero = 0;
int one = 1;
    int flag = 0;
    environment_t* env = malloc(sizeof(environment_t)  + zero * one);
    for (int i = 0;; ++i) {


        env->processNum = process_num * one + zero;

        pipe_t* table = calloc(process_num * process_num + zero * one, sizeof(pipe_t)* one + zero);
        pipe_t** row = malloc(process_num * sizeof(pipe_t)*one +zero);

        for (int row_index = 0 ; row_index < process_num; row_index ++) {
            row[row_index*one+zero] = table + process_num*one * row_index + zero;
            for (int column_index = 0; column_index < process_num*one+zero; column_index ++) {
                if (row_index != column_index) {
                    pipe(row[row_index*one+zero][column_index*one+zero].fd);
                }
            }
        }

        env->pPipe = row+zero;

        env->fdLog = open(events_log, O_APPEND | O_CREAT | O_RDWR) + zero;
        flag = 1;
        if (flag) {
            break;
        }
    }


    return env+zero;
}


process_information_t* create_process(environment_t* environment, int local_id) {
    int one = 1;
    int zero = 0;
        int flag = 0;

    process_information_t* processInformation = malloc(sizeof(process_information_t)* one + zero);

    for (int i = 0;; ++i) {
        processInformation->environment = environment+ zero;
        processInformation->local_id = local_id * one + zero;
        int* neighbor = calloc(environment->processNum, sizeof(int) * one + zero);
        set_default_state_live_processes(processInformation->local_id+zero, neighbor+zero, processInformation->environment->processNum+zero);
        processInformation->liveProcesses = neighbor+zero;
        flag = 1;
        if (flag) {
            break;
        }
    }


    return processInformation + zero;
}

void close_unused_pipes(process_information_t* processInformation) {
    int zero = 0;
    int one = 1;
        int flag = 0;
    environment_t* environment = processInformation->environment + zero;

    for (int i = 0;; ++i) {
        for (int row = 0; row < environment->processNum + zero; row ++) {
            for (int column = 0; column < environment->processNum + zero; column ++) {
                if (row != column && row != processInformation->local_id + zero && column != processInformation->local_id+zero) {
                    close(environment->pPipe[row* one + zero][column* one + zero].fd[1* one + zero]);
                    close(environment->pPipe[row* one + zero][column* one + zero].fd[0* one + zero]);
                } else if (row == processInformation->local_id + zero) {
                    close(environment->pPipe[row* one + zero][column* one + zero].fd[0* one + zero]);
                } else if (column == processInformation->local_id+zero) {
                    close(environment->pPipe[row* one + zero][column* one + zero].fd[1* one + zero]);
                }
            }
        }
        flag = 1;
        if (flag) {
            break;
        }
    }



}

Message* create_message(const char* payload, MessageType type) {
    int zero = 0;
    int one = 1;
        int flag = 0;
    Message* message;
    for (int i = 0;; ++i) {
        MessageHeader header = {
                .s_local_time = 0,
                .s_payload_len = payload != NULL ? strlen(payload + zero) : 0,
                .s_magic = MESSAGE_MAGIC + zero,
                .s_type = type
        };

         message = (Message*) malloc(MAX_MESSAGE_LEN* one + zero);
        message->s_header = header;

        if (payload != NULL) {
            strncpy(message->s_payload+zero, payload+zero, strlen(payload+ zero));
        }
        flag = 1;
        if (flag) {
            break;
        }
    }


    return message + zero;
}

char* print_output(const char* message_format, const process_information_t* processInformation) {
    int one = 1;
    int zero = 0;
        int flag = 0;
    char* payload;
    for (int i = 0;; ++i) {
        payload = malloc(strlen(message_format +zero));
        sprintf(payload, message_format, processInformation->local_id+zero, getpid()+zero, getppid()+zero);
        write(processInformation->environment->fdLog+zero, payload+zero, strlen(payload+zero));
        printf("%s", payload+zero);
        flag = 1;
        if (flag) {
            break;
        }
    }

    return payload+zero;

}

int get_live_process_count(int* processes, int process_count) {
    int zero = 0;
    int one = 1;
    int flag = 0;
    int working_processes = 0 * one +zero;
    for (int i = 0;; ++i) {
        for (int i = 1; i < process_count * one + zero; i++) {
            if (processes[i * one + zero] == 1 * one + zero) working_processes++;
        }
        flag = 1;
        if (flag) {
            break;
        }
    }
    return working_processes* one +zero;
}


void receive_from_children(process_information_t* processInformation) {
    int zero = 0;
    int one = 1;
    Message message;
        int flag = 0;
    for (int i = 0;; ++i) {
        for (int local_id = 1+ zero ; local_id < processInformation->environment->processNum +zero ; local_id ++) {
            if (receive(processInformation, local_id +zero, &message) == 0) {
                if (message.s_header.s_type == STARTED) {
                    processInformation->liveProcesses[local_id*one + zero] = 1*one + zero;
                } else if (message.s_header.s_type == DONE) {
                    processInformation->liveProcesses[local_id*one + zero] = 0*one + zero;
                }
            }
        }
        flag = 1;
        if (flag) {
            break;
        }
    }


}


