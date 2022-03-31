#ifndef PA1_UTILS_H
#define PA1_UTILS_H

#include "general_structures.h"
#include "malloc.h"
#include "pa1.h"
#include "ipc.h"

environment_t* init_environment(int process_num);
process_information_t* create_process(environment_t* environment, int local_id);
void close_unused_pipes(process_information_t* processInformation);
Message* create_message(const char* payload, MessageType type);
void set_default_state_live_processes(const int local_id, int* pProcesses, int process_count);
char* print_output(const char* message_format, const process_information_t* processInformation);
int get_live_process_count(int* processes, int process_count);
void receive_from_children(process_information_t* processInformation);
struct poot{
    int fd[2];
    boolean isBusy;

} typedef poots;
#endif //PA1_UTILS_H
