#include <jni.h>
#include <string>
#include "native-lib.h"

int m_child;
int m_parent = -1;

const char *userId;
const char *PATH = "/data/data/com.jason.socket.process/my.sock";

extern "C" JNIEXPORT jstring JNICALL
Java_com_jason_socket_process_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jason_socket_process_Watcher_watch(JNIEnv *env, jobject thiz, jstring user_id) {

    userId = env->GetStringUTFChars(user_id, NULL);
    create_child_process();
}

void create_child_process() {

    pid_t pid = fork();

    if(pid < 0) {

    } else if (pid > 0) {

    } else {
        do_child_work();
    }

}

void do_child_work() {
    //1 在子进程建立socket服务，作为服务端，等待父进程连接
    //2 读取消息来自父进程的消息：这边唯一的消息是父进程被杀掉
    if(create_socket_server()) {
        child_listen_msg();
    }
}

int create_socket_server() {

    //1 创建socket对象
    int listenId = socket(AF_LOCAL, SOCK_STREAM, 0);
    //2 断开之前的连接
    unlink(PATH);
    struct sockaddr_un addr;
    //3 清空内存
    memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_LOCAL;

    strcpy(addr.sun_path, PATH);
    int connfd = 0;
    LOGE("绑定端口号");
    if(bind(listenId, (const sockaddr *) &addr, sizeof(addr))<0) {
        LOGE("绑定错误");
        return 0;
    }
    //设置最大的连接数
    listen(listenId, 5);
    while (1) {
        LOGE("子进程循环等待连接  %d ",m_child);
        // 不断接受客户端请求的数据
        // 等待 客户端连接  accept阻塞式函数
        if ((connfd = accept(listenId, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            } else{
                LOGE("读取错误");
                return 0;
            }
        }
        //apk 进程连接上了
        m_child = connfd;
        LOGE("apk 父进程连接上了  %d ",m_child);
        break;
    }
    LOGE("返回成功");
    return 1;
}

void child_listen_msg() {

    fd_set rfds;
    while (1) {
        // 清空端口号
        FD_ZERO(&rfds);
        // 设置新的端口号
        FD_SET(m_child,&rfds);
        // 设置超时时间
        struct timeval timeout={3,0};
        int r = select(m_child + 1, &rfds, NULL, NULL, &timeout);
        LOGE("读取消息前  %d  ",r);
        if (r > 0) {
            char pkg[256] = {0};
            // 确保读到的内容是制定的端口号
            if (FD_ISSET(m_child, &rfds)) {
                // 阻塞式函数  客户端写到内容
                int result = read(m_child, pkg, sizeof(pkg));
                // 读到内容的唯一方式 是客户端断开
                LOGE("重启父进程  %d ",result);
                LOGE("读到信息  %d    userid  %d ",result, userId);
                execlp("am", "am", "startservice", "--user", userId,
                       "com.jason.socket.process/com.jason.socket.process.KeepProcessService", (char*)NULL);
                break;
            }
        }



    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jason_socket_process_Watcher_connect(JNIEnv *env, jobject thiz) {

    //子进程1    父进程2
    int sockfd;
    struct sockaddr_un  addr;
    while (1) {
        LOGE("客户端  父进程开始连接");
        sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (sockfd < 0) {
            return;
        }
        memset(&addr, 0, sizeof(sockaddr_un));
        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path, PATH);
        if (connect(sockfd, (const sockaddr *) &addr, sizeof(addr)) < 0) {
            LOGE("连接失败  休眠");
            // 连接失败
            close(sockfd);
            sleep(1);
            // 再来继续下一次尝试
            continue;
        }
        // 连接成功
        m_parent = sockfd;
        LOGE("连接成功  父进程跳出循环");
        break;
    }
}