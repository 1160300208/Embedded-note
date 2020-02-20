/**************************************************
 * 6.2 Linux文件IO编程（一）                      *
 **************************************************/
// 打开文件
#include <sys/types.h>
#include <stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
int creat(const char *pathname, mode_t mode);
// int返回值是文件描述符
/* Flag标记
 *
 * -- O_RDONLY 以只读方式打开
 * -- O_WRONLY 以只写方式打开
 * -- O_RDWR 以可读可写方式打开
 *
 * 当打开已经存在并且内部有内容的文件时
 * -- O_APPEND 以添加方式打开文件
 * -- O_TRUNC 删除文件中的全部数据
 */

// 文件读写
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
/* fd 文件描述符
 * count 缓存大小
 * read函数返回值是读出数据的大小
 */

/**************************************************
 * 6.3 Linux文件IO编程（二）                      *
 **************************************************/
// 控制文件读写位置
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
/* lseek参数
 *
 * fd 已经打开的文件描述符
 * offset 偏移量，每一读写操作需要移动的距离，单位是字节，可正可负
 * whence 当前位置的基点
 * -- SEEK_SET 当前位置为文件的开头
 * -- SEEK_CUR 当前位置为文件指针的位置
 * -- SEEK_END 当前位置为文件的结尾
 */

/**************************************************
 * 6.4 Linux文件IO编程（三）                      *
 **************************************************/
// fcntl函数
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock* lock);
/* fcntl参数
 *
 * cmd
 * -- F_DUPFD 与dup函数功能一样，复制由fd指向的文件描述符，调用成功后
 *            返回新的文件描述符，与旧的文件描述符共同指向同一个文件
 * -- F_GETFD 读取文件描述符close-on-exec标志
 * -- F_SETFD 将文件描述符close-on-exec标志设置为arg的最后一位
 * -- F_GETFL 获取文件打开方式的标志，标志含义与open调用一致
 * -- F_SETF 设置文件打开方式为arg指定方式
 * -- F_SETLK 此时fcntl函数用来设置或释放锁。当l_type为F_RDLCK为读锁，
 *            F_WRLCK为写锁，F_UNLCK为解锁
 * 如果锁被其他进程占用，则返回-1；
 * 这种情况设的锁遇到锁被其他进程占用时，会立刻停止进程。
 * -- F_SETLKW 此时也是给文件上锁，不同于F_SETLK的是，该上锁是阻塞方式
 * -- F_GETLK lock指向一个希望设置锁的属性结构，如果锁能被设置，该命令
 *            并不真的设置锁，而只是修改lock的l_type为F_UNLCK，然后返回
 *            该结构体。如果存在一个或多个锁与希望设置的锁冲突，则fcntl
 *            返回其中一个锁的flock结构
 */
struct flock {
		short l_type; // 锁的类型
		short l_whence; // 偏移量的起始位置：SEEK_SET, SEEK_CUR, SEEK_END
		off_t l_start; // 加锁的起始偏移
		off_t l_len; // 上锁字节
		pid_t l_pid; // 锁的属主进程ID
};

/**************************************************
 * 6.5 标准IO编程                                 *
 **************************************************/
// 打开文件
#include <stdio.h>
FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);
/** mode取值
 * +-----------+--------------------------------------------------------------+
 * | 标志名    | 说明                                                         |
 * +-----------+--------------------------------------------------------------+
 * | r或rb     | 打开只读文件，该文件必须存在                                 |
 * +-----------+--------------------------------------------------------------+
 * | r+或r+b   | 打开可读写的文件，该文件必须存在                             |
 * +-----------+--------------------------------------------------------------+
 * | w或wb     | 打开只写文件，若文件不存在则建立该文件                       |
 * +-----------+--------------------------------------------------------------+
 * | w+或w+b   | 打开可读写文件，若文件不存在则建立该文件                     |
 * +-----------+--------------------------------------------------------------+
 * | a或ab     | 以附加方式打开只写文件                                       |
 * +-----------+--------------------------------------------------------------+
 * | a+或a+b   | 以附加方式打开可读写文件                                     |
 * +-----------+--------------------------------------------------------------+
 *
 */

// 文件读写
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
/**
 * *ptr 内容
 * size 每个条目的字节数
 * nmemb 条目数
 * 返回值：读或写的条目数
 *
 */

//文件状态
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf); // 返回符号连接的信息
struct stat {
		dev_t st_dev; // 文件的设备编号
		ino_t st_ino; // 文件的inode
		mode_t st_mode; // 文件的类型和存取的权限
		nlink_t st_nlink; // 连接到该文件的硬链接数，刚建立的文件值为1
		uid_t st_uid; // 文件所有者的uid
		gid_t st_gid; // 文件所有者的gid
		dev_t st_rdev; // 若此文件为装置设备文件，则为其设备编号
		off_t st_size; // 文件大小，以字节计算
		blksize_t st_blksize; // 文件系统的I/O缓冲区大小
		blkcnt_t st_blocks; // 占用文件区块的个数，区块大小为512字节
		time_t st_atime; // 文件最近一次被存取或执行的时间
		time_t st_mtime; // 文件最后一次被修改的时间
		time_t st_ctime; // inode最近一次被更改的时间
};

/**************************************************
 * 6.8 串口通讯编程（二）                         *
 **************************************************/
// 串口设置
struct termios {
		unsigned short c_iflag; // 输入模式标志
		unsigned short c_oflag; // 输出模式标志
		unsigned short c_cflag; // 控制模式标志
		unsigned short c_lflag; // 本地模式标志
		unsigned char c_line; // 线路规程
		unsigned char c_cc[NCC]; // 控制特性
		speed_t c_ispeed; // 输入速度
		speed_t c_ospeed; // 输出速度
};
/** c_cflag常量名称
 * +-----------+-----------------------------------------------------+
 * | 标志名    | 说明                                                |
 * +-----------+-----------------------------------------------------+
 * | INPCK     | 奇偶校验使能                                        |
 * +-----------+-----------------------------------------------------+
 * | IGNPAR    | 忽略奇偶校验错误                                    |
 * +-----------+-----------------------------------------------------+
 * | PARMKR    | 奇偶校验错误掩码                                    |
 * +-----------+-----------------------------------------------------+
 * | ISTRIP    | 裁剪掉第8位比特                                     |
 * +-----------+-----------------------------------------------------+
 * | IXON      | 启动输出软件流控                                    |
 * +-----------+-----------------------------------------------------+
 * | IXOFF     | 启动输入软件流控                                    |
 * +-----------+-----------------------------------------------------+
 * | IXANY     | 输入任意字符可以重新启动输出                        |
 * |           | (默认为输入起始字符才重启输出)                      |
 * +-----------+-----------------------------------------------------+
 * | IGNBRK    | 忽略输入终止条件                                    |
 * +-----------+-----------------------------------------------------+
 * | BRKINT    | 当检测到输入终止条件时发送SIGINT信号                |
 * +-----------+-----------------------------------------------------+
 * | INLCR     | 将接收到的NL(换行符)转换为CR(回车符)                |
 * +-----------+-----------------------------------------------------+
 * | IGNCR     | 忽略接收到的CR(回车符)                              |
 * +-----------+-----------------------------------------------------+
 * | ICRNL     | 将接收到的CR(回车符)转换为NL(换行符)                |
 * +-----------+-----------------------------------------------------+
 * | IUCLC     | 将接收到的大写字符映射为小写字符                    |
 * +-----------+-----------------------------------------------------+
 * | IMAXBEL   | 当输入队列满时响铃                                  |
 * +-----------+-----------------------------------------------------+
 *
 */

/* 设置波特率 */
tcgetattr(fd, &options);
// 设置波特率为19200
cfsetispeed(&options, B19200);
cfsetospeed(&options, B19200);
// 将本地模式(CLOCAL)和串行数据接收(CREAD)设置为有效
options.c_cflag |= (CLOCAL | CREAD);
// 设置串口
tcsetattr(fd, TCSANOW, &options);

/* 设置数据位 */
options.c_cflag &= ~CSIZE; //用数据位掩码清空数据位设置
options.c_cflag |= CS8; // 使用8位数据位

/* 设置奇偶校验位 */
// 使能奇校验
options.c_cflag |= (PARODD | PARENB);
options.c_iflag |= INPCK;
// 使能偶校验
options.c_cflag |= PARENB;
options.c_cflag &= ~PARODD;
options.c_iflag |= INPCK;

/* 设置停止位 */
options.c_cflag &= ~CSTOPB; // 将停止位设置为1个比特
options.c_cflag |= CSTOPB; // 将停止位设置为2个比特

/* 激活配置 */
#include <termios.h>
#include <unistd.h>
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);

/**************************************************
 * 6.9 串口通讯编程（三）                         *
 **************************************************/
/* 打开串口 */
int fd;
fd = open("/dev/ttyS0", O_RDWR | O_CTTY | O_NDELAY);
if (fd == -1) {
		perror("open_port: Unable to open /dev/ttyS0\n");
}

/* 读写串口 */
// 串口发送数据
char buffer[] = "Hello Linux";
int len;
int nByte;
len = strlen(buffer);
nByte =	write(fd, buffer, len);
if (nByte < len) {
		perror("write fault\n");
}
// 串口读取数据
char buffer[1000];
int nByte;
int len = 900;
nByte = read(fd, buffer, len);
if (nByte < 0) {
		perror("read fault\n");
}

/**************************************************
 * 6.11 网络通讯编程（二）                        *
 **************************************************/
/* socket() */
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
/* domain 协议族
 * type 套接字类型
 * protocol 协议号，通常是0
 */
/** 协议族定义
 * +-------------------+-----------------------------+
 * | AF_UNIX, AF_LOCAL | UNIX域协议                  |
 * +-------------------+-----------------------------+
 * | AF_INET           | IPv4协议                    |
 * +-------------------+-----------------------------+
 * | AF_INET6          | IPv6协议                    |
 * +-------------------+-----------------------------+
 * | AF_IPX            | IPX - Novell协议            |
 * +-------------------+-----------------------------+
 * | AF_NETLINK        | 内核用户接口设备            |
 * +-------------------+-----------------------------+
 * | AF_X25            | ITU-T X.25 / ISO-8208协议   |
 * +-------------------+-----------------------------+
 * | AF_AX25           | AX.25协议                   |
 * +-------------------+-----------------------------+
 * 
 *  套接字类型
 * +-------------+--------------------+
 * | SOCK_STREAM | 字节流套接字       |
 * +-------------+--------------------+
 * | SOCK_DGRAM  | 数据报套接字       |
 * +-------------+--------------------+
 * | SOCK_RAW    | 原始套接字         |
 * +-------------+--------------------+
 *
 */

/* bind() */
// 用于将本地ip绑定到端口号
#include <sys/types.h>
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
/* sockfd 套接字
 * addrlen sockaddr结构体的长度
 */
struct sockaddr {
		unsigned short sa_family; // 地址家族，AF_xxx
		char sa_data[14]; // 14字节协议地址
};
struct sockaddr_in {
		short int sin_family; // 通信类型
		unsigned short int sin_port; // 端口
		struct in_addr sin_addr; // Internet地址
		unsigned char sin_zero[8]; // 与sockaddr结构长度保持一致
};

/* listen() */
#include <sys/types.h>
#include <sys/socket.h>
int listen(int sockfd, int backlog);
/* sockfd 套接字
 * backlog 监听队列容量
 */

/* accept() */
// 接收连接并返回一个新的socket
#include <sys/types.h>
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
/* addr 客户端ip
 *
 * accept函数为阻塞函数
 */

/* send()和recv() */
#include <sys/types.h>
#include <sys/socket.h>
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
/* flags一般设为0
 */

/* connect() */
// 初始化与另一个socket的连接
#include <sys/types.h>
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
