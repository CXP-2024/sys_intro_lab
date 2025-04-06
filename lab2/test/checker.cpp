#include <cstdio>
#include <unordered_map>
#include <vector>
#include <cstdlib>

using namespace std;

unordered_map<int, int> M, N;
unordered_map<int, vector<int> > Data;


void check(const char * answer_file, const char * output_file){
    // check the answer correct or not
    FILE* answer = fopen(answer_file, "r");
    FILE* output = fopen(output_file, "r");
    if(answer == NULL || output == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    int task_id, m, n;
    int cnt = 0;
    while(fscanf(answer, "%d%d%d", &task_id, &m, &n) == 3){
        M[task_id] = m;
        N[task_id] = n;
        vector<int> data;
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                int x;
                fscanf(answer, "%d", &x);
                data.push_back(x);
            }
        }
        Data[task_id] = data;
        cnt ++;
    }
    int cnt_y = 0;
    while(fscanf(output, "%d%d%d", &task_id, &m, &n) == 3){
        if(M.find(task_id) == M.end()){
            printf("Error: task_id %d not found\n", task_id);
            exit(1);
        }
        if(M[task_id] != m || N[task_id] != n){
            printf("Error: task_id %d, m = %d, n = %d\n", task_id, m, n);
            exit(1);
        }
        vector<int> data;
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                int x;
                fscanf(output, "%d", &x);
                data.push_back(x);
            }
        }
        if(Data[task_id] != data){
            printf("Error: task_id %d, data error", task_id);
            bool flag = true;
            for(int i = 0; i < m && flag; i++){
                for(int j = 0; j < n && flag; j++){
                    if(Data[task_id][i * n + j] != data[i * n + j]){
                        printf("task_id=%d i=%d j=%d answer=%d output=%d\n", task_id, i, j, Data[task_id][i * n + j], data[i * n + j]);
                        flag = false;
                    }
                }
            }
            exit(1);
        }
        cnt_y++;
    }
    if(cnt != cnt_y){
        printf("Error: only %d tasks\n", cnt_y);
        exit(1);
    }
    printf("checker: ok, %d cases\n", cnt);
}


int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Usage: %s <answer_file> <output_file>\n", argv[0]);
        return -1;
    }
    check(argv[1], argv[2]);
    return 0;
}