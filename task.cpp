#include <bits/stdc++.h>

using namespace std;

bool is_file_empty(fstream& task_file)
{
    return task_file.peek() == fstream::traits_type::eof();
}

//help Function for the describing the functionality of program
void help()
{
    cout<<"Usage :-"<<endl;
    cout<<"$ ./task add 2 hello world    # Add a new item with priority 2 and text \"hello world\" to the list"<<endl;
    cout<<"$ ./task ls                   # Show incomplete priority list items sorted by priority in ascending order"<<endl;
    cout<<"$ ./task del INDEX            # Delete the incomplete item with the given index"<<endl;
    cout<<"$ ./task done INDEX           # Mark the incomplete item with the given index as complete"<<endl;
    cout<<"$ ./task help                 # Show usage"<<endl;
    cout<<"$ ./task report               # Statistics";
}


//List all the pending items
void list_task()
{
    fstream task_file("task.txt");
    if(!is_file_empty(task_file))
    {
        map<int, string> priority_task;
        string statement;
        while(getline(task_file, statement))
        {
            priority_task[(int)(statement[0]-'0')] = statement.substr(2,statement.size());
        }   
        int index=1;
        for(auto i : priority_task) {cout<<index++<<". "<<i.second<<" ["<<i.first<<"]\n";}  
    }
    else cout<<"There are no pending tasks!\n";
    task_file.close();
}


//Add a new item
void add_task(int check, char* new_task[])
{
    fstream task_file("task.txt", ios::app);
    if(task_file) 
    {
        if(check==4)
        {
            task_file << new_task[2] << " " << new_task[3] << "\n";
            task_file.close();
            cout<<"Added task: \""<<new_task[3]<<"\" with priority "<<new_task[2]<<"\n";
        }
        else {cout<<"Error: Missing tasks string. Nothing added!";}
    } 
    task_file.close();   
}


//Delete an item
void del_task(int check, char* del_index[], bool message)
{
    if(check != 3) 
    {
        cout<<"Error: Missing NUMBER for deleting tasks."<<endl;
        return;
    }
    ifstream task_file("task.txt");
    ofstream temp_file("temp.txt");
    
    int index=atoi(del_index[2]);
    int given_index=index;
    bool flag=false;
    if(index<=0) flag=true;
    string statement;
    while(getline(task_file, statement))
    {
        if(--index!=0) temp_file << statement <<"\n";
    }   
    task_file.close();
    temp_file.close();
    remove("task.txt");
    rename("temp.txt", "task.txt");
    if(index>0 || flag) cout<<"Error: task with index #"<<given_index<<" does not exist. Nothing deleted."<<endl;
    else if(message) cout<<"Deleted task #"<<given_index<<endl;
    else return;
}


//Mark a task as completed
void done_task(int check, char* done_index[])
{
    if(check != 3) 
    {
        cout<<"Error: Missing NUMBER for marking tasks as done."<<endl;
        return;
    }
    ifstream task_file("task.txt");
    ofstream completed_file("completed.txt", ios::app);
    
    int index=atoi(done_index[2]);
    int given_index=index;
    bool flag=false;
    if(index<=0) flag=true;
    string statement;
    while(getline(task_file, statement))
    {
        if(--index==0) completed_file << statement.substr(2,statement.size()) <<"\n";
    }   
    task_file.close();
    completed_file.close();
    del_task(3, done_index, false);
    if(index>0 || flag) cout<<"Error: no incomplete item with index #"<<given_index<<" exists."<<endl;
    else cout<<"Marked item as done."<<endl;
}


//Generate a report
void report_task()
{
    int pending=0, completed=0;
    ifstream task_file("task.txt");
    ifstream completed_file("completed.txt");
    string statement;
    while(getline(task_file, statement))    {pending++;}
    while(getline(completed_file, statement))   {completed++;}
    cout<<"Pending : "<<pending<<endl;
    list_task();
    cout<<endl;
    cout<<"Completed : "<<completed<<endl;
    task_file.close();
    completed_file.close();
    pending=1;
    completed_file.open("completed.txt");
    while(getline(completed_file, statement)) 
    {cout<<pending++<<". "<<statement<<endl;}
}


int main(int argc, char* argv[])
{
    if(argc > 1)
    {
        //help() route
        if(strcmp(argv[1], "help")==0) {help();}
        //List() route
        else if(strcmp(argv[1], "ls")==0) {list_task();}
        //add_task() route
        else if(strcmp(argv[1], "add")==0) {add_task(argc, argv);}
        //delete_task() route
        else if(strcmp(argv[1], "del")==0) {del_task(argc, argv, true);}
        //done_task() route
        else if(strcmp(argv[1], "done")==0) {done_task(argc, argv);}
        //reoport_task() route
        else if(strcmp(argv[1], "report")==0) {report_task();}
        //error handling route
        else cout<<"Undefined Action"<<endl;
    }
    else {help();}
    return 0;
}
