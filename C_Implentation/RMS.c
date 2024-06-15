#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    char name[10];
    int period;
    int computation_time;
    int remaining_time;
    int next_deadline;
} Task;

void simulate_rms(Task tasks[], int num_tasks, int total_time) {
    int clock = 0;

    while (clock < total_time) {
        int highest_priority_task_index = -1;

        // Find the highest priority task that is ready to run
        for (int i = 0; i < num_tasks; i++) {
            if (clock >= tasks[i].next_deadline - tasks[i].period && (highest_priority_task_index == -1 || tasks[i].period < tasks[highest_priority_task_index].period)) {
                highest_priority_task_index = i;
            }
        }

        // If no task is ready to run, increment the clock
        if (highest_priority_task_index == -1) {
            printf("Clock: %d, Idle\n", clock);
            clock++;
            continue;
        }

        Task* current_task = &tasks[highest_priority_task_index];
        printf("Clock: %d, Running: %s\n", clock, current_task->name);

        // Run the current task for one unit of time
        sleep(1);  // Simulate time passing
        current_task->remaining_time--;
        clock++;

        // If the task is finished, update its next deadline and reset remaining time
        if (current_task->remaining_time == 0) {
            current_task->remaining_time = current_task->computation_time;
            current_task->next_deadline += current_task->period;
            printf("%s completed its execution. Next deadline: %d\n", current_task->name, current_task->next_deadline);
        }

        // Check if any task has missed its deadline
        for (int i = 0; i < num_tasks; i++) {
            if (tasks[i].next_deadline <= clock && tasks[i].remaining_time > 0) {
                printf("Deadline Missed: %s\n", tasks[i].name);
                return;
            }
        }

        // Re-evaluate task readiness
        for (int i = 0; i < num_tasks; i++) {
            if (clock % tasks[i].period == 0) {
                tasks[i].remaining_time = tasks[i].computation_time;
                tasks[i].next_deadline = clock + tasks[i].period;
                printf("%s is reset with remaining time %d and next deadline %d\n", tasks[i].name, tasks[i].remaining_time, tasks[i].next_deadline);
            }
        }
    }
}

int main() {
    Task tasks[] = {
        {"Task1", 10, 3, 3, 10},
        {"Task2", 20, 4, 4, 20},
        {"Task3", 30, 5, 5, 30}
    };

    int num_tasks = sizeof(tasks) / sizeof(tasks[0]);
    simulate_rms(tasks, num_tasks, 60);

    return 0;
}
