### Project name: Litalk (Litalk聊天程序)
### Update date: 2020-2-15 18:00
### Version: 1.2
### Complier: Qt-4.8.7(Clinet)   and  Qt-5.9(Server)
### Author: I0gan (徐绿国)
### Plateform: Windows / Linux
### Update: 
新增功能: 可以好友聊天和好友查找功能, 优化了UI界面.并解决了UI透明问题.

不足:
网络延迟比较大,容易丢包..
增加好友重复增加列表或者没有(网络延迟和丢包导致)


### Notice:
	这是我的IM 类的第一个项目吧,做得很不好,但可以涵盖了简单IM的设计思路.
	让我来介绍一下它目前的功能:
	1. 注册登录帐号
	2. 登录后可以修改基本信息还有修改密码
	3. 按帐号查找好友
	4. 添加好友
	5. 好友私聊
	6. 公共群聊

	该项目主要分为: clinet(客户端) 和 server(服务端). 
	部分效果图在temp目录下.

### 使用
#### 建议: 
	服务端:
	Linux平台. 可以从阿里云或者其他平台购买服务器,安装QT5框架和mysql与mysql驱动 
	然后插入下面的表,且修改源码中的mysql登录信息.(connectDatabase()函数里)

	客户端:
	Windows平台:
	Windows平台下特别注意的是打包操作:
	我在build中加入Qt4.87的运行环境所需要一些dll,建议采用Qt4.8.7版本进行编译客户端.
	所需要的依赖环境:
	QtCore4.dll
	QtGui4.dll
	QtNetwork4.dll
	imageformats/qjpeg4.dll  //没有这个的话UI无法解析jpg图片格式
	images/defualt.jpg  //这个是添加好友时的的头像, 可以不要
	images/female.jpg   //这个是女生头像, 可以不要
	images/male.jpg		//这个是男生头像, 可以不要
	libgcc_s_dw2-1.dll  //以下c++程序的一些基础dll
	libstdc++-6.dll
	libwinpthread-1.dll
	msvcrt.dll

	Linux 平台:
	只需让linux安装了Qt的基础运行环境即可.若是deepin桌面或者kde桌面的无需安装Qt框架

#### 修改客户端:
	修改客户端中登陆模块里的ip地址为自己的ip地址,在udp初始化函数里.
	还有control模块也要修改..

#### 修改服务端的
	src/Server/backprosess.cpp 中的connectDatabase函数里的数据.连接自己的数据库.
	在操作此处之前,先建立好数据库里的表.如下:


###		LiTalk MySql Database tables:
```sql
	create table users(
			id int primary key auto_increment,
			account varchar(32),
			password varchar(255),
			nickname varchar(255),
			headImage varchar(255),
			signature varchar(255),
			gender int,
			grade int,
			msg varchar(4096),
			ip varchar(64),
			uport int,
			tport int,
			online int,
			lastlogin varchar(64)
			);

	//nop number of person
	create table groups(
			id int primary key auto_increment,
			account varchar(32),
			name varchar(255),
			headImage varchar(255),
			hacc varchar(32),
			files varchar(255),
			notice varchar(255),
			nop int
			);

	create table user_friend(
			id int primary key auto_increment,
			uacc varchar(32),
			facc varchar(32),
			remark varchar(255)
			);

	create table group_user(
			id int primary key auto_increment,
			gacc varchar(32),
			uacc varchar(32)
			);

	create table pendingtodo(
			id int primary key auto_increment,
			cmd int,
			uacc varchar(32),
			facc varchar(32),
			msg varchar(255)
			);
```
