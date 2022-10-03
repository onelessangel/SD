Copyright Teodora Stroe 311CA 2021

# SD - Data Structures

## The Galactic War

A console game simulating a galactic war between planets found in the same galaxy, focusing 
on the usage of Circular Doubly Linked Lists.

The player can give the following commands:

- add a planet;
- black hole it;
- collision between 2 planets;
- expand shield;
- remove shield;
- rotate planet;
- show details about planet.

## Load Balancer

A load balancer implementation, having a Hashtable based system.

It uses Consistent Hashing in order to fulfill the _minimal disruption contraint_.

The performance of the project has been improved by **20%** by using multiple server 
replicas in order to distribute products evenly.

The program can perform the following operations:

- initialize the load balancer;
- add a new server to the system;
- store the key-value pair inside the system;
- get a value associated with the key and return it;
- remove a specific server from the system.

The system can efficiently handle over 20,000+ commands with various products and servers.

## Rope Data Structure

An implementation of various operations using the **Rope** data structure, used in real text editors.

The Rope data structure is a 2-properties driven binary tree that allows efficient 
operations on very long strings (a problem common to text editors of any kind).

The following operations have been implemented:

- concatenation of two rope trees;
- finding a character located at given index in rope tree;
- spliting of rope tree at the provided split point;
- insertion of a new string in the rope tree;
- deletion of a string in given interval from rope tree.
