#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import sys

def init():
    os.system("rm allProxy.txt");
    os.system("touch allProxy.txt");

def ParseProxy():
    for line in open("ProxyWeb.txt"):
        cmd = "./a.out" + " " + line
        content = os.popen(cmd).read();

        f = open("allProxy.txt", "a");
        f.write(content);

def CheckProxy():
    for line in open("allProxy.txt"):
        line = line.strip('\n');
        cmd = "wget -t 1 -T 5 -e http_proxy=http://" + line + " " +  "www.baidu.com";
        #cmd = "wget -t 1 -T 5 -e http_proxy=http://" + line + " " +  "www.costco.com";
        #cmd = "wget -t 1 -T 5 -e http_proxy=http://" + line + " " +  "www.freeproxylists.net";
        os.system(cmd);
        

def main():
    init();
    ParseProxy();
    CheckProxy();

if __name__ == "__main__":
    main();
