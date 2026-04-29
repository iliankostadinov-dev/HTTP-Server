# HTTP Server in C

A lightweight high-performance HTTP/1.1 server implemented in C using an event-driven architecture.

The server is built around an **epoll-based event loop** combined with a **thread pool model**, enabling efficient handling of multiple concurrent client connections.

It supports **non-blocking I/O**, **persistent connections (keep-alive)**, and serves **static files** from a local directory.

---

## Features

- Event-driven architecture using epoll
- Non-blocking socket I/O
- Thread pool for concurrent request handling
- HTTP GET request support
- Keep-alive connection handling
- Static file server (HTML)
- Basic MIME type detection

---

## Architecture

The system is based on a hybrid model:

epoll event loop → task queue → worker threads → request handler → response

---

## Technologies

- C (POSIX sockets)
- epoll (Linux I/O multiplexing)
- pthread (multithreading)
- TCP/IP networking

---

## Build & Run

### Build project
```bash
make
```

### Run server
```bash
make run
```

### Clean build files
```bash
make clean
```

### Rebuild project
```bash
make rebuild
```

## 📌 Purpose

This project demonstrates low-level system programming, network programming, and scalable server design principles.
