#include <linux/sched.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/file.h>
#include <net/sock.h>
#include <uapi/linux/in.h>

BPF_PERF_OUTPUT(events);

struct netevent_t {
    u32 pid;
    u64 ts;
    char comm[TASK_COMM_LEN];
    int fd;
    int uid;
    unsigned short port;
    unsigned int address;
    int inet_family;
};

extern struct socket *sockfd_lookup(int fd, int *err);
extern unsigned long __fdget(unsigned int fd);
extern unsigned long __fdget_raw(unsigned int fd);


int probe_connect_enter (struct pt_regs *ctx, int sockfd, struct sockaddr* addr, int addrlen) {
    struct sockaddr_in* poop = (struct sockaddr_in*) addr;
    if (poop->sin_family != AF_INET) {
        return 0;
    }

    //__fdget_raw(sockfd);
    //struct socket* test = sockfd_lookup(sockfd, NULL);
    struct netevent_t netevent = {};
    netevent.pid = bpf_get_current_pid_tgid();
    netevent.ts  = bpf_ktime_get_ns();
    //netevent.fd  = sk->__sk_common.skc_family;
    netevent.uid = bpf_get_current_uid_gid();
    netevent.port = poop->sin_port;
    netevent.address = poop->sin_addr.s_addr;
    bpf_get_current_comm(&netevent.comm, sizeof(netevent.comm));
    events.perf_submit(ctx, &netevent, sizeof(netevent));

    return 0;
}

int tcp_v4 (struct pt_regs *ctx, struct sock *sk, struct sockaddr *uaddr, int addr_len) {
    struct sockaddr_in* poop = (struct sockaddr_in*) uaddr;
    if (poop->sin_family != AF_INET) {
        return 0;
    }

    struct netevent_t netevent = {};
    netevent.pid = bpf_get_current_pid_tgid();
    netevent.ts  = bpf_ktime_get_ns();
    netevent.fd  = sk->__sk_common.skc_family;
    netevent.uid = bpf_get_current_uid_gid();
    netevent.port = poop->sin_port;
    netevent.address = poop->sin_addr.s_addr;
    bpf_get_current_comm(&netevent.comm, sizeof(netevent.comm));
    events.perf_submit(ctx, &netevent, sizeof(netevent));

    return 0;
}

int udp_v4 (struct pt_regs *ctx, struct sock *sk, struct msghdr *msg, size_t len) {
    struct netevent_t netevent = {};
    sk = (struct sock *)PT_REGS_PARM1(ctx);

    netevent.pid = bpf_get_current_pid_tgid();
    netevent.ts  = bpf_ktime_get_ns();
    netevent.fd  = sk->__sk_common.skc_family;
    netevent.uid = bpf_get_current_uid_gid();
    netevent.port = 0x35;//poop->sin_port;
    netevent.address = 0;//poop->sin_addr.s_addr;
    strcpy(netevent.comm, "sagar");
//    bpf_get_current_comm(&netevent.comm, sizeof(netevent.comm));
    events.perf_submit(ctx, &netevent, sizeof(netevent));

    return 0;
}

