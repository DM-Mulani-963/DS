#include <bits/stdc++.h>
using namespace std;

struct Operation {
    string type;     // send, recv, print
    string target;   // pN
    string msg;
};

struct Message {
    string from, to, msg;
    vector<int> clock;   // vector clock
};

struct Process {
    string name;
    vector<Operation> ops;
    int pc = 0;
    vector<int> clock;
    bool blocked = false;
};

string vcToString(const vector<int>& vc) {
    string s = "[";
    for (int i = 0; i < (int)vc.size(); i++) {
        s += to_string(vc[i]);
        if (i + 1 < (int)vc.size()) s += ",";
    }
    s += "]";
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Process> processes;
    unordered_map<string, int> pid;

    string line;

    // ---------- INPUT ----------
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

    int n = processes.size();

    // Initialize vector clocks
    for (auto &p : processes)
        p.clock.assign(n, 0);

    vector<Message> msgPool;
    vector<string> log;

    // ---------- EXECUTION ----------
    while (true) {
        bool progress = false;

        for (auto &p : processes) {
            int i = pid[p.name];

            if (p.pc >= (int)p.ops.size())
                continue;

            Operation &op = p.ops[p.pc];

            // SEND
            if (op.type == "send") {
                p.clock[i]++;

                msgPool.push_back({
                    p.name,
                    op.target,
                    op.msg,
                    p.clock
                });

                log.push_back("sent " + p.name + " " + op.msg + " " +
                              op.target + " " + vcToString(p.clock));

                p.pc++;
                p.blocked = false;
                progress = true;
            }

            // PRINT
            else if (op.type == "print") {
                p.clock[i]++;

                log.push_back("printed " + p.name + " " + op.msg +
                              " " + vcToString(p.clock));

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

                        // VC = max(VC, msg.VC)
                        for (int k = 0; k < n; k++)
                            p.clock[k] = max(p.clock[k], it->clock[k]);

                        p.clock[i]++;

                        log.push_back("received " + p.name + " " + op.msg +
                                      " " + it->from + " " +
                                      vcToString(p.clock));

                        msgPool.erase(it);
                        p.pc++;
                        p.blocked = false;
                        progress = true;
                        found = true;
                        break;
                    }
                }
                p.blocked = !found;
            }
        }

        // ---------- TERMINATION / DEADLOCK ----------
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
            }

            break;  // normal completion
        }
    }

    // ---------- OUTPUT ----------
    for (auto &e : log)
        cout << e << "\n";

    for (auto &m : msgPool) {
        cout << "message " << m.msg << " (" 
             << m.from << " -> " << m.to 
             << ") not delivered\n";
    }

    return 0;
}