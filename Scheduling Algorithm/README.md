# Scheduling Algorithms in Operating Systems

## Introduction
This directory contains implementations of four classic scheduling algorithms in operating systems: First-Come First-Served (FCFS), Shortest Job Next (SJN), Priority Scheduling, and Round Robin. Each algorithm offers a unique approach to managing process execution order and time in an operating system's scheduler.

## Algorithms Overview

### First-Come First-Served (FCFS)
FCFS is the simplest scheduling algorithm that operates on a "first come, first serve" basis. The process that arrives first gets executed first, and subsequent processes wait in a queue. The implementation involves calculating waiting time and turnaround time for each process. FCFS is easy to implement but can lead to poor performance if long processes block shorter ones, a problem known as the convoy effect.

### Shortest Job Next (SJN)
SJN, also known as Shortest Job First (SJF), selects the process with the shortest burst time (execution time) next for execution. This algorithm effectively reduces the average waiting time compared to FCFS, particularly for a batch system where executing jobs take different amounts of time. The implementation sorts processes based on their burst time and calculates their waiting and turnaround times.

### Priority Scheduling
Priority scheduling assigns a priority to each process, and the process with the highest priority (lowest integer value) is selected for execution. In case of equal priorities, FCFS rule is applied. This algorithm is practical for systems with diverse process requirements but can lead to issues like starvation, where lower priority processes may never execute. The implementation here sorts processes based on their priority and calculates their waiting and turnaround times.

### Round Robin
Round Robin is a preemptive scheduling algorithm that assigns a fixed time quantum to all processes. The scheduler goes through the queue, allocating the quantum to each process in turn. If a process's burst time is longer than the quantum, it is preempted and placed at the back of the queue. This method ensures a balance between response and waiting times but requires proper time quantum selection. The implementation involves cycling through processes and reducing their remaining time by the quantum until all processes are completed.

Each of these algorithms offers distinct advantages and trade-offs, and their applicability depends on the specific requirements and constraints of the operating system's environment. Understanding these algorithms provides foundational knowledge for designing and evaluating process scheduling mechanisms in operating systems.
