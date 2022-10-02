// Copyright 2021 Teodora Stroe
#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_

#include "server.h"

struct load_balancer;
typedef struct load_balancer load_balancer;

load_balancer* init_load_balancer();

void loader_add_server(load_balancer* main, int server_id);

void loader_store(load_balancer* main, char* key, char* value, int* server_id);

char* loader_retrieve(load_balancer* main, char* key, int* server_id);

void loader_remove_server(load_balancer* main, int server_id);

void free_load_balancer(load_balancer* main);

#endif  // LOAD_BALANCER_H_
