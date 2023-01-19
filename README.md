<h1 align="center">
  <img alt="cgapp logo" src="https://github.com/sagarbhure/certificates/blob/main/ebpf%20-%20Copy.PNG" width="284px"/><br/>
  <p>Advanced IP-Intelligence & DNS Monitoring using eBPF</p>
</h1>
<p align="center"><b>🛡️ Advanced host monitoring and threat detection with eBPF 🛡️</b></p>

<p align="center"><b>eBPFShield</b> is a high-performance <b>security tool</b> that utilizes eBPF and Python to provide real-time <b>IP-Intelligence</b> and <b>DNS monitoring</b>. By executing in kernel space, eBPFShield avoids costly context switches and offers efficient <b>detection</b> and <b>prevention</b> of malicious behavior on your network through monitoring of outbound connections and comparison with <b>threat intelligence</b> feeds. 🔍 </p>
<div align='center'>
<a href='https://github.com/sagarbhure/eBPFShield/releases'>
  
<img src='https://img.shields.io/github/v/release/chroline/well_app?color=%23FDD835&label=version&style=for-the-badge'>
  
</a>
  
<a href='https://github.com/sagarbhure/eBPFShield/blob/main/LICENSE'>
  
<img src='https://img.shields.io/github/license/chroline/well_app?style=for-the-badge'>

</div>

  ---

## Table of Contents

- [Introduction](#-introduction)
- [Features](#-features)
- [Dependencies](#-dependencies)
- [Usage](#-usage)
- [Sample Output](#sample-output)
- [Contributing](#-contributing)
- [Author](#-author)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 📝 Introduction

  [![Build Status](https://travis-ci.org/dwyl/esta.svg?branch=master)]()
   [![Known Vulnerabilities](https://snyk.io/test/github/dwyl/hapi-auth-jwt2/badge.svg?targetFile=package.json)]()
[![Build Pass](https://img.shields.io/badge/start%20with-why%3F-brightgreen.svg?style=flat)]()
 [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)]()

Welcome to eBPFShield, a powerful and intuitive security tool for monitoring and protecting your servers. Featuring both <b>IP-Intelligence</b> and <b>DNS monitoring</b> capabilities, eBPFShield utilizes the power of ebpf and python to provide real-time monitoring and actionable insights for identifying and mitigating potential threats.
  
Say goodbye to constantly monitoring your servers with tcpdump and hello to a more efficient and automated security solution with eBPFShield.

**Available for ~~Windows~~, Linux and Ubuntu.**

<p align="center">
  <img src = "https://github.com/sagarbhure/eBPFShield/blob/main/.github/images/linux_ubuntu.png" width=350>
</p>

## 🛠 Features
A few of the things you can do with eBPFShield:

**Current Features: 🔥**
  
- **`DNS Monitoring`**: Shows all DNS queries in the system.
- **`IP-Intelligence`**: Monitors outbound connections (tcp/udp) and checks it against threat intelligence lists, block **Malicious Destination**.
Includes script to pull down public threat feeds.


**Feature Roadmap: 📅**

- Automated IP reputation analysis using **Machine Learning** algorithms
- Support for IPv6 and non-standard DNS ports for improved coverage and detection
- Integration with popular **SIEM** systems for centralized monitoring and alerting
- JSON output for easy integration with a **UI** dashboard
- Detection of DNS packets on non-standard ports
  
 ## 📦 Dependencies
 ###### Installation
 
  `apt install python3-bpfcc bpfcc-tools libbpfcc linux-headers-$(uname -r)`
  
 ## 🚀 Usage
  This tool monitors outbound connections (tcp/udp, ipv4 only) and checks it against threat intelligence lists. There is a script included that pulls down two public feeds, the list of active tor exit nodes and Talos' IP blacklist. Just run `./update_feeds.sh` in the root directory of this project and it'll populate the `ip_feeds/` directory. You can add your custom lists to that directory as well. 

You can run the `update_feeds.sh` script in a cron job using `crontab` to regularly update the threat intelligence feed list. This ensures that the list stays up-to-date and that eBPFShield is able to detect and prevent the latest threats.

Run `python main.py` to get started. Out of the box it will not take any action, it'll just print violations as it sees them.

```
$ python main.py -h
usage: main.py [-h] [--block {print, dump, suspend, kill}] [--feature {ebpf_ipintelligence, ebpf_monitor}] [--verbose]

optional arguments:
  -h, --help            show this help message and exit
  --block {print, dump, suspend, kill}
  --feature {ebpf_ipintelligence, ebpf_monitor}
  --verbose

```
There are two options supported under `--features` flag:
  - `ebpf_ipintelligence`: monitor and block outbound connections against IP threat intelligence lists using tcp/udp and ipv4.
  - `ebpf_monitor`: displays all DNS queries in the system. 
  
There are four actions currently supported via the `--block` flag:
- `print`: the default action, just writes to the screen and that's it
- `suspend`: send a `SIGSTOP` to the process. This can be useful if you need to keep the process in a state where you can interact with it.
- `kill`: kill the process. This may be useful if all you want to do is immediately stop potentially malicious behavior.
- `dump`: suspend the process, take a core dump of it for forensics, and then kill it.

If you're interested in debugging, the `--verbose` flag may be useful to you. This tells the program to print all connections it sees, not just malicious ones.
  
## Sample Output
### Block Malicious Destination 🚫

1. In one terminal with root privileges: `$ sudo python main.py --action kill`
2. In another terminal as any user, let's use curl to send an HTTP request to a Tor exit node and another one to google.

We can see we were alerted to only the two out of three curls and that the first two are killed before the connection can complete. The last curl completes just fine.

```
root@host:~/eBPFShield# python3 main.py  --feature ebpf_ipintelligence --block kill
The program is running. Press Ctrl-C to abort.
Client:b'curl' (pid:140278) was killed by eBPFShield (ip-blacklist:31.41.8.66)
Client:b'curl' (pid:140279) was killed by eBPFShield (ip-blacklist:103.43.12.106)
```

```
root@host:~# curl -v 31.41.8.66
*   Trying 31.41.8.66:80...
* TCP_NODELAY set
Killed
root@host:~# curl -v 103.43.12.106
*   Trying 103.43.12.106:80...
* TCP_NODELAY set
Killed
root@host:~# curl google.com
<HTML><HEAD><meta http-equiv="content-type" content="text/html;charset=utf-8">
<TITLE>301 Moved</TITLE></HEAD><BODY>
<H1>301 Moved</H1>
The document has moved
<A HREF="http://www.google.com/">here</A>.
</BODY></HTML>

```


https://user-images.githubusercontent.com/25385987/212558430-7249ee79-2972-44c2-b3aa-1d315fcae1a3.mp4




### Monitor DNS Traffic 🔍
  
```
root@host:~# dig @1.1.1.1 google.com +tcp +short
172.217.160.206

root@host:~# dig @1.1.1.1 geekwire.com +tcp
104.26.14.176
172.67.69.185
104.26.15.176
```
  
```
root@host:~/eBPFShield# python3 main.py --feature ebpf_monitor
The program is running. Press Ctrl-C to abort.
COMM=dig PID=140623 TGID=140624 DEV=ens3 PROTO=TCP SRC=10.XX.20.37 DST=1.1.1.1 SPT=60687 DPT=53 UID=0 GID=0 DNS_QR=0 DNS_NAME=google.com. DNS_TYPE=A
COMM=dig PID=140623 TGID=140624 DEV=ens3 PROTO=TCP SRC=1.1.1.1 DST=10.XX.20.37 SPT=53 DPT=60687 UID=0 GID=0 DNS_QR=1 DNS_NAME=google.com. DNS_TYPE=A DNS_DATA=172.217.160.206

COMM=dig PID=140627 TGID=140628 DEV=ens3 PROTO=TCP SRC=10.XX.20.37 DST=1.1.1.1 SPT=42469 DPT=53 UID=0 GID=0 DNS_QR=0 DNS_NAME=geekwire.com. DNS_TYPE=A
COMM=dig PID=140627 TGID=140628 DEV=ens3 PROTO=TCP SRC=1.1.1.1 DST=10.XX.20.37 SPT=53 DPT=42469 UID=0 GID=0 DNS_QR=1 DNS_NAME=geekwire.com. DNS_TYPE=A DNS_DATA=104.26.14.176
COMM=dig PID=140627 TGID=140628 DEV=ens3 PROTO=TCP SRC=1.1.1.1 DST=10.XX.20.37 SPT=53 DPT=42469 UID=0 GID=0 DNS_QR=1 DNS_NAME=geekwire.com. DNS_TYPE=A DNS_DATA=172.67.69.185
COMM=dig PID=140627 TGID=140628 DEV=ens3 PROTO=TCP SRC=1.1.1.1 DST=10.XX.20.37 SPT=53 DPT=42469 UID=0 GID=0 DNS_QR=1 DNS_NAME=geekwire.com. DNS_TYPE=A DNS_DATA=104.26.15.176
```
  


https://user-images.githubusercontent.com/25385987/212558437-86ed7b2e-2c74-41d5-93b4-3b67a4da949d.mp4




 ## 🤝 Contributing 
  [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/sagarbhure/eBPFShield/blob/main/CONTRIBUTING.md)
  
  Would you like to contribute to this project? CONTRIBUTING.md has all the details on how to do that.
  
 ## 🙋‍ Author
Developed by [@sagarbhure](https://www.github.com/sagarbhure) 🔨 with ❤️ and ☕, Visit me [🌐sagarbhure.com](https://www.sagarbhure.com).
  
  📧 Reach out to me at [mail-id](mailto:sagarbhureaerospace@gmail.com) for any questions or collaborations
 or connect with me on [@LinkedIn](https://www.linkedin.com/in/sagarbhure/) 💻.
  
