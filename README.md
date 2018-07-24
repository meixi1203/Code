### 项目描述:<br>
* 项目名: 区块链钱包前置机<br>
* 项目功能简述: 本模块是区块链钱包前置网关，提供外部访问接口，连接后端区块链服务器<br>
* 代码仓库地址: http://gitlab.mumubuy.com/wallet/wallet_front.git<br>

### 如何运行:<br>
* 开发环境配置: linux 安装cmake, g++(支持c++11)<br>
* 发布命令: 在project目录下建立build目录(project/build),在build目录里执行cmake .. && make命令,进入bin目录执行wallet_front,wallet_front_gtest是单体测试可执行文件<br>
* 业务介绍:  <br>

### 业务介绍:<br>
* project里的gtest是单元测试，其余是该项目的业务代码<br>
* tests目录的代码是结合测试代码，配置好端口号之后就可以与服务端(wallet_front)进行通信<br>
* <br>

### 项目备注:<br>
* ZMQ      version 5.1.6<br> 
* Protobuf version 3.0.0-beta-1<br> 
* log4plus version 1.2.1<br> 
* <br>

//2018/5/29 zhoupenghui