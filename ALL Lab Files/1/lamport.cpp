#include <bits/stdc++.h>
using namespace std;

struct Operation {
    string type;     // send, recv, print
    string target;   // pN
    string msg;
};

struct Message {
    string from, to, msg;
};

struct Process {
    string name;
    vector<Operation> ops;
    int pc = 0;
    int clock = 0;
    bool blocked = false;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Process> processes;
    unordered_map<string, int> pid;

    string line;
    while (getline(cin, line)) {
        if (line.find("begin process") != string::npos) {
            Process p;
            string tmp;
            stringstream ss(line);
            ss >> tmp >> tmp >> p.name;
            pid[p.name] = processes.size();

            while (getline(cin, line)) {
                if (line.find("end process") != string::npos)
                    break;

                stringstream os(line);
                Operation op;
                os >> op.type;

                if (op.type == "send" || op.type == "recv") {
                    os >> op.target >> op.msg;
                } else if (op.type == "print") {
                    os >> op.msg;
                }
                p.ops.push_back(op);
            }
            processes.push_back(p);
        }
    }

    vector<Message> msgPool;
    vector<string> log;

    while (true) {
        bool progress = false;

        for (auto &p : processes) {
            if (p.pc >= (int)p.ops.size())
                continue;

            Operation &op = p.ops[p.pc];

            // SEND
            if (op.type == "send") {
                p.clock++;
                msgPool.push_back({p.name, op.target, op.msg});
                log.push_back("sent " + p.name + " " + op.msg + " " +
                              op.target + " (" + to_string(p.clock) + ")");
                p.pc++;
                p.blocked = false;
                progress = true;
            }

            // PRINT
            else if (op.type == "print") {
                p.clock++;
                log.push_back("printed " + p.name + " " + op.msg +
                              " (" + to_string(p.clock) + ")");
                p.pc++;
                p.blocked = false;
                progress = true;
            }

            // RECEIVE
            else if (op.type == "recv") {
                bool found = false;
                for (auto it = msgPool.begin(); it != msgPool.end(); ++it) {
                    if (it->from == op.target &&
                        it->to == p.name &&
                        it->msg == op.msg) {

                        p.clock = max(p.clock, processes[pid[it->from]].clock) + 1;
                        log.push_back("received " + p.name + " " + op.msg +
                                      " " + it->from + " (" +
                                      to_string(p.clock) + ")");
                        msgPool.erase(it);
                        p.pc++;
                        p.blocked = false;
                        progress = true;
                        found = true;
                        break;
                    }
                }
                p.blocked = !found; // true only if message not found
            }
        }

        // Check if no process made progress
        if (!progress) {
            bool anyBlocked = false;
            for (auto &p : processes) {
                if (p.pc < (int)p.ops.size() && p.blocked)
                    anyBlocked = true;
            }

            if (anyBlocked) {
                for (auto &e : log)
                    cout << e << "\n";
                cout << "system deadlocked\n";
                return 0;
            } else {
                break; // no progress, but no blocked processes → finished
            }
        }
    }

    // Print execution log
    for (auto &e : log)
        cout << e << "\n";

    // Print undelivered messages
    for (auto &m : msgPool) {
        cout << "message " << m.msg << " (" << m.from
             << " -> " << m.to << ") not delivered\n";
    }

    return 0;
}