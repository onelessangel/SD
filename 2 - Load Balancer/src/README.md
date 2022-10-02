Copyright Teodora Stroe 311CA 2021

### The load balancer retains:

- the hashring (replica_array)
- the number of replicas (elements) currently in it
- how much memory has been allocated for the hashring

### A replica retains:

- the server ID
- the replica hash
- the replica tag
- a pointer to the server

### A server retains:

- the associated hashtable

### init_load_balancer -- initializes the load balancer

- initializes the hashring
- starts the allocated memory count


### loader_add_server -- adds a new server to the system

- if necessary, increases the size of the replica array
- computes each replica's hash and adds it in the right place on the hashring
- for each replica, retains it's next neighbour and moves objects from said
	  neighbours to the right server


### loader_store -- stores the key-value pair inside the system

- retains the index of the correct server replica using a modified binary search
- adds the key-value pair to the right server


### loader_retrieve -- gets a value associated with the key and returns it

- retains the index of the correct server replica using a modified binary search
- searches the server for the key and retrieves the associated value


### loader_remove_server -- removes a specific server from the system

- eliminates the removed server's replicas from the replica array
- redistributes the objects from the removed server


### server_store -- stores a key-value pair to the server

### server_retrieve -- gets the value associated with the key

### server_remove -- removes a key-pair value from the server