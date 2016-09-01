# ProxyDiscovery
1. main.cpp 从网页中分析出代理，能够解析base64编码，将代理以 a.b.c.d:port的方式打印出来
2. proxy.py是一个包裹器，能够调用main，获取main打印的代理用wget取网页进行验证
