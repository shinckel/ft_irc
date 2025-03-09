# CPP-[projects from 05 to 09]
This project was developed for 42 school. For comprehensive information regarding the requirements, please consult the PDF file in the subject folder of the repository. Furthermore, I have provided my notes and a concise summary below.

```diff
+ keywords: internet relay chat
+ early real time chat communication
+ client server setup - run a client in your pc, that connects to a server. Everybody else connected to that server can communicate in real time.
```

## High-level Overview

- Chat server in C++ for real-time communication through direct messages and group channels. Built to the C++98 standard, it uses non-blocking I/O and a single poll() operation to handle multiple connections efficiently and without delays.

0. You must not develop an IRC client — **HexChat** was the chosen one.
1. You must not implement server-to-server communication.
2. Your executable will be run as follows: `./ircserv <port> <password>`

## Concepts

| Task | Prototype | Description |
|:----|:-----:|:--------|
| **HexChat** | `ubuntu VM` `sudo apt update` `sudo apt install hexchat` | HexChat will try to open a graphical user interface (GUI), therefore, VM must be initialized with X11 forwarding enabled — install XQuartz, then enable X11 forwarding in SSH `ssh -X ubuntu@your_vm_ip`. Run `hexchat`. |


### References
[Internet Relay Chat: Communicate Outside Of Normal Channels](https://www.youtube.com/watch?v=FxZ2epcJ9l0)<br />
