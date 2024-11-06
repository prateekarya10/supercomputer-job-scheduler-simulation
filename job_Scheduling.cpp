#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm>
#include <limits.h>
using namespace std;

struct Job {
    int jobId;
    int arrivalTime, coresRequired, memoryRequired, executionTime;
    Job(int id, int at, int cr, int mr, int et)
        : jobId(id), arrivalTime(at), coresRequired(cr), memoryRequired(mr), executionTime(et) {}
    int grossValue() const { return coresRequired * memoryRequired * executionTime; }
};

struct WorkerNode {
    int id, availableCores = 24, availableMemory = 64;
    int totalCores = 24, totalMemory = 64;
    WorkerNode(int id) : id(id) {}
    bool allocate(Job& job) {
        if (availableCores >= job.coresRequired && availableMemory >= job.memoryRequired) {
            availableCores -= job.coresRequired;
            availableMemory -= job.memoryRequired;
            return true;
        }
        return false;
    }
    void release(Job& job) {
        availableCores += job.coresRequired;
        availableMemory += job.memoryRequired;
    }
    double cpuUtilization() const {
        return (1.0 - double(availableCores) / totalCores) * 100;
    }
    double memoryUtilization() const {
        return (1.0 - double(availableMemory) / totalMemory) * 100;
    }
};

class MasterScheduler {
private:
    vector<WorkerNode> workerNodes;
    vector<Job> jobQueue;
    const int maxRetries = 5;
    int currentTime = 0;

    void sortJobsFCFS() {
        sort(jobQueue.begin(), jobQueue.end(), [](const Job& a, const Job& b) {
            return a.arrivalTime < b.arrivalTime;
        });
    }

    void sortJobsBySmallest() {
        sort(jobQueue.begin(), jobQueue.end(), [](const Job& a, const Job& b) {
            return a.grossValue() < b.grossValue();
        });
    }

    void sortJobsByDuration() {
        sort(jobQueue.begin(), jobQueue.end(), [](const Job& a, const Job& b) {
            return a.executionTime < b.executionTime;
        });
    }

    WorkerNode* findFirstFit(Job& job) {
        for (auto& worker : workerNodes) {
            if (worker.allocate(job)) return &worker;
        }
        return nullptr;
    }

    WorkerNode* findBestFit(Job& job) {
        WorkerNode* bestFit = nullptr;
        int minWaste = INT_MAX;
        for (auto& worker : workerNodes) {
            if (worker.allocate(job)) {
                int waste = (worker.availableCores - job.coresRequired) + (worker.availableMemory - job.memoryRequired);
                if (waste < minWaste) {
                    minWaste = waste;
                    bestFit = &worker;
                }
                worker.release(job);  // Release and check next fit
            }
        }
        return bestFit;
    }

    WorkerNode* findWorstFit(Job& job) {
        WorkerNode* worstFit = nullptr;
        int maxWaste = 0;
        for (auto& worker : workerNodes) {
            if (worker.allocate(job)) {
                int waste = (worker.availableCores - job.coresRequired) + (worker.availableMemory - job.memoryRequired);
                if (waste > maxWaste) {
                    maxWaste = waste;
                    worstFit = &worker;
                }
                worker.release(job);  // Release and check next fit
            }
        }
        return worstFit;
    }

public:
    MasterScheduler(int numWorkers) {
        for (int i = 0; i < numWorkers; i++) workerNodes.emplace_back(i);
    }

    void addJob(Job job) { jobQueue.push_back(job); }

    void scheduleJobs(string queuePolicy, string fitPolicy, ofstream& outfile) {
        if (queuePolicy == "FCFS") {
            sortJobsFCFS();
        } else if (queuePolicy == "smallest") {
            sortJobsBySmallest();
        } else if (queuePolicy == "duration") {
            sortJobsByDuration();
        }

        while (!jobQueue.empty()) {
            currentTime++;
            vector<Job> pendingJobs;

            for (auto& job : jobQueue) {
                bool scheduled = false;
                for (int retries = 0; retries < maxRetries && !scheduled; retries++) {
                    WorkerNode* selectedWorker = nullptr;
                    if (fitPolicy == "first") {
                        selectedWorker = findFirstFit(job);
                    } else if (fitPolicy == "best") {
                        selectedWorker = findBestFit(job);
                    } else if (fitPolicy == "worst") {
                        selectedWorker = findWorstFit(job);
                    }
                    if (selectedWorker) {
                        cout << "JobId: " << job.jobId 
                             << " Arrival Day: " << job.arrivalTime / 24 
                             << " Time Hour: " << job.arrivalTime % 24 
                             << " MemReq: " << job.memoryRequired 
                             << " CPUReq: " << job.coresRequired 
                             << " ExeTime: " << job.executionTime << endl;
                        scheduled = true;
                        break;
                    }
                }
                if (!scheduled) {
                    pendingJobs.push_back(job);  // Only add unscheduled jobs to pending
                }
            }

            jobQueue = pendingJobs;
            collectAndSaveUtilization(outfile);
        }
    }

    void collectAndSaveUtilization(ofstream& outfile) {
        double totalCpuUtil = 0.0, totalMemUtil = 0.0;
        for (const auto& worker : workerNodes) {
            totalCpuUtil += worker.cpuUtilization();
            totalMemUtil += worker.memoryUtilization();
        }
        outfile << currentTime << "," << totalCpuUtil / workerNodes.size() << "," 
                << totalMemUtil / workerNodes.size() << endl;
    }
};

void generateJobs(MasterScheduler& scheduler, int numJobs) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> arrival(0, 10), cores(1, 24), memory(1, 20), execution(1, 5);
    for (int i = 0; i < numJobs; i++) {
        scheduler.addJob(Job(i, arrival(gen), cores(gen), memory(gen), execution(gen)));
    }
}

int main() {
    MasterScheduler scheduler(128);
    generateJobs(scheduler, 5000);

    // Prepare output file for CSV
    ofstream outfile("utilization.csv");
    outfile << "Time,CPU Utilization,Memory Utilization\n";

    // Test with different policies
    scheduler.scheduleJobs("FCFS", "first", outfile);
    scheduler.scheduleJobs("smallest", "best", outfile);
    scheduler.scheduleJobs("duration", "worst", outfile);

    // Close the output file
    outfile.close();
    
    return 0;
}