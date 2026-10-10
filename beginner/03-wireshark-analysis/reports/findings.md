# Wireshark Traffic Analysis — Findings Report

**Status:** Completed  
**Started:** October 9, 2026  
**Completed:** October 9, 2026

## Executive summary

This lab documented packet analysis in an Ubuntu VM using synthetic HTTP and a DNS lookup for example.com. The HTTP evidence verifies a TCP three-way handshake, a GET request, a 200 OK response, and a readable synthetic page. DNS evidence matches a query and response using transaction ID 0x43bf and identifies two IPv4 answers. Filtered protocol statistics summarize four DNS packets. The Python lab server was stopped with Ctrl+C.

## Environment and scope

| Item | Recorded value |
|---|---|
| Environment | Ubuntu desktop VM in VirtualBox; exact OS release not independently shown in this project's evidence |
| Wireshark | 3.2.3, packaged 3.2.3-1 |
| dnsutils | 1:9.18.30-0ubuntu0.20.04.2 |
| Working directory | ~/portfolio-labs/traffic |
| Synthetic page | index.html containing Synthetic HTTP lab page and a newline |
| HTTP service | Python server bound to 127.0.0.1:8000 |
| Advertised versions | HTTP Server header: SimpleHTTP/0.6 Python/3.8.5; request User-Agent: curl/7.68.0; independent version commands not recorded |
| Capture interface | Loopback: lo for HTTP and the observed local DNS resolver exchange |
| Account display | Setup uses customized tsedeneya-bayew@VM prompt; server terminal shows seed@VM |
| Authorized scope | Own VM, synthetic local HTTP, and example.com DNS lookup; no third-party scanning or personal browsing |

## Evidence and outcomes

| Step | Work documented | Actual outcome | Evidence |
|---|---|---|---|
| 1 | Install tools and prepare synthetic page | dnsutils configured; directory and page creation commands show no visible errors | [Setup](../screenshots/01-setup.png) |
| 1 | Inspect capture interfaces | Wireshark opens and lists lo, enp0s3, and other interfaces | [Interfaces](../screenshots/01-interfaces.png) |
| 2 | Start loopback web server | Serving HTTP on 127.0.0.1 port 8000 | [Server](../screenshots/02-server.png) |
| 3 | Filter HTTP traffic and inspect request | GET / HTTP/1.1; client port 44470, server port 8000; 12 packets displayed, 0 dropped | [HTTP request](../screenshots/03-http.png) |
| 3 | Inspect TCP connection establishment | SYN, SYN/ACK, ACK before GET | [Handshake](../screenshots/03-handshake.png) |
| 4 | Follow TCP Stream | HTTP/1.0 200 OK and Synthetic HTTP lab page visible in ASCII | [Stream](../screenshots/04-stream.png) |
| 4 | Save HTTP capture locally | Wireshark title and status bar show http-lab.pcapng | [Saved capture](../screenshots/04-saved-capture.png) |
| 5 | Inspect DNS query, frame 3 | UDP 127.0.0.1:42751 to 127.0.0.53:53; ID 0x43bf | [Query](../screenshots/05-dns-query.png) |
| 5 | Inspect another DNS question, frame 7 | example.com, type A, class IN; response linked to frame 8 | [Expanded question](../screenshots/05-dns-name.png) |
| 5 | Inspect response, frame 4 | example.com A/IN; two answers; Request In: 3 | [Initial response](../screenshots/05-dns-response.png) |
| 5 | Match first query/response and inspect answers | ID 0x43bf; No error; two IPv4 addresses | [Verified response](../screenshots/05-dns-verified.png) |
| 6 | Inspect protocol hierarchy | Four displayed DNS packets, each IPv4/UDP/DNS | [Statistics](../screenshots/06-summary.png) |
| 6 | Stop lab server | ^C followed by Keyboard interrupt received, exiting. | [Cleanup](../screenshots/06-cleanup.png) |

The guide embeds the screenshots under their matching steps. The linked evidence above preserves the complete record without repeating every image here.

## HTTP and TCP findings

The observed client is 127.0.0.1:44470 and the server is 127.0.0.1:8000. The first three packets show client SYN, server SYN/ACK, and client ACK. The client initial sequence number 259722748 is acknowledged as 259722749; the server initial sequence number 1620620 is acknowledged as 1620621. Each SYN consumes one sequence number, consistent with TCP connection establishment.

Frame 4 contains GET / HTTP/1.1, Host 127.0.0.1:8000, User-Agent curl/7.68.0, and Accept */*. Wireshark links the response to frame 8. Follow TCP Stream shows HTTP/1.0 200 OK, Content-type: text/html, Content-Length: 24, and the synthetic page body. The 24-byte body is consistent with the page text plus a newline. The 286-byte entire conversation value counts stream data, not the packet capture's file size.

The HTTP capture has 12 packets displayed and zero dropped as reported by Wireshark. Its local save is documented by the filename http-lab.pcapng. The raw capture has not been uploaded or independently inspected.

## DNS findings

In the DNS capture, frame 3 is a standard query from 127.0.0.1:42751 to the local resolver 127.0.0.53:53 over UDP. It has transaction ID 0x43bf, flags 0x0120, one question, zero answer records, and one additional record. Zero answers in a query is normal.

Frame 4 shows the matching ID 0x43bf and flags 0x8180, decoded as Standard query response, No error. The question is example.com, type A, class IN. A requests IPv4 addresses; IN is the Internet class. The two answers are:

| Name | Type | Class | Observed address |
|---|---|---|---|
| example.com | A | IN | 172.66.147.243 |
| example.com | A | IN | 104.20.23.154 |

Wireshark links Request In: 3 and reports 0.000078441 seconds between query and response. This is an observed interval on the local resolver leg, not a measurement of an upstream server's latency. Addresses may change; these values describe this capture only. The evidence does not establish cache status, upstream resolver identity, or DNSSEC validation.

Frames 7 and 8 form a separate exchange. Frame 7's example.com A/IN question is documented, but its ID and the expanded frame 8 response were not collected. The completed DNS validation relies on frames 3 and 4; their values are not assigned to frames 7 and 8.

## Protocol summary

| Exchange | Transport | Source | Destination | Meaning |
|---|---|---|---|---|
| HTTP request | TCP | 127.0.0.1:44470 | 127.0.0.1:8000 | GET following verified handshake |
| HTTP response | TCP | 127.0.0.1:8000 | 127.0.0.1:44470 | 200 OK and synthetic plaintext body |
| DNS query, frame 3 | UDP | 127.0.0.1:42751 | 127.0.0.53:53 | example.com IPv4 lookup |
| DNS response, frame 4 | UDP | 127.0.0.53 | 127.0.0.1 | Matching response with two A answers; response UDP ports were not expanded in the screenshots |

Protocol Hierarchy Statistics applies the dns display filter. All four displayed packets appear at the IPv4, UDP, and DNS layers, each showing 100%. These are nested layers of the same packets and must not be added together. This summarizes the filtered DNS subset, not the HTTP capture or all VM traffic. Clipped byte-percentage columns were not transcribed.

## Explanations and security implications

**Capture versus display filters:** A capture filter limits packets recorded during capture. A display filter selects which already-captured packets Wireshark shows; hidden packets can remain in the saved capture. This lab documents display filters tcp.port == 8000, tcp.stream == 0, and dns. It does not document a capture filter.

**Temporary client ports:** The operating system typically allocates an ephemeral client port to distinguish a connection from others. Here the HTTP client uses 44470 while the server listens on 8000. The DNS query uses 42751 to contact resolver port 53. Future runs can use different client ports.

**HTTP versus HTTPS:** The observed HTTP request headers and response body are readable in plaintext. HTTPS protects application data using TLS; an ordinary capture without session keys does not reveal the same page contents. IP addresses, transport ports, timing, packet sizes, and some handshake metadata can still be visible. HTTPS was explained conceptually, not captured or tested in this lab.

**Practical controls:** Bind local test services to loopback, use synthetic content, stop the service after testing, and review captures before publishing. For real web services, use properly configured HTTPS. Restrict capture access and avoid collecting unrelated traffic.

## Deviations and limitations

DNS was captured on loopback because the observed query targets a local resolver, rather than on the initially proposed NAT interface. Interface selection followed the actual traffic path. The terminal dig command and resolver configuration were not separately screenshotted; packet evidence establishes the DNS exchange itself.

Only dnsutils installation is visible in the package excerpt. The 570 not-upgraded packages line alone does not establish vulnerabilities or patch status. Successful loopback capture is documented, but capture privilege configuration is not. Numeric command exit codes are not recorded. No sensitive information requiring redaction was found in the published screenshots.

## Cleanup and completion

The server terminal logs three local GET requests returning 200, followed by ^C and Keyboard interrupt received, exiting. This records shutdown by Ctrl+C; the final shell prompt is outside the screenshot. A separate listener check was not performed. The VM, synthetic page, and local HTTP capture are retained for learning. No raw packet capture has been published.

- [x] Environment and scope documented
- [x] Setup and local server documented
- [x] HTTP request, response, ports, and TCP handshake verified
- [x] Stream reconstruction and local capture save documented
- [x] DNS query/response matched and answers inspected
- [x] Filtered protocol statistics and explanations completed
- [x] Server shutdown documented
- [x] Evidence linked, report finalized, and portfolio status updated
