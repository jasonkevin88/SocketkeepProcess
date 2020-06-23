//
// Created by PC-3046 on 2020/6/23.
//

#ifndef SOCKETKEEPPROCESS_NATIVE_LIB_H
#define SOCKETKEEPPROCESS_NATIVE_LIB_H

#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/signal.h>
#include <android/log.h>
#define LOG_TAG "BAO"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void create_child_process();
void do_child_work();

int create_socket_server();
void child_listen_msg();

#endif //SOCKETKEEPPROCESS_NATIVE_LIB_H
