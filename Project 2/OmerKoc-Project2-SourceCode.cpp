// Omer Seyfeddin Koc - EECE7205 / Project2

// Required Libraries
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <limits>
#include <numeric>
#include "iomanip"
#include <cstring>
#include <chrono>


using namespace std;

struct Task {
  int id;  // Unique identifier for the task
  vector<int> localExecutions; // Local execution times on each core
  vector<int> preID; // Identifiers of preceding tasks
  vector<int> succID; // Identifiers of succeeding tasks
  vector<int> finishTimes;  // Vector for finish times of the task
  vector<int> readyTimes;  // Vector for ready times of the task 
  int startingTime = 0; // Starting time of the task, initialized to 0 by default
  int assignedCore = -1; // The core to which the task is assigned (0: cloud, -1: not assigned yet)
  double weight = 0; // weight (weight) of the task
  double priority = 0; // Priority of the task
};

// Initialize cloud execution times for each core
vector<int> cloudTimes = {3, 1, 1};

// Initialize power values for each core
vector<float> corePower = { 1, 2, 4, 0.5};

// Number of available cores
const int coresNumber = 3;

// Number of tasks to be scheduled
const int tasksNumber = 10;

vector<Task> initializeTasks() {
    // Initialize tasks
    return { 
    { },
    {1, {9, 7, 5}, {}, {2, 3, 4, 5, 6}},
    {2, {8, 6, 5}, {1}, {8, 9}},
    {3, {6, 5, 4}, {1}, {7}},
    {4, {7, 5, 3}, {1}, {8, 9}},
    {5, {5, 4, 2}, {1}, {9}},
    {6, {7, 6, 4}, {1}, {8}},
    {7, {8, 5, 3}, {3}, {10}},
    {8, {6, 4, 2}, {2, 4, 6}, {10}},
    {9, {5, 3, 2}, {2, 4, 5}, {10}},
    {10, {7, 4, 2}, {7, 8, 9}, {}}
  };
}

// Function to initialize time-related vectors for tasks
void initializeTime(vector<Task>& tasks) {
    for (Task& task : tasks) {
        task.finishTimes.resize(4, 0); // Initialize the finishTimes vector with 4 zeros
        task.readyTimes.resize(4, 0); // Initialize the readyTimes vector with 4 zeros
    }
}

// Declare a vector to store Task objects
vector<Task> tasks;
// Declare vectors for wireless send queue, and exit tasks
vector<int> wirelessSendQueue, exitTasks;
// Declare a vector of vectors to represent core queues
vector<vector<int>> cores(coresNumber);

// Function to update the priority of a Task
void updatePriority(Task &task) {
  // Check if the task's priority has not been set yet
  if (task.priority == 0) {
    // If the task has no successors, set its priority equal to its weight
    if (task.succID.empty()) {
      task.priority = task.weight;
    } else {
      double maxPriority = 0;
      // Iterate through the successor tasks and recursively update their priorities
      for (int successor : task.succID) {
        updatePriority(tasks[successor]);
        // Find the maximum priority among the successors
        maxPriority = max(maxPriority, tasks[successor].priority);
      }
      // Set the task's priority as the sum of its weight and the maximum priority of its successors
      task.priority = task.weight + maxPriority;
    }
  }
}

// Function to calculate the ready time for the wireless send stage
int calculateSendRT(const vector<Task> &tasks, const vector<int> &queue) {
  if (!queue.empty()) {
    int lastIndex = queue.back();
    // Return the finish time of the last task in the queue for stage 1 (wireless send)
    return tasks[lastIndex].finishTimes[1];
  }
  // If the queue is empty, return 0 as there are no tasks in the queue
  return 0;
}

// Function to calculate the ready time for the core stage
int calculateCoreRT(const vector<Task> &tasks, const vector<int> &queue) {
  if (!queue.empty()) {
    int lastIndex = queue.back();
    // Return the finish time of the last task in the queue for stage 0 (core execution)
    return tasks[lastIndex].finishTimes[0];
  }
  // If the queue is empty, return 0 as there are no tasks in the queue
  return 0;
}

// Function to compute time-related information for the cloud stage of a task
void computeCloudTimes(const vector<Task> &tasks, Task &task, const vector<int> wirelessQueue) {
  // Calculate the maximum finish time among predecessors for stages 0 and 1
  int maximumFTSending = 0;
  for (int j : task.preID) {
    int max_FT = max(tasks[j].finishTimes[0], tasks[j].finishTimes[1]);
    maximumFTSending = max(maximumFTSending, max_FT);
  }

  // Stage 1 calculations
  task.readyTimes[1] = maximumFTSending;
  task.startingTime = max(task.readyTimes[1], calculateSendRT(tasks, wirelessQueue));
  task.finishTimes[1] = task.startingTime + cloudTimes[0];

  // Calculate the maximum finish time among predecessors for Cloud Computing
  int maximumFTCloud = 0;
  for (int j : task.preID) {
    maximumFTCloud = max(maximumFTCloud, tasks[j].finishTimes[2]);
  }

  // Cloud Computing calculations
  task.readyTimes[2] = max(task.finishTimes[1], maximumFTCloud);
  task.finishTimes[2] = task.readyTimes[2] + cloudTimes[1];

  // Wireless Receiving calculations
  task.readyTimes[3] = task.finishTimes[2];
  task.finishTimes[3] = task.finishTimes[2] + cloudTimes[2];
}


// Function to compute core execution times for a task
void computeCoreTimes(const vector<Task> &tasks, Task &task, const vector<int> coreQueue, int coreNumber) {
  int maxPredFT = 0;  // Maximum finish time among predecessors

  // Find the maximum finish time among predecessors
  for (int j : task.preID) {
    // Choose the maximum finish time between local execution and wireless receive
    int currentMaxFT = (tasks[j].finishTimes[0] > tasks[j].finishTimes[3]) ? tasks[j].finishTimes[0] : tasks[j].finishTimes[3];
    if (currentMaxFT > maxPredFT) {
      maxPredFT = currentMaxFT;
    }
  }

  // Calculate the ready time of the task
  task.readyTimes[0] = maxPredFT;

  // Calculate the starting time of the task using a function related to coreQueue
  task.startingTime = max(task.readyTimes[0], calculateCoreRT(tasks, coreQueue));

  // Calculate the finish time of the task
  task.finishTimes[0] = task.startingTime + task.localExecutions[coreNumber];
}

// Function to compute the total energy consumption
double computeTotalEnergy(const vector<Task> &tasks, const vector<int> &sendQueue, const vector<vector<int>> &coreQueue) {
  double energy = 0.0;

  // Calculate energy consumption for wireless communication
  double wirelessEnergy = static_cast<double>(sendQueue.size()) * cloudTimes[0] * corePower[3];
  energy += wirelessEnergy;

  // Calculate energy consumption for each core
  for (int i = 0; i < coresNumber; ++i) {
    for (int j : coreQueue[i]) {
      // Calculate the execution time for the task
      double executionTime = tasks[j].finishTimes[0] - tasks[j].startingTime;

      // Calculate energy consumption for the core
      double coreEnergy = executionTime * corePower[i];
      energy += coreEnergy;
    }
  }

  return energy;
}

// Function to compute the total time
int computeTotalTime(const vector<Task> &tasks) {
  int maxOverallFT = 0;  // Variable to hold the maximum finish time overall

  // Iterate through exit tasks to find the maximum finish time among them
  for (int i : exitTasks) {
    // Find the maximum finish time for the current task
    int maxFTtask = max(tasks[i].finishTimes[0], tasks[i].finishTimes[3]);

    // Update the overall maximum finish time
    maxOverallFT = max(maxOverallFT, maxFTtask);
  }

  return maxOverallFT;
}


void printAssignment(const vector<Task> &tasks, const vector<int> &wirelessQueue,
                      const vector<vector<int>> &coreQueues, double energyTotal, int timeTotal) {
    int maxTimeline = timeTotal;
    int numCores = coreQueues.size();
    
    // Set the maximum timeline to 27
    maxTimeline = max(maxTimeline, 27);
    
    // Calculate the length of core headers and WL Send header
    int headerLength = 0;
    for (int core = 0; core < numCores; ++core) {
        headerLength = max(headerLength, static_cast<int>(to_string(core + 1).length()));
    }
    headerLength = max(headerLength, static_cast<int>(strlen("WL Sending")));
    
    cout << "Time:       ";
    for (int i = 0; i <= maxTimeline; ++i) {
        cout << setw(3) << i;
    }
    cout << endl;
    
    cout << string(99, '-') << endl;
    
    for (int core = 0; core < numCores; ++core) {
        cout << "Core " << core + 1 << setw(5) << "       :";
        for (int i = 0; i <= maxTimeline; ++i) { 
            bool taskScheduled = false;
            for (int taskId : coreQueues[core]) {
                const Task &task = tasks[taskId];
                if (task.startingTime <= i && i < task.finishTimes[0]) {
                    cout << setw(3) << taskId;
                    taskScheduled = true;
                    break;
                }
            }
            if (!taskScheduled) {
                cout << "  -";
            }
        }
        cout << endl;
    }
    
    cout << "WL Sending   :";
    for (int i = 0; i <= maxTimeline; ++i) { 
        bool taskScheduled = false;
        for (int taskId : wirelessQueue) {
            const Task &task = tasks[taskId];
            if (task.startingTime <= i && i < task.finishTimes[1]) {
                cout << setw(3) << taskId;
                taskScheduled = true;
                break;
            }
        }
        if (!taskScheduled) {
            cout << "  -";
        }
    }
    cout << endl;
    
    cout << "Cloud        :";
    for (int i = 0; i <= maxTimeline; ++i) { 
        bool taskScheduled = false;
        for (int taskId : wirelessQueue) {
            const Task &task = tasks[taskId];
            if (task.readyTimes[2] <= i && i < task.finishTimes[2]) {
                cout << setw(3) << taskId;
                taskScheduled = true;
                break;
            }
        }
        if (!taskScheduled) {
            cout << "  -";
        }
    }
    cout << endl;
    
    cout << "WL Receiving :";
    for (int i = 0; i <= maxTimeline; ++i) { 
        bool taskScheduled = false;
        for (int taskId : wirelessQueue) {
            const Task &task = tasks[taskId];
            if (task.readyTimes[3] <= i && i < task.finishTimes[3]) {
                cout << setw(3) << taskId;
                taskScheduled = true;
                break;
            }
        }
        if (!taskScheduled) {
            cout << "  -";
        }
    }
    cout << endl;
    
    cout << string(99, '-') << endl;
    cout << "Total Energy (energyTotal): " << energyTotal << endl;
    cout << "Total Time (timeTotal): " << timeTotal << endl;
}


void removeTaskFromCore(vector<vector<int>> &scheduling, int core, int taskId) {
    auto &queue = scheduling[core];
    queue.erase(remove(queue.begin(), queue.end(), taskId), queue.end());
}

void addTaskToCore(vector<vector<int>> &scheduling, int core, int taskId, const vector<Task> &tasks) {
    auto &queue = scheduling[core];
    
    // Find the position where the task should be inserted
    auto insertPos = find_if(queue.begin(), queue.end(),
                             [&](int id) { return tasks[id].startingTime >= tasks[taskId].readyTimes[0]; });
    
    // Insert the task into the found position
    queue.insert(insertPos, taskId);
}

void resetTaskScheduling(vector<Task> &tasks) {
    for (auto &task : tasks) {
        fill(task.readyTimes.begin(), task.readyTimes.end(), 0);
        fill(task.finishTimes.begin(), task.finishTimes.end(), 0);
        task.startingTime = 0;
    }
}

void initializeSchedulingTasks(stack<int> &toScheduleTasks, vector<int> &temporaryVectorF, vector<int> &temporaryVectorS, const vector<Task> &tasks, const vector<vector<int>> & currentTask, int tasksNumber, int coresNumber) {
    // Create and initialize the temporaryVectorF vector
    temporaryVectorF.push_back(1); // Dummy task 0
    for (int i = 1; i <= tasksNumber; ++i) {
        temporaryVectorF.push_back(static_cast<int>(tasks[i].preID.size()));
    }

    // Create and initialize the temporaryVectorS vector
    temporaryVectorS.assign(tasksNumber + 1, 1);
    for (int i = 0; i <= coresNumber; ++i) {
        if (!currentTask[i].empty()) {
            temporaryVectorS[currentTask[i][0]] = 0;
        }
    }

    // Create and initialize the toScheduleTasks stack
    for (int i = 0; i <= tasksNumber; ++i) {
        if (temporaryVectorF[i] == 0 && temporaryVectorS[i] == 0) {
            toScheduleTasks.push(i);
            temporaryVectorF[i] = temporaryVectorS[i] = -1; // Prevent adding the same task again
        }
    }
}


void processSchedulingQueue(vector<vector<int>> &schedulingQueue, stack<int> &toScheduleTasks, vector<Task> &copyTasks, vector<int> &temporaryVectorF, vector<int> &temporaryVectorS, vector<vector<int>> & currentTask, int tasksNumber, int coresNumber) {
    // Initialize the scheduling queue
    schedulingQueue.resize(coresNumber + 1);

    // Process the tasks in the toScheduleTasks stack
    while (!toScheduleTasks.empty()) {
        Task &task = copyTasks[toScheduleTasks.top()];
        toScheduleTasks.pop();

        int k = task.assignedCore;
        if (k == 0) {
            // Compute the timing for cloud execution
            computeCloudTimes(copyTasks, task, schedulingQueue[k]);
        } else {
            // Compute the timing for core execution
            computeCoreTimes(copyTasks, task, schedulingQueue[k], k - 1);
        }
        schedulingQueue[k].push_back(task.id);

        // Update readiness of successor tasks
        for (int i : task.succID) {
            --temporaryVectorF[i];
        }

        // Remove the task from the front of its assigned core's scheduling queue
        currentTask[k].erase(currentTask[k].begin());

        // Update readiness of other tasks waiting for the same core
        if (!currentTask[k].empty()) {
            temporaryVectorS[currentTask[k][0]] = 0;
        }

        // Push ready tasks to the toScheduleTasks stack
        for (int i = 1; i <= tasksNumber; ++i) {
            if (temporaryVectorF[i] == 0 && temporaryVectorS[i] == 0) {
                toScheduleTasks.push(i);
                temporaryVectorF[i] = temporaryVectorS[i] = -1; // Prevent adding the same task again
            }
        }
    }
}

void computeTotalEnergyAndTime(const vector<Task>& tasks, const vector<int>& wirelessQueue, 
                               const vector<vector<int>>& schedulingQueue, double &energyTotal, int &timeTotal) {
    // Calculate the total energy consumption based on task scheduling
    energyTotal = computeTotalEnergy(tasks, wirelessQueue, schedulingQueue);
    
    // Calculate the total time based on task scheduling
    timeTotal = computeTotalTime(tasks);
}


vector<Task> reschedule(int targetTask, int targetCore, double &energyTotal, int &timeTotal, vector<vector<int>> currentTask,
                      vector<vector<vector<int>>> &schedulingQueues) {
    // Get the original core assignment for the target task
    int originalCore = tasks[targetTask].assignedCore;
    
    // If the target task is already assigned to the target core, no rescheduling is needed
    if (originalCore == targetCore) { 
      schedulingQueues.push_back(currentTask);
      return tasks;
    }
    
    // Reset energy and time totals
    energyTotal = 0;
    timeTotal = 0;
    
    // Remove the target task from its original core
    auto iter = currentTask[originalCore].begin();
    removeTaskFromCore(currentTask, originalCore, targetTask);
    
    // Add the target task to the new core using the addTaskToCore function
    addTaskToCore(currentTask, targetCore, targetTask, tasks);
    
    // Create a copy of the tasks and reset their scheduling
    vector<Task> copyTasks = tasks;
    resetTaskScheduling(copyTasks);
    
    // Update the assigned core for the target task in the copy
    copyTasks[targetTask].assignedCore = targetCore;

    // Initialize scheduling variables, including scheduling queues and ready lists
    vector<vector<int>> schedulingQueue;
    vector<int> temporaryVectorF, temporaryVectorS;
    stack<int> toScheduleTasks;

    // Initialize scheduling variables by calling the initializeSchedulingTasks function
    initializeSchedulingTasks(toScheduleTasks, temporaryVectorF, temporaryVectorS, tasks, currentTask, tasksNumber, coresNumber);
    
    // Process the scheduling queue by calling the processSchedulingQueue function
    processSchedulingQueue(schedulingQueue, toScheduleTasks, copyTasks, temporaryVectorF, temporaryVectorS, currentTask, tasksNumber, coresNumber);
  
    // Store the scheduling information in the schedulingQueues vector
    schedulingQueues.push_back(schedulingQueue);
    
    // Extract the wireless queue and remove it from the scheduling queue
    vector<int> wirelessQueue = schedulingQueue[0];
    schedulingQueue.erase(schedulingQueue.begin());
    
    // Calculate the total energy consumption and time
    computeTotalEnergyAndTime(copyTasks, wirelessQueue, schedulingQueue, energyTotal, timeTotal);
    
    // Return the updated copyTasks vector
    return copyTasks;
}

void identifyExitTasks(const vector<Task>& tasks, vector<int>& exitTasks) {
    // Iterate through the tasks vector
    for (const Task& task : tasks) {
        // Check if the task has no successors (an empty succID vector)
        if (task.succID.empty()) {
            // If the task has no successors, add its ID to the exitTasks vector
            exitTasks.push_back(task.id);
        }
    }
}



void assignTasksAndCalculateWorkload(vector<Task>& tasks, const vector<int>& cloudTimes, int coresNumber) {
    int totalTimeCloudExecution = cloudTimes[0] + cloudTimes[1] + cloudTimes[2]; // Total time for cloud execution

    for (Task& task : tasks) {
        int totalTimeLocalExecution = numeric_limits<int>::max();
        
        // Find the minimum of local execution times for the task
        for (int executionTime : task.localExecutions) {
            totalTimeLocalExecution = min(totalTimeLocalExecution, executionTime);
        }

        if (totalTimeCloudExecution < totalTimeLocalExecution) {
            task.assignedCore = 0; // Assign the task to the cloud
            task.weight = totalTimeCloudExecution; // Set the workload for the task to the cloud execution time
        } else {
            // Calculate the total local execution time for the task
            int totalLocalExecutionTime = accumulate(task.localExecutions.begin(), task.localExecutions.end(), 0);
            
            // Calculate the workload for the task, distributing it equally among cores
            task.weight = static_cast<double>(totalLocalExecutionTime) / coresNumber;
        }
    }
}

vector<Task> sortTasksByPriority(const vector<Task>& tasks) {
    // Create a copy of the input vector to avoid modifying the original
    vector<Task> taskSorted(tasks);

    // Sort the copied vector in descending order based on task priorities
    sort(taskSorted.begin(), taskSorted.end(), [](const Task &Vector1, const Task &Vector2) {
        return Vector1.priority > Vector2.priority;
    });

    // Check if the taskSorted vector is not empty, and if so, remove the last element (empty task)
    if (!taskSorted.empty()) {
        taskSorted.pop_back(); // Remove the empty task
    }

    // Return the sorted and cleaned vector of tasks by priority
    return taskSorted;
}


void scheduleTask(Task& task, const vector<Task>& tasks, vector<int>& wirelessSendQueue, vector<vector<int>>& cores, int coresNumber) {
    int assignedCore = -1;
    int MinimumFinalTime = numeric_limits<int>::max();

    // Calculate the best time for local cores
    vector<Task> CopyTaskCore(coresNumber, task);
    for (int j = 0; j < coresNumber; ++j) {
        computeCoreTimes(tasks, CopyTaskCore[j], cores[j], j);
        if (CopyTaskCore[j].finishTimes[0] < MinimumFinalTime) {
            MinimumFinalTime = CopyTaskCore[j].finishTimes[0];
            assignedCore = j + 1;
        }
    }

    // Calculate timing for the cloud
    Task CopyTaskWireless = task;
    computeCloudTimes(tasks, CopyTaskWireless, wirelessSendQueue);
    if (CopyTaskWireless.finishTimes[3] < MinimumFinalTime) {
        MinimumFinalTime = CopyTaskWireless.finishTimes[3];
        assignedCore = 0; // cloud
    }

    // Assign the task to the most suitable core and update timing
    if (assignedCore == 0) {
        task = CopyTaskWireless;
    } else {
        task = CopyTaskCore[assignedCore - 1];
    }
    task.assignedCore = assignedCore;

    // Add the task to the queue of the assigned core
    if (assignedCore == 0) {
        wirelessSendQueue.push_back(task.id);
    } else {
        cores[assignedCore - 1].push_back(task.id);
    }
}

void initialAssignment(vector<Task>& tasks, const vector<int>& cloudTimes, int coresNumber, vector<int>& wirelessSendQueue, vector<vector<int>>& cores) {
    // Assign tasks to cores and calculate workload
    assignTasksAndCalculateWorkload(tasks, cloudTimes, coresNumber);

    // Update priorities for all tasks
    for (int i = 1; i <= tasksNumber; ++i) {
        updatePriority(tasks[i]);
    }

    // Sort tasks by priority (descending order)
    vector<Task> taskSorted = sortTasksByPriority(tasks);

    // Schedule tasks based on priority
    for (int i = 0; i < tasksNumber; ++i) {
        scheduleTask(tasks[taskSorted[i].id], tasks, wirelessSendQueue, cores, coresNumber);
    }
}




int main() {
    using namespace std::chrono;
    
    // Initial time start
    auto initialTimeStart = high_resolution_clock::now();
    
    // Initialize tasks and their properties
    tasks = initializeTasks();
    
    // Initialize timing information for tasks
    initializeTime(tasks);
    
    // Identify exit tasks (tasks without successors)
    identifyExitTasks(tasks, exitTasks);

    // Variables to store initial and final values
    double initialEnergyTotal, finalEnergyTotal;
    int initialTimeTotal, finalTimeTotal;
    
    // Perform initial task assignments to cores and calculate initial energy and time
    initialAssignment(tasks, cloudTimes, coresNumber, wirelessSendQueue, cores);
    initialEnergyTotal = computeTotalEnergy(tasks, wirelessSendQueue, cores);
    initialTimeTotal = computeTotalTime(tasks);
    double energyTotal = computeTotalEnergy(tasks, wirelessSendQueue, cores);
    int timeTotal = computeTotalTime(tasks);

    auto initialTimeStop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(initialTimeStop - initialTimeStart);

    // Print the initial assignment table
    cout << "------------------------------------ Initial Scheduling Table  ------------------------------------ \n";
    printAssignment(tasks, wirelessSendQueue, cores, energyTotal, timeTotal);
    // Print Time
    cout << "Execution Time of My Algorithm for Initial Scheduling: " << duration.count() << " microsecond.\n" << endl;
    
    // Optimize task assignments iteratively
    double T_max = 1.5 * timeTotal; // Maximum allowable time increase
    vector<Task> taskSorted = sortTasksByPriority(tasks);
    
    // Final time start
    auto finalTimeStart = high_resolution_clock::now();
    for (int run = 0; run < 1; ++run) { // Perform one optimization run
        for (const Task &task : taskSorted) {
            // Data structures to store results and decisions
            vector<double> energies;
            vector<int> times;
            vector<vector<Task>> assignments;
            vector<vector<vector<int>>> schedulingQueues;

            // Create a vector currentTask to represent current task assignments on cores, including the wireless queue
            vector<vector<int>> currentTask(coresNumber + 1);
            currentTask[0] = wirelessSendQueue;
            for (int i = 1; i <= coresNumber; ++i) {
                currentTask[i] = cores[i - 1];
            }

            // Try reassigning the task to different cores and record the results
            for (int targetCore = 0; targetCore <= coresNumber; ++targetCore) {
                double testingEnergyTotal = energyTotal; // Energy total for the assignment attempt
                int testingTimeTotal = timeTotal;      // Time total for the assignment attempt

                // Calculate the assignment to a new core and record the results
                assignments.push_back(reschedule(task.id, targetCore, testingEnergyTotal, testingTimeTotal, currentTask, schedulingQueues));
                energies.push_back(testingEnergyTotal);
                times.push_back(testingTimeTotal);
            }

            // Select the best core assignment based on time and energy criteria
            int assignmentBest = tasks[task.id].assignedCore; // Best assigned core
            double best_energy = energyTotal; // Best energy consumption

            // Iterate over all core assignments
            for (int targetCore = 0; targetCore <= coresNumber; ++targetCore) {
                // If the assignment is valid and results in lower energy consumption, update the selection
                if (times[targetCore] <= timeTotal && energies[targetCore] < best_energy) {
                    assignmentBest = targetCore;
                    best_energy = energies[targetCore];
                }
            }

            // If the best assignment is the same as the current assignment
            if (assignmentBest == tasks[task.id].assignedCore) {
                double best_ratio = 0;

                // Iterate over all core assignments again
                for (int targetCore = 0; targetCore <= coresNumber; ++targetCore) {
                    // Skip the current assignment
                    if (targetCore == tasks[task.id].assignedCore) {
                        continue;
                    }
                    // If the assignment is valid and results in lower energy consumption, update the selection based on energy reduction ratio
                    if (times[targetCore] <= T_max && energies[targetCore] < energyTotal) {
                        double ratioOfEnergyReduction = (energyTotal - energies[targetCore]) / (times[targetCore] - timeTotal);
                        if (ratioOfEnergyReduction > best_ratio) {
                            assignmentBest = targetCore;
                            best_ratio = ratioOfEnergyReduction;
                        }
                    }
                }
            }

            // If a change in assignment is needed, update the tasks and scheduling queues
            if (assignmentBest != tasks[task.id].assignedCore) {
                tasks = assignments[assignmentBest];
                wirelessSendQueue = schedulingQueues[assignmentBest][0];
                schedulingQueues[assignmentBest].erase(schedulingQueues[assignmentBest].begin());
                cores = schedulingQueues[assignmentBest];
                energyTotal = energies[assignmentBest];
                timeTotal = times[assignmentBest];
            }
        }
    }

    // Store final energy and time after optimization
    finalEnergyTotal = computeTotalEnergy(tasks, wirelessSendQueue, cores);
    finalTimeTotal = computeTotalTime(tasks);
    
    auto finalTimeStop = high_resolution_clock::now();
    auto finalDuration = duration_cast<microseconds>(finalTimeStop - finalTimeStart);
    // Print the final assignment table
    cout << "------------------------------------- Final Assignment Table  ------------------------------------- \n";
    printAssignment(tasks, wirelessSendQueue, cores, energyTotal, timeTotal);
    
    // Print Time
    cout << "Execution Time of My Algorithm for Final Scheduling: " << finalDuration.count() << " microsecond.\n" << endl;
    
    // Print comparison table
    cout << "----------- Energy-Time Table ---------- \n";
    cout << left << setw(20) << "Metric" << setw(10) << "Initial" << setw(10) << "Final" << endl;
    cout << string(40, '-') << endl;
    cout << left << setw(20) << "Total Energy" << setw(10) << initialEnergyTotal << setw(10) << finalEnergyTotal << endl;
    cout << left << setw(20) << "Total Time" << setw(10) << initialTimeTotal << setw(10) << finalTimeTotal << endl;
    cout << "  " << endl;

}
// DONE
