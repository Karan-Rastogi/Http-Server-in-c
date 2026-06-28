# Simple C HTTP Server

A minimal, single-connection HTTP server written in C using POSIX sockets. It listens on a TCP port, accepts one incoming connection, checks whether the request is an HTTP `GET`, and responds with a small hardcoded HTML page.

This project is intended as a learning exercise for understanding raw socket programming (`socket`, `bind`, `listen`, `accept`, `recv`, `send`) rather than as a production-ready web server.

## Features

- Creates a TCP socket and binds it to port `8080`
- Uses `SO_REUSEADDR` so the port can be reused quickly after restarts
- Accepts a single client connection
- Parses the first bytes of the request to detect a `GET` method
- Sends back a basic `HTTP/1.1 200 OK` response with a hardcoded HTML body
- Rejects (and ignores) any non-`GET` request

## Limitations

- Handles **only one request** before shutting down (no loop around `accept`)
- Does not serve actual files — the response body is hardcoded in the source
- No support for other HTTP methods (`POST`, `HEAD`, etc.)
- No concurrency — cannot serve multiple clients at once
- Minimal error handling and no request parsing beyond the method check

## Requirements

- A POSIX-compliant system (Linux, macOS, WSL, etc.)
- `gcc` or any C compiler with standard POSIX networking headers

## Building

```bash
gcc -o server server.c
```

## Running

```bash
./server
```

The server will start listening on port `8080` and wait for a single connection.

## Testing

In another terminal, send a request using `curl`:

```bash
curl http://localhost:8080
```

Or open the address in a browser:

```
http://localhost:8080
```

You should see a small HTML page rendered, and the server will print logging info to its terminal before exiting.

## Project Structure

```
.
├── server.c     # Main server source file
└── README.md
```

## Possible Improvements

- Wrap `accept()` in a loop to handle multiple sequential connections
- Fork or use threads for concurrent client handling
- Serve actual files from disk (e.g. `index.html`) instead of a hardcoded string
- Add proper HTTP request parsing (headers, methods, paths)
- Add error handling for `getaddrinfo()` and other calls that currently ignore return values

## License

See [LICENSE](LICENSE) (MIT).
