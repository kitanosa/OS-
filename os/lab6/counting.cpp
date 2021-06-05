#include <unordered_map>
#include <unistd.h>
#include <sstream>
#include <unordered_map>
#include "zmq_functions.h"
#include <ctime>
#include <chrono>
#include <ctime>

#define SEC 1000000000
int main(int argc, char* argv[]) {
    struct timespec start_clock, stop_clock;
    double time_clock;
    if (argc != 2 && argc != 3) {
        throw std::runtime_error("Wrong args for counting node");
    }
    int cur_id = std::atoi(argv[1]);
    int child_id = -1;
    if (argc == 3) {
        child_id = std::atoi(argv[2]);
    }

    //auto start_clock = std::chrono::steady_clock::now();
    //auto stop_clock = std::chrono::steady_clock::now();
    bool flag_clock = false;
    //double time_clock;

    std::unordered_map<std::string, int> dictionary;

    zmq::context_t context;
    zmq::socket_t parent_socket(context, ZMQ_REP); 
    connect(parent_socket, cur_id);

    zmq::socket_t child_socket(context, ZMQ_REQ);
    child_socket.setsockopt(ZMQ_SNDTIMEO, 5000); 
    if (child_id != -1) {
        bind(child_socket, child_id);
    }
 
    std::string message;
    while (true) {
        message = receive_message(parent_socket);
        std::istringstream request(message);
        int dest_id;
        request >> dest_id;

        std::string cmd;
        request >> cmd;

        if (dest_id == cur_id) {

            if (cmd == "pid") {
                send_message(parent_socket, "OK: " + std::to_string(getpid()));
            }

            else if (cmd == "create") {
                int new_child_id;
                request >> new_child_id;
                if (child_id != -1) {
                    unbind(child_socket, child_id);
                }
                bind(child_socket, new_child_id);
                pid_t pid = fork();
                if (pid < 0) {
                    perror("Can't create new process");
                    return -1;
                }
                if (pid == 0) {
                    execl("./counting", "./counting", std::to_string(new_child_id).c_str(), std::to_string(child_id).c_str(), NULL);
                    perror("Can't execute new process");
                    return -2;
                }
                send_message(child_socket, std::to_string(new_child_id) + "pid");
                child_id = new_child_id;
                send_message(parent_socket, receive_message(child_socket));
            }
            else if (cmd == "exec") {
                std::string result;
                std::string cmd;
                request >> cmd;

                if (cmd == "start") {
                    //start_clock = std::chrono::high_resolution_clock::now();
                    timespec_get(&start_clock, TIME_UTC);
                    

                    flag_clock = true;
                }
                else if (cmd == "stop") {
                    if (flag_clock) {
                        //stop_clock = std::chrono::high_resolution_clock::now();
                        timespec_get(&stop_clock, TIME_UTC);
                        time_clock += ((stop_clock.tv_sec * SEC + stop_clock.tv_nsec) - (start_clock.tv_sec * SEC + start_clock.tv_nsec)) / (double) SEC, result;
                        flag_clock = false;
                    }
                }
                else if (cmd == "time") {
                    result = std::to_string(time_clock);
                }
                send_message(parent_socket, "OK: " + std::to_string(cur_id) + ": " + result);
            }
            /* else if (cmd == "heartbeat") {
                std::string reply;
                if (child_id != -1) {
                    send_message(child_socket, std::to_string(child_id) + " heartbeat");
                    std::string msg = receive_message(child_socket);
                    reply += " " + msg;
                }
                send_message(parent_socket, std::to_string(cur_id) + reply);
            } */
            else if (cmd == "kill") {
                if (child_id != -1) {
                    send_message(child_socket, std::to_string(child_id) + " kill");
                    std::string msg = receive_message(child_socket);
                    if (msg == "OK") {
                        send_message(parent_socket, "OK");
                    }
                    unbind(child_socket, child_id);
                    disconnect(parent_socket, cur_id);
                    break;
                }
                send_message(parent_socket, "OK");
                disconnect(parent_socket, cur_id);
                break;
            }
        }
        else if (child_id != -1) {
            send_message(child_socket, message);
            send_message(parent_socket, receive_message(child_socket));
            if (child_id == dest_id && cmd == "kill") {
                child_id = -1;
            }
        }
        else {
            send_message(parent_socket, "Error: node is unavailable");
        }
    }
}
