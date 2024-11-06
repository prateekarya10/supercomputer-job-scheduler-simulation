# 📅 Job Scheduler Project

## 🎯 Overview

This project simulates a job scheduler for a supercomputer with 128 worker nodes and a master scheduler node. It allocates resources and schedules jobs using various queuing and worker node selection policies, aiming to optimize resource utilization.

## ✨ Key Features

- Supports 128 worker nodes, each with 24 cores and 64 GB of RAM.
- Implements three job queuing policies: FCFS, smallest job first, and short duration job first.
- Allocates jobs based on three node selection policies: first fit, best fit, and worst fit.
- Collects and analyzes data on CPU and memory utilization per policy combination.
- Generates CSV files and visual graphs to display performance metrics.

## 📊 Results Highlight

The scheduler simulation provides insight into the effectiveness of different queuing and node allocation policies:

1. Resource Utilization: Analyzes CPU and memory usage across various job scheduling policies.
2. Job Distribution Efficiency: Measures allocation success rates under different queuing approaches.
3. Performance Comparison: Highlights the impact of each policy combination on overall efficiency.

## 🛠️ Technical Implementation

 ### Core Components
 - Job Struct: Stores job details like arrival time, required cores, memory, and execution time.
 - Scheduler Class: Manages job queue and worker nodes, implements scheduling and allocation policies.
 - Resource Allocation Logic: Checks worker node resources to determine the best fit based on selected policies.
 ### Technology Stack
 - Language: C++
 - Data Format: Input/output in CSV for resource utilization statistics.
 - Analysis: Statistical methods for performance evaluation, outputting metrics as bar graphs.

## 📈 Performance

This simulation processes job queues and resource allocation efficiently:

- 128 worker nodes managed simultaneously.
- Calculates resource usage metrics on a per-policy basis.
- Outputs average daily CPU and memory utilization for each policy combination.
## 🚀 Getting Started

1. Clone the repository
2. Ensure job data is provided in the specified input format.
3. Compile the C++ source code.
4. Run the executable to simulate scheduling.
5. Access generated CSV files and visualizations in the output directory.

## 📝 Output Files

The program generates these files for analysis:

- resource_utilization.csv: Average daily CPU and memory usage for each policy combination.
- performance_graphs.png: Graphical representation of resource utilization metrics.

## 🔍 Future Scope

- Advanced Scheduling Algorithms: Explore machine learning-based scheduling approaches.
- Scalability Testing: Adapt for larger supercomputing environments.
- Dynamic Resource Management: Implement dynamic allocation based on real-time conditions.
- Cross-Platform Compatibility: Extend support for different OS architectures.

## 💡 Technical Note

The simulation runs as a single-threaded application to ensure consistency in job scheduling behavior. Future versions may incorporate multi-threading to improve performance on larger workloads.

## 🤝 Acknowledgments

Gratitude to:

- Project Mentors for their valuable guidance and insights.
- Collaborators who contributed ideas and support throughout development.
- Open-Source Community for inspiring solutions and sharing resources.
- Users and Testers who provided feedback to refine the scheduler’s functionality.



Created with ❤️ by a passionate programmer

Submittion from - ##IITGCS-24061107 (prateekarya10 / Prateek Arya)
