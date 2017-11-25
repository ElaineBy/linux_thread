#include "helper.h"
#include <cstdio>
#include <iostream>
using namespace std;

pthread_mutex_t mut_output = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut_input = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut_input_flag = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut_cnt = PTHREAD_MUTEX_INITIALIZER;

int cnt = 0;
int input_flag = 0;

/* global data structure*/
struct shit{
    int initial;
    int i1;
    int i2;
    int i3;
};

struct shit s;

void reset(){
    s.initial = 0;
    s.i1 = 0;
    s.i2 = 0;
    s.i3 = 0;
}


/* to simulate 3 algorithms :(*/

//a1 is to mutiply initial by 1
void* a1(void* args){
    //cout << "stuck here  a1" << endl;
    mulock(LOCK,&mut_input);
    s.i1 = s.initial * 1;
    //cout << "a1:" << s.i1 << endl;
    
    
    mulock(LOCK,&mut_cnt);
    cnt++;
    if(cnt == 3){
        mulock(UNLOCK,&mut_output);
    }
    mulock(UNLOCK,&mut_cnt);
    
    mulock(UNLOCK,&mut_input);
}

//a2 is to mutiply initial by 2
void* a2(void* args){
    //cout << "stuck here  a2" << endl;
    mulock(LOCK,&mut_input);
    s.i2 = s.initial * 2;
    //cout << "a2:" << s.i2 << endl;
    mulock(LOCK,&mut_cnt);
    cnt++;
    if(cnt == 3){
        mulock(UNLOCK,&mut_output);
    }
    mulock(UNLOCK,&mut_cnt);
    
    mulock(UNLOCK,&mut_input);
}

//a3 is to mutiply initial by 3
void* a3(void* args){
    //cout << "stuck here  a3" << endl;
    mulock(LOCK,&mut_input);
    s.i3 = s.initial * 3;
    //cout << "a3:" << s.i3 << endl;
    
    mulock(LOCK,&mut_cnt);
    cnt++;
    if(cnt == 3){
        mulock(UNLOCK,&mut_output);
    }
    mulock(UNLOCK,&mut_cnt);
    mulock(UNLOCK,&mut_input);
}

void* io(void* args){
    cin >> s.initial;
    //cout << s.initial << endl;
    
    mulock(UNLOCK,&mut_input);
    
    //cout << "stuck here  0" << endl;
    mulock(LOCK,&mut_output);
    
    cout << "a1:" << s.i1 << endl;
    cout << "a2:" << s.i2 << endl;
    cout << "a3:" << s.i3 << endl;

    mulock(LOCK,&mut_cnt);
    cout << "set counter 0" << endl;
    cnt = 0;
    mulock(UNLOCK,&mut_cnt);
    
    
    
    mulock(UNLOCK,&mut_output);
    
    
    
}

int main(){
    while(1){
    mulock(LOCK,&mut_input);
    mulock(LOCK,&mut_output);
    
    pthread_t thread_a1,thread_a2,thread_a3,thread_io;
    void* res_a1,*res_a2,*res_a3;
    
    if (pthread_create(&thread_io, NULL, &io, (void *)NULL) == -1){
        puts("fail to create pthread thread_a3");
        exit(1);
    }
    
    if (pthread_create(&thread_a1, NULL, &a1, (void *)NULL) == -1) {
        puts("fail to create pthread thread_a1");
        exit(1);
    }
    if (pthread_join(thread_a1,&res_a1) == -1){
        puts("fail to recollect thread_a1");
        exit(1);
    }
        
    if (pthread_create(&thread_a2, NULL, &a2, (void *)NULL) == -1) {
        puts("fail to create pthread thread_a2");
        exit(1);
    }
        
    if (pthread_join(thread_a2,&res_a2) == -1){
        puts("fail to recollect thread_a2");
        exit(1);
    }
        
    if (pthread_create(&thread_a3, NULL, &a3, (void *)NULL) == -1){
        puts("fail to create pthread thread_a3");
        exit(1);
    }
        
    if (pthread_join(thread_a3,&res_a3) == -1){
        puts("fail to recollect thread_a3");
        exit(1);
    }
    }
    
            

}
