# SignalSentry
Writing a tcpdump/wireshark like packet sniffer for educational purposes only.

The goal of this project is to understand how packet sniffers works and handling different protocols for network analysis.

To build and use SignalSentry use the following:

```
mkdir build && cd build && cmake .. && cmake --build . && cd .. && sudo ./build/SignalSentry
```

You have to execute this program as root since it is using raw sockets to listen to the chosen network interface.

The different services that are handled to this date are:

    - SMTP
    - SSH
    - HTTP
    - FTP
    - DNS

You can test each services on the loopback network interface (lo) by sending commands to 127.0.0.1. If you try to send packets to localhosts, adresses will be configured to be IPV6 which are not handled by SignalSentry. Here are a list of commands to test packet capturing on different services. Make sure you have the corresponding services up and running on their default ports:

    - HTTP : curl http://localhost
    - ssh : ssh 127.0.0.1
    - ftp : ftp 127.0.0.1
    - dns : dig @127.0.0.1
    - SMTP : Use telnet to connect to port 25 (default SMTP port) then you can interact with your SMTP server and will get various informations about the captured packets.