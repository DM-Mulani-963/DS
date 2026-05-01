#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>

#define DIR_RIGHT 0
#define DIR_LEFT  1

typedef enum { CANDIDATE, LOST, WON } Status;

static int my_id, total_nodes;
static int left_id,  right_id;
static int left_port, right_port;       
static int srv_left_port, srv_right_port; 

static Status status    = CANDIDATE;
static int    cur_phase = 0;
static int    echo_count = 0;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static volatile int    election_done = 0;


static const char *status_str(Status s)
{
    if (s == CANDIDATE) return "candidate";
    if (s == LOST)      return "lost";
    return "won";
}

static void send_msg(int dest_id, int port, const char *msg)
{
    char disp[256];
    strncpy(disp, msg, sizeof(disp)-1);
    disp[sizeof(disp)-1] = '\0';
    int len = strlen(disp);
    if (len > 0 && disp[len-1] == '\n') disp[len-1] = '\0';

    printf("(%d) sending message to (%d): (%s)\n", my_id, dest_id, disp);
    fflush(stdout);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int retries = 50;
    while (retries-- > 0) {
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) break;
        close(sock);
        sock = socket(AF_INET, SOCK_STREAM, 0);
        usleep(100000);
    }
    send(sock, msg, strlen(msg), 0);
    close(sock);
}

static void forward_probe(int id, int phase, int ttl, int dir)
{
    char msg[128];
    snprintf(msg, sizeof(msg), "SENDPASS %d %d %d %d\n", id, phase, ttl, dir);
    if (dir == DIR_RIGHT)
        send_msg(right_id, right_port, msg);
    else
        send_msg(left_id,  left_port,  msg);
}

static void forward_echo(int id, int phase, int dir)
{
    char msg[128];
    snprintf(msg, sizeof(msg), "SENDECHO %d %d %d\n", id, phase, dir);
    if (dir == DIR_RIGHT)
        send_msg(right_id, right_port, msg);
    else
        send_msg(left_id,  left_port,  msg);
}

static void start_phase(int phase)
{
    int ttl = (1 << phase);   /* 2^phase */
    char msg[128];

    snprintf(msg, sizeof(msg), "SENDPASS %d %d %d %d\n", my_id, phase, ttl, DIR_RIGHT);
    send_msg(right_id, right_port, msg);

    snprintf(msg, sizeof(msg), "SENDPASS %d %d %d %d\n", my_id, phase, ttl, DIR_LEFT);
    send_msg(left_id,  left_port,  msg);
}

static void handle_message(const char *raw, int from_left)
{
    int sender = from_left ? left_id : right_id;

    char disp[256];
    strncpy(disp, raw, sizeof(disp)-1);
    disp[sizeof(disp)-1] = '\0';
    int dl = strlen(disp);
    if (dl > 0 && disp[dl-1] == '\n') disp[dl-1] = '\0';

    pthread_mutex_lock(&lock);
    Status cur = status;
    pthread_mutex_unlock(&lock);

    printf("(%d) received a message from (%d): (%s); Status of the node: %s\n",
           my_id, sender, disp, status_str(cur));
    fflush(stdout);

    char type[32];
    sscanf(raw, "%31s", type);

    /* ── LEADER ── */
    if (strcmp(type, "LEADER") == 0) {
        int lid;
        sscanf(raw, "LEADER %d", &lid);

        if (lid != my_id) {
            if (from_left)
                send_msg(right_id, right_port, raw);
            else
                send_msg(left_id,  left_port,  raw);
        }

        pthread_mutex_lock(&lock);
        if (lid == my_id) status = WON;
        pthread_mutex_unlock(&lock);

        election_done = 1;
        return;
    }

    /* ── SENDPASS ── */
    if (strcmp(type, "SENDPASS") == 0) {
        int id, phase, ttl, dir;
        sscanf(raw, "SENDPASS %d %d %d %d", &id, &phase, &ttl, &dir);

        if (id == my_id) {
            pthread_mutex_lock(&lock);
            echo_count++;
            int ec = echo_count;
            int ph = cur_phase;
            pthread_mutex_unlock(&lock);
            if (ec >= 2) {
                pthread_mutex_lock(&lock);
                echo_count = 0;
                cur_phase++;
                int np = cur_phase;
                pthread_mutex_unlock(&lock);
                if ((1 << np) >= total_nodes) {
                    pthread_mutex_lock(&lock);
                    status = WON;
                    pthread_mutex_unlock(&lock);
                    printf("\n*** (%d) I AM THE LEADER ***\n\n", my_id);
                    char lmsg[64];
                    snprintf(lmsg, sizeof(lmsg), "LEADER %d\n", my_id);
                    send_msg(left_id,  left_port,  lmsg);
                    send_msg(right_id, right_port, lmsg);
                } else {
                    start_phase(np);
                }
            }
            return;
        }

        if (id > my_id) {
            pthread_mutex_lock(&lock);
            if (status == CANDIDATE) status = LOST;
            pthread_mutex_unlock(&lock);

            if (ttl > 1) {
                
                forward_probe(id, phase, ttl - 1, dir);
            } else {
                int echo_dir = (dir == DIR_RIGHT) ? DIR_LEFT : DIR_RIGHT;
                forward_echo(id, phase, echo_dir);
            }
        }
       
        return;
    }

    /* ── SENDECHO ── */
    if (strcmp(type, "SENDECHO") == 0) {
        int id, phase, dir;
        sscanf(raw, "SENDECHO %d %d %d", &id, &phase, &dir);

        if (id == my_id) {
            pthread_mutex_lock(&lock);
            echo_count++;
            int ec = echo_count;
            pthread_mutex_unlock(&lock);

            if (ec == 2) {
                pthread_mutex_lock(&lock);
                echo_count = 0;
                cur_phase++;
                int np = cur_phase;
                pthread_mutex_unlock(&lock);

                if ((1 << np) >= total_nodes) {
                    pthread_mutex_lock(&lock);
                    status = WON;
                    pthread_mutex_unlock(&lock);

                    printf("\n*** (%d) I AM THE LEADER ***\n\n", my_id);
                    fflush(stdout);

                    char lmsg[64];
                    snprintf(lmsg, sizeof(lmsg), "LEADER %d\n", my_id);
                    send_msg(left_id,  left_port,  lmsg);
                    send_msg(right_id, right_port, lmsg);
                } else {
                    start_phase(np);
                }
            }
            return;
        }

        forward_echo(id, phase, dir);
        return;
    }

    fprintf(stderr, "(%d) unknown message: %s\n", my_id, raw);
}


typedef struct { int port; int from_left; } Arg;

static void *server_thread(void *a)
{
    Arg *arg = (Arg*)a;

    int srv = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(arg->port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(srv, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); return NULL;
    }
    listen(srv, 16);

    while (!election_done) {
        int c = accept(srv, NULL, NULL);
        if (c < 0) continue;

        char buf[512] = {0};
        int n = recv(c, buf, sizeof(buf)-1, 0);
        close(c);

        if (n > 0) {
            buf[n] = '\0';
            handle_message(buf, arg->from_left);
        }
    }
    close(srv);
    return NULL;
}

  

int main(int argc, char *argv[])
{
    if (argc != 9) {
        fprintf(stderr,
            "Usage: %s total my_id left_id right_id "
            "send_left_port send_right_port srv_left_port srv_right_port\n",
            argv[0]);
        return 1;
    }

    total_nodes    = atoi(argv[1]);
    my_id          = atoi(argv[2]);
    left_id        = atoi(argv[3]);
    right_id       = atoi(argv[4]);
    left_port      = atoi(argv[5]);
    right_port     = atoi(argv[6]);
    srv_left_port  = atoi(argv[7]);
    srv_right_port = atoi(argv[8]);

    printf("(%d) starting. left=%d right=%d\n", my_id, left_id, right_id);
    fflush(stdout);

    pthread_t t1, t2;
    Arg a1 = { srv_left_port,  1 };  /* receives msgs from left  neighbor */
    Arg a2 = { srv_right_port, 0 };  /* receives msgs from right neighbor */
    pthread_create(&t1, NULL, server_thread, &a1);
    pthread_create(&t2, NULL, server_thread, &a2);

    sleep(2);  /* let all nodes bind before any sends */

    printf("(%d) starting phase 0\n", my_id);
    fflush(stdout);
    start_phase(0);

    while (!election_done) sleep(1);
    sleep(1); 

    pthread_mutex_lock(&lock);
    printf("(%d) Election over. Final status: %s\n", my_id, status_str(status));
    pthread_mutex_unlock(&lock);
    fflush(stdout);

    return 0;
}