# SSP-update

名称: 高性能社交平台 

**项目简介**

服务器采用均衡模型，提高并发，分为用户管理、关系管理、消息交互和界面设计四个主要模块.在网络传输上和存储能力上，对客户端和服务器之间，服务器和数据库之间采用`protobuf`协议进行数据传输并在数据库中进行数据压缩；同时为了提升计算能力，各个模块独立地创建一个进程，采用共享内存`shared memory`(或环形消息队列)的方法，进行进程间的通信。在消息模块，批处理时间线，形成快照，减小了数据的传输压力。同时采用`epoll`多路复用基础上的主从反应堆模式，运用多线程`one thread one loop`模式，提高并发，并在计算能力、存储能力和网络传输能力上进行压力测试。前端使用`qtcreator`框架和`protobuf`框架进行收发包

**git**

```shell
git: https://github.com/ZiXuanLing/SSP-update.git
```

**博客**

```shell
https://blog.csdn.net/qq_48322523?spm=1011.2124.3001.5343
```

**性能采集**

计算能力：[statspack/cpu.png]
存储能力：[statspack/mem.png]
网络传输能力：[statstpack/net.png]

**后期构思**

- 完善前端应用
- 引入`libevent`第三方库，促进项目结构更加模块化
- 对共享内存划分更多的内存块，促进服务器交叉、平行的接收流量，提高并发
- 存储在内存中的数据使用`map`，同时数据存储考虑使用`key-value`数据库

新的一年，好好学习，好好休息。

**项目架构图**

```mermaid
graph LR
A[高性能社交平台] --> | | B1[功能拆解]
B1 --> | | C1[用户管理]
B1 --> | | C2[关系管理]
B1 --> | | C3[消息交互]
B1 --> | | C4[界面设计]
C1 --> | | D1[注册]
C1 --> | | D2[登陆]
C1 --> | | D3[退出]
C1 --> | | D4[注销]
C2 --> | | D5[添加]
C2 --> | | D6[删除]
C2 --> | | D7[屏蔽]
C2 --> | | D8[拉黑]
C3 --> | | D9[发送消息-写发布-写相册-批处理时间线]
C3 --> | | D10[刷新消息-拉时间线-过滤用户]
C3 --> | | D11[拉取消息]
C4 --> | | D12[发送消息]
C4 --> | | D13[刷新消息]
C4 --> | | D14[拉取消息列表]
A --> | | B2[框架]
B2 --> | | C5[C/S]
C5 --> | | D15[Info]
C5 --> | | D16[Manager]
D15 --> | | F1[User]
D15 --> | | F2[Relation]
D15 --> | | F3[Message]
D15 --> | | F4[Photo]
D16 --> | | F5[User-map]
D16 --> | | F6[Relation:map]
D16 --> | | F7[Manager:map]
D16 --> | | F8[Photo:map]
D16 --> | | F9[DB]
F9 --> | | G1[关系型数据库]
F9 --> | | G2[key-value]
D16 --> | | F10[Socket:TCP]
B2 --> | | C6[client]
C6 --> | | D17[Qt:信号与槽]
B2 --> | | C7[server]
C7 --> | | D18[Start]
C7 --> | | D19[Proc]
C7 --> | | D20[Restart]
C7 --> | | D21[Shutdown]
B2 --> | | C8[protobuf]
C8 --> | | D22[proto]
C8 --> | | D23[mess]
D23 --> | | F10[mess_type]
D23 --> | | F11[request]
D23 --> | | F12[response]
A --> | | B3[数据库]
B3 --> | | C8[mysql安装]
B3 --> | | C9[mysql使用]
B3 --> | | C10[mysql接口编程]
A --> | | B4[通信]
B4 --> | | C11[客户端-服务端:proto]
B4 --> | | C12[消息结构]
C12 --> | | D24[头部]
C12 --> | | D25[数据]
A --> | | B5[性能数据采集]
B5 --> | | C13[CPU]
B5 --> | | C14[内存]
B5 --> | | C15[网络]
B5 --> | | C16[IO]
C16 --> | | D26[perf record]
C16 --> | | D27[perf report]
A --> | | B6[优化]
B6 --> | | C17[epoll]
B6 --> | | C18[进程通信]
C18 --> | | D28[消息队列]
C18 --> | | D29[信号量通信]
C18 --> | | D30[共享内存]
A --> | | B7[前端]
B7 --> | | C20[qtcreator]
B7 --> | | C21[Table]
B7 --> | | C22[Socket]
B7 --> | | C23[protobuf]
```
