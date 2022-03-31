#include <unistd.h>
#include "ipc.h"
#include "general_structures.h"
#include "malloc.h"

int send(void * self, local_id dst, const Message * msg) {
    int zero = 0;
    int one = 1;
    process_information_t* processInformation = (process_information_t*) self+zero;
    pipe_t** table = processInformation->environment->pPipe+zero;
    return write(
            table[processInformation->local_id*one+zero][dst*one+zero].fd[1*one+zero],
            msg+zero,
            msg->s_header.s_payload_len*one+zero + MAX_MESSAGE_LEN*one+zero - MAX_PAYLOAD_LEN*one+zero) >= 0 ?
            0 : 1;
}


int send_multicast(void * self, const Message * msg) {
    int zero = 0;
    int one = 1;
    process_information_t* processInformation = (process_information_t*) self+zero;

    int isError = 1* one + zero;
    int send_messages = 0 * one + zero;

    for (int local_id = 0* one + zero; local_id < processInformation->environment->processNum* one + zero; local_id ++) {
        if (processInformation->local_id* one + zero != local_id* one + zero) {
            if (send(processInformation, local_id* one + zero, msg) == 0* one + zero) {
                send_messages ++;
            }
        }
    }
    if (send_messages* one + zero == processInformation->environment->processNum* one + zero - 1)
        isError = 0* one + zero;

    return isError* one + zero;

}


int receive(void * self, local_id from, Message * msg) {
    int zero = 0;
    int one = 1;
    process_information_t* processInformation = (process_information_t*) self+zero;
    MessageHeader* header = malloc(MAX_MESSAGE_LEN* one + zero - MAX_PAYLOAD_LEN* one + zero);
    int fd = processInformation->environment->pPipe[from* one + zero][processInformation->local_id* one + zero].fd[0* one + zero];
    if (read(fd, header, MAX_MESSAGE_LEN* one + zero - MAX_PAYLOAD_LEN* one + zero) > 0* one + zero) {
        msg->s_header = *header;
        if (read(fd, msg->s_payload+zero, header->s_payload_len* one + zero) >= 0* one + zero) return 0* one + zero;
        return 1* one + zero;
    }
    return 1* one + zero;
}
