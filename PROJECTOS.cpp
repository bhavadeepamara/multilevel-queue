#include<iostream>
using namespace std;
    int queue_index,end_tm,queue_tm,quan_tm_queue,quan_time_q1,
    quan_time_q2,quan_time_q3,noOfProcesses;
    struct process{
        int burst_tm;int priority;int rem_tm;int index;int compete_tm;
    }*processes;
    struct queue{
        process **pros;int size;int current_loc;
    }*queues;
    
    void put_data();
    void sort(bool ind);
    int remaning_time(int opt);
    void pro_add(int value);
    void pro_sel(int local_quan_tm);
    void process_data();
    void get_data();
main(){
  queue_index=0;end_tm=0;queue_tm=10;quan_time_q1=4;quan_time_q2=-1;quan_time_q3=-1;
  get_data();
}
void get_data(){
  cout<<"enter number of processes:";
  cin>>noOfProcesses;
  processes = new process[noOfProcesses];
  queues = new queue[3];
  queues[0].size=0;queues[1].size=0;queues[2].size=0;
  for (int i = 0; i < noOfProcesses; i++) {
    cout<<"enter burst time and priority of process "<<i+1<<":";
    cin>>processes[i].burst_tm>>processes[i].priority;
    processes[i].rem_tm=processes[i].burst_tm;
    processes[i].index=i+1;
    processes[i].compete_tm=0;
    if(processes[i].priority>10&&processes[i].priority<=20){
      queues[0].size++;
      queues[0].pros = new process*;
      queues[0].pros[queues[0].size-1] = &processes[i];
    }
    else if(processes[i].priority>20&&processes[i].priority<=30){
      queues[1].size++;
      queues[1].pros = new process*;
      queues[1].pros[queues[1].size-1] = &processes[i];
    }
    else if(processes[i].priority>30&&processes[i].priority<=40){
      queues[2].size++;
      queues[2].pros = new process*;
      queues[2].pros[queues[2].size-1] = &processes[i];
    }
    else{
      cout<<"invalid input"<<i;
      exit(0);
    }
  }
  queues[0].pros = new process*[(queues[0].size)];
  queues[1].pros = new process*[(queues[1].size)];
  queues[2].pros = new process*[(queues[2].size)];
  int q1loc=-1,q3loc=-1,q2loc=-1;
  for (int i = 0; i < noOfProcesses; i++) {
    if(processes[i].priority>10&&processes[i].priority<=20){
      q1loc++;
      queues[0].pros[q1loc] = &processes[i];
    }
    else if(processes[i].priority>20&&processes[i].priority<=30){
      q2loc++;
      queues[1].pros[q2loc] = &processes[i];
    }
    else if(processes[i].priority>30&&processes[i].priority<=40){
      q3loc++;
      queues[2].pros[q3loc] = &processes[i];
    }
  }
  sort(false);
  for (int j = 0; j < 3; j++) {
    for(int i=0; i<queues[j].size; i++){
      queues[j].current_loc=0;
    }
  }
  process_data();
}
void sort(bool ind){
  bool comp;
  for (int i = 0; i < queues[1].size; i++) {
    for (int j = 0; j < queues[1].size; j++) {
      if(ind)
      comp=(queues[1].pros[i]->index<queues[1].pros[j]->index);
      else
      comp=(queues[1].pros[i]->priority<queues[1].pros[j]->priority);
      if(comp){
        process *temp = new process;
        temp = queues[1].pros[i];
        queues[1].pros[i] = queues[1].pros[j];
        queues[1].pros[j] = temp;
        // delete temp;
      }
    }
  }
}
int remaning_time(int opt){
  int totalTM=0;
  if(opt<0)
    for (int i = 0; i < noOfProcesses; i++)
      totalTM+=processes[i].rem_tm;
  else
    for (int i = 0; i < queues[opt].size; i++)
      totalTM+=queues[opt].pros[i]->rem_tm;
  return totalTM;
}
void pro_add(int value){
  end_tm+=value;
  queues[queue_index].pros[queues[queue_index].current_loc]->compete_tm=end_tm;
  queues[queue_index].pros[queues[queue_index].current_loc]->rem_tm-=value;
  queue_tm-=value;
}
void pro_sel(int local_quan_tm){
  if(local_quan_tm<=0){
    if((queues[queue_index].pros[queues[queue_index].current_loc]->rem_tm)>=queue_tm)
      pro_add(queue_tm);
    else
      pro_add(queues[queue_index].pros[queues[queue_index].current_loc]->rem_tm);
  }
  else{
    if((queues[queue_index].pros[queues[queue_index].current_loc]->rem_tm)>=local_quan_tm){
      if(local_quan_tm>=queue_tm)
        pro_add(queue_tm);
      else{
        pro_add(local_quan_tm);
        if(queues[queue_index].current_loc<(queues[queue_index].size-1))
          queues[queue_index].current_loc++;
        else
          queues[queue_index].current_loc=0;
      }
    }
    else{
      if((queues[queue_index].pros[queues[queue_index].current_loc]->rem_tm)>=queue_tm)
        pro_add(queue_tm);
      else
        pro_add(queues[queue_index].pros[queues[queue_index].current_loc]->rem_tm);
    }
  }
}
void process_data(){
  while (remaning_time(-1)>0) {
    if(queue_index>=3)
      queue_index=0;
    while((queue_tm<=0)||(remaning_time(queue_index)<=0)){
      if(queue_index<2)
      queue_index++;
      else
      queue_index=0;
      queue_tm=10;
    }
    while((queues[queue_index].pros[queues[queue_index].current_loc]->rem_tm)<=0){
      if(queues[queue_index].current_loc<(queues[queue_index].size-1))
        queues[queue_index].current_loc++;
      else
        queues[queue_index].current_loc=0;
    }
    if(queues[queue_index].current_loc>=queues[queue_index].size)
      queues[queue_index].current_loc=0;
    if(queue_index==0)
      pro_sel(4);
    else
      pro_sel(-1);
  }
  sort(true);
  put_data();
}
void put_data(){
	cout<<"process name    burst time   priority    waiting time    turnaround time.\n";
  for (int i = 0; i < noOfProcesses; i++) {
    cout<<"P"<<processes[i].index<<"\t\t | "<<processes[i].burst_tm<<" \t\t| "<<processes[i].priority
    <<" \t\t| "<<processes[i].compete_tm-processes[i].burst_tm<<" \t\t| "<<processes[i].compete_tm<<"\n";
  }
}
