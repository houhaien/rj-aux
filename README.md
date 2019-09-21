# 锐捷有线认证，Linux客户端

> 使用qt平台，部分采用Dtk框架，构建图形界面。
> 通过QProcess直接调用原生程序。
> 未对锐捷源生linux客户端程序进行任何实质性修改。



##  Dev Status

- [x] 登录界面
- [x] dock支持
- [x] 认证主逻辑
- [x] 状态界面
- [ ] 信息记忆
- [ ] 禁止重复启动


# 依赖环境

```bash
sudo apt-get install qt5-default qt5-qmake g++ qtcreator

```


## Build

```bash
mkdir build
qmake ..
make 

```


