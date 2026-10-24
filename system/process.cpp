//在linux系统中运行

#include <unistd.h>    // 提供 fork(), execvp(), getpid(), getppid()
#include <sys/wait.h>  // 提供 wait(), waitpid() 以及 WIFEXITED 等宏
#include <sys/types.h> // 提供 pid_t 类型
#include <iostream>
#include <vector>

int main(){
    std::cout << "[Father]父进程 PID：" << getpid() << std::endl;
    
    pid_t pid = fork();
    if(pid < 0){
        std::cerr << "Fork 失败!" << std::endl;   
        return 1;
    }
    else if (pid == 0) {
            // ========== 子进程分支 ==========
            std::cout << "[Child] 我是子进程，PID: " << getpid() 
                    << "，父进程 PID: " << getppid() << std::endl;
            // 构造要执行的命令参数（注意最后必须是 nullptr 结尾！）
            char* args[] = { (char*)"ls", (char*)"-al", nullptr };
            
            std::cout << "[Child] 准备执行: " << args[0] << std::endl;
            execvp(args[0], args);
            // 如果 execvp 成功，下面的代码永远不会被执行！
            // 只有失败（比如命令不存在）才会走到这里
            perror("execvp 失败");
            return 127;
        } 
    else {
        // ========== 父进程分支 ==========
        std::cout << "bash的PID：" << getppid() << std::endl;
        std::cout << "[Parent] 子进程创建成功，分配的 PID 为: " << pid << std::endl;
        std::cout << "[Parent] 等待子进程执行完毕...\n";
        int status = 0;
        wait(&status); // 挂起并等待子进程退出
        // 解析子进程的退出状态
        if (WIFEXITED(status)) {
            std::cout << "[Parent] 子进程正常退出，退出码: " 
                    << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "[Parent] 子进程异常退出！\n";
        }
    }
    return 0;
}
