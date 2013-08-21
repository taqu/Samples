#ifndef INC_LCORE_SOCKET_H__
#define INC_LCORE_SOCKET_H__
/**
@file Socket.h
@author t-sakai
@date 2011/08/06 create
*/
#include "../lcore.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <utility>
#include <iosfwd>

namespace lcore
{
    /// プロトコルファミリ
    enum Family
    {
        Family_INET  = AF_INET,  /// IPv4
        Family_INET6 = AF_INET6, /// IPv6
    };

    /// 通信方式
    enum SocketType
    {
        SocketType_STREAM = SOCK_STREAM, /// TCP
        SocketType_DGRAM  = SOCK_DGRAM,  /// UDP
        SocketType_SEQPACKET = SOCK_SEQPACKET,
    };

    /// プロトコル
    enum Protocol
    {
        Protocol_IP  = IPPROTO_IP,  /// 通常はこちらを指定
        Protocol_TCP = IPPROTO_TCP, /// 明示しても可
        Protocol_UDP = IPPROTO_UDP, /// 明示しても可
    };

    enum Flag
    {
        Flag_Passive = AI_PASSIVE, /// 待ちうけフラグ
    };

    /// shutdownに使用
    enum Shut
    {
        Shut_RECEIVE = SD_RECEIVE,
        Shut_SEND    = SD_SEND,
        Shut_BOTH    = SD_BOTH,
    };

    enum SocketOption
    {
        SocketOption_DEBUG = SO_DEBUG,
        SocketOption_ACCEPTCONN = SO_ACCEPTCONN,
        SocketOption_REUSEADDR = SO_REUSEADDR,
        SocketOption_KEEPALIVE = SO_KEEPALIVE,
        SocketOption_DONTROUTE = SO_DONTROUTE,
        SocketOption_BROADCAST = SO_BROADCAST,
        SocketOption_USELOOPBACK = SO_USELOOPBACK,
        SocketOption_LINGER = SO_LINGER,
        SocketOption_OOBINLINE = SO_OOBINLINE,
        SocketOption_DONTLINGER = SO_DONTLINGER,
        SocketOption_EXCLUSIVEADDRUSE = SO_EXCLUSIVEADDRUSE,

        SocketOption_SNDBUF = SO_SNDBUF,
        SocketOption_RCVBUF = SO_RCVBUF,
        SocketOption_SNDLOWAT = SO_SNDLOWAT,
        SocketOption_RCVLOWAT = SO_RCVLOWAT,
        SocketOption_SNDTIMEO = SO_SNDTIMEO,
        SocketOption_RCVTIMEO = SO_RCVTIMEO,

        SocketOption_ERROR = SO_ERROR,
        SocketOption_TYPE = SO_TYPE,
    };

    enum SocketError
    {
        SocketError_EINTR = WSAEINTR, //Interrupted function call
        SocketError_EACCESS = WSAEACCES, //Permission denied
        SocketError_EFAULT = WSAEFAULT, //Bad address
        SocketError_EINVAL = WSAEINVAL, //Invalid argument
        SocketError_EMFILE = WSAEMFILE, //Too many open files
        SocketError_EWOULDBLOCK = WSAEWOULDBLOCK, //Resource temporarily unavailable
        SocketError_EINPROGRESS = WSAEINPROGRESS, //Operation now in progress
        SocketError_EALREADY = WSAEALREADY, //Operation already in progress
        SocketError_ENOTSOCK = WSAENOTSOCK, //Socket operation on nonsocket
        SocketError_EDESTANDDRREQ = WSAEDESTADDRREQ, //Destination address required
        SocketError_EMSGSIZE = WSAEMSGSIZE, //Message too long
        SocketError_EPROTOTYPE = WSAEPROTOTYPE, //Protocol wrong type for socket
        SocketError_ENOPROTOOPT = WSAENOPROTOOPT, //Bad protocol option
        SocketError_EPROTONOSUPPORT = WSAEPROTONOSUPPORT, //Protocol not supported
        SocketError_ESOCKTNOSUPPORT = WSAESOCKTNOSUPPORT, //Socket type not supported
        SocketError_EOPNOTSUPP = WSAEOPNOTSUPP, //Operation not supported
        SocketError_EPFNOSUPPORT = WSAEPFNOSUPPORT, //Protocol family not supported
        SocketError_EAFNOSUPPORT = WSAEAFNOSUPPORT, //Address family not supported by protocol family
        SocketError_EADDRINUSE = WSAEADDRINUSE, //Address already in use
        SocketError_EADDRNOTAVAIL = WSAEADDRNOTAVAIL, //Cannot assign requested address
        SocketError_ENETDOWN = WSAENETDOWN, //Network is down
        SocketError_ENETUNREACH = WSAENETUNREACH, //Network is unreachable
        SocketError_ENETRESET = WSAENETRESET, //Network dropped connection on reset
        SocketError_ECONNABORTED = WSAECONNABORTED, //Software caused connection abort
        SocketError_ECONNRESET = WSAECONNRESET, //Connection reset by peer
        SocketError_ENOBUFS = WSAENOBUFS, //No buffer space available
        SocketError_EISCONN = WSAEISCONN, //Socket is already connected
        SocketError_ENOTCONN = WSAENOTCONN, //Socket is not connected
        SocketError_ESHUTDOWN = WSAESHUTDOWN, //Cannot send after socket shutdown
        SocketError_ETIMEOUT = WSAETIMEDOUT, //Connection timed out
        SocketError_ECONNREFUSED = WSAECONNREFUSED, //Connection refused
        SocketError_EHOSTDOWN = WSAEHOSTDOWN, //Host is down
        SocketError_EHOSTUNREACH = WSAEHOSTUNREACH, //No route to host
        SocketError_EPROCLIM = WSAEPROCLIM, //Too many processes
        SocketError_ESYSNOTREADY = WSASYSNOTREADY, //Network subsystem is unavailable
        SocketError_EVERNOTSUPPORTED = WSAVERNOTSUPPORTED, //Winsock.dll version out of range
        SocketError_ENOTINITIALISED = WSANOTINITIALISED, //Successful WSAStartup not yet performed
        SocketError_EDISCON = WSAEDISCON, //Graceful shutdown in progress
        SocketError_ETYPE_NOT_FOUND = WSATYPE_NOT_FOUND, //Class type not found
        SocketError_EHOST_NOT_FOUND = WSAHOST_NOT_FOUND, //Host not found
        SocketError_ETRY_AGAIN = WSATRY_AGAIN, //Nonauthoritative host not found
        SocketError_ENO_RECOVERY = WSANO_RECOVERY, //This is a nonrecoverable error
        SocketError_ENO_DATA_ = WSANO_DATA, //Valid name, no data record of requested type
    };

    static const u64 InAddrAny = INADDR_ANY;

    class SocketBase;
    class FDSet;
    class NetworkBuffer;

#ifndef _WIN32
    typedef s32 SOCKET;
#endif

    //----------------------------------------------------
    //---
    //--- SocketUtil
    //---
    //----------------------------------------------------
    class SocketUtil
    {
    public:
        static u64 getIP(Family family, const Char* ipInString);
        static bool getIPString(Family family, Char* dst, u32 size, u64 ip);
        static u16 htons(u16 port);
        static u16 ntohs(u16 port);

        static s32 select(s32 nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const timeval* timeout);
    };

    //----------------------------------------------------
    //---
    //--- SocketSystem
    //---
    //----------------------------------------------------
    /**
    @brief ソケットシステム

    ソケットの使用前後に、初期化と終了を行う。
    */
    class SocketSystem
    {
    public:
        /// 初期化
        static bool initialize();

        /// 終了
        static void terminate();

        /**
        @brief 直前のエラーコード取得
        */
        static s32 getError();

        static void printError(std::ostream& os, const Char* message);
    private:
        SocketSystem(){}
        ~SocketSystem(){}

        SocketSystem(const SocketSystem&);
        SocketSystem& operator=(const SocketSystem&);

        static SocketSystem instance_;

        WSADATA wsaData_; /// Windowsソケット初期化情報
    };

    //----------------------------------------------------
    //---
    //--- FDSet
    //---
    //----------------------------------------------------
    class FDSet
    {
    public:
        void clear(SocketBase& socket);
        s32 isSet(SocketBase& socket);
        void set(SocketBase& socket);
        void zero();

        fd_set set_;
    };

    //----------------------------------------------------
    //---
    //--- AddrInfo
    //---
    //----------------------------------------------------
    /**
    @brief 接続情報
    */
    class AddrInfo
    {
    public:
        /// イテレータ
        class Iterator
        {
        public:
            Iterator()
                :addrInfo_(NULL)
            {}

            ~Iterator()
            {}

            bool operator==(const Iterator& rhs) const{ return addrInfo_ == rhs.addrInfo_;}
            bool operator!=(const Iterator& rhs) const{ return addrInfo_ != rhs.addrInfo_;}

            const addrinfo* operator->() const{ LASSERT(addrInfo_!=NULL); return addrInfo_;}

            const addrinfo& operator*() const{ LASSERT(addrInfo_!=NULL); return *addrInfo_;}

            void operator++()
            {
                LASSERT(addrInfo_!=NULL);
                addrInfo_ = addrInfo_->ai_next;
            }

        private:
            friend class AddrInfo;

            explicit Iterator(const addrinfo* info)
                :addrInfo_(info)
            {}

            const addrinfo* addrInfo_;
        };


        AddrInfo()
            :addrInfo_(NULL)
        {}

        ~AddrInfo()
        {
            release();
        }

        /**
        @brief 接続情報取得
        @return 成否
        @param node ... IPまたはホスト名
        @param service ... ポート
        @param family ... プロトコルファミリ
        @param type ... 通信方式
        @param protocol ... プロトコル
        @param flag ... フラグ
        */
        s32 get(
            const Char* node,
            const Char* service,
            Family family,
            SocketType type,
            Protocol protocol,
            s32 flag);

        /// 有効か
        bool valid() const{ return addrInfo_ != NULL;}

        Iterator begin(){ return Iterator(addrInfo_);}
        Iterator begin() const{ return Iterator(addrInfo_);}

        Iterator end(){ return Iterator();}
        Iterator end() const{ return Iterator();}

        /// 先頭取得
        const addrinfo& first() const
        {
            LASSERT(addrInfo_ != NULL);
            return *addrInfo_;
        }
    private:
        AddrInfo(const AddrInfo&);
        AddrInfo& operator=(const AddrInfo&);

        /// 解放
        void release();

        addrinfo* addrInfo_; /// 実態
    };


    //----------------------------------------------------
    //---
    //--- SocketBase
    //---
    //----------------------------------------------------
    /**
    @brief ソケット関数ラッパー
    */
    class SocketBase
    {
    public:
        virtual ~SocketBase();

        /**
        @brief ソケット作成
        @return 成否
        @param family ... プロトコルファミリ
        @param type ... 通信方式
        @param protocol ... プロトコル
        */
        bool create(Family family, SocketType type, Protocol protocol);

        /**
        @brief ソケットが有効か
        @return 有効・無効
        */
        bool valid() const{ return socket_ != INVALID_SOCKET;}

        s32 setOption(SocketOption name, const Char* val, s32 length);
        s32 getOption(SocketOption name, Char* val, s32* length);

        template<class T>
        s32 setOption(SocketOption name, const T* val)
        {
            return setOption(name, reinterpret_cast<const Char*>(val), sizeof(T));
        }

        bool setNonBlock();

        /**
        @brief
        */
        void close();

        /**
        @brief バイト受信
        @return 成功なら受信したバイト数、失敗なら-1
        */
        inline s32 recv(Char* buff, s32 len, s32 flags);

        inline s32 recvfrom(Char* buff, s32 len, s32 flags, SOCKADDR* from, s32* fromlen);

        /**
        @brief バイト送信
        @return 成功なら送信したバイト数、失敗なら-1
        */
        inline s32 send(const Char* buff, s32 len, s32 flags);

        inline s32 sendto(const Char* buff, s32 len, s32 flags, const SOCKADDR* to, s32 tolen);

        /**
        @brief 指定方向の接続を閉じる
        @return 成否
        @param 閉じる接続方向
        */
        s32 shutdown(Shut how);

        const SOCKET& getRaw(){ return socket_;}

    protected:
        friend class FDSet;
        friend class NetworkBuffer;

        SocketBase();

        /**
        @param family ... プロトコルファミリ
        @param type ... 通信方式
        @param protocol ... プロトコル
        @param socket ... ソケットハンドル
        */
        SocketBase(Family family, SocketType type, Protocol protocol, SOCKET socket);

        /**
        @brief 
        @return 成否
        */
        s32 connect(const addrinfo& info);

        /**
        @brief 
        @return 成否
        */
        s32 connect(const SOCKADDR& info);

        /**
        @brief ソケットとアドレス情報を結びつける
        @return 成否
        */
        s32 bind(const SOCKADDR& info);

        /**
        @brief ソケットとアドレス情報を結びつける
        @return 成否
        */
        s32 bind(u16 port, u64 acceptIP);

        /**
        @brief 接続要求待ち受け
        @return 成否
        @param backlog ... 要求キューのサイズ
        */
        s32 listen(s32 backlog);

        /**
        @brief 接続要求をひとつ取り出し、接続確立しソケット作成
        @return 成否
        @param socket ... 出力。接続確立したソケット
        @param addr ... 出力。接続先情報
        @param addrlen ... 接続先情報サイズ
        */
        bool accept(SocketBase& socket, SOCKADDR* addr, s32* addrlen);


        /// スワップ
        void swap(SocketBase& rhs);

        SOCKET socket_; /// ソケットハンドル
        Family family_; /// プロトコルファミリ
        SocketType type_; /// 通信方式
        Protocol protocol_; /// プロトコル

    private:
        SocketBase(const SocketBase&);
        SocketBase& operator=(const SocketBase&);
    };

    // バイト受信
    inline s32 SocketBase::recv(Char* buff, s32 len, s32 flags)
    {
        LASSERT(socket_ != INVALID_SOCKET);
        LASSERT(buff != NULL);
        LASSERT(len > 0);

        return ::recv(socket_, buff, len, flags);
    }

    // バイト受信
    inline s32 SocketBase::recvfrom(Char* buff, s32 len, s32 flags, SOCKADDR* from, s32* fromlen)
    {
        LASSERT(socket_ != INVALID_SOCKET);
        LASSERT(buff != NULL);
        LASSERT(len > 0);

        return ::recvfrom(socket_, buff, len, flags, from, fromlen);
    }

    // バイト送信
    inline s32 SocketBase::send(const Char* buff, s32 len, s32 flags)
    {
        LASSERT(socket_ != INVALID_SOCKET);
        LASSERT(buff != NULL);
        LASSERT(len > 0);

        return ::send(socket_, buff, len, flags);
    }

    // バイト送信
    inline s32 SocketBase::sendto(const Char* buff, s32 len, s32 flags, const SOCKADDR* to, s32 tolen)
    {
        LASSERT(socket_ != INVALID_SOCKET);
        LASSERT(buff != NULL);
        LASSERT(len > 0);

        return ::sendto(socket_, buff, len, flags, to, tolen);
    }

    //----------------------------------------------------
    //---
    //--- Socket
    //---
    //----------------------------------------------------
    /**
    @brief ソケット
    */
    class Socket : public SocketBase
    {
    public:
        Socket()
        {}

        /**
        @param family ... プロトコルファミリ
        @param type ... 通信方式
        @param protocol ... プロトコル
        @param socket ... ソケットハンドル
        */
        Socket(Family family, SocketType type, Protocol protocol, SOCKET socket)
            :SocketBase(family, type, protocol, socket)
        {}

        virtual ~Socket()
        {}

        /**
        @brief 
        @return 成否
        */
        inline s32 connect(const addrinfo& info)
        {
            return SocketBase::connect(info);
        }

        /**
        @brief 
        @return 成否
        */
        inline s32 connect(const SOCKADDR& info)
        {
            return SocketBase::connect(info);
        }

        /**
        @brief ソケットとアドレス情報を結びつける
        @return 成否
        */
        inline s32 bind(const SOCKADDR& info)
        {
            return SocketBase::bind(info);
        }

        /**
        @brief ソケットとアドレス情報を結びつける
        @return 成否
        */
        inline s32 bind(u16 port, u64 acceptIP)
        {
            return SocketBase::bind(port, acceptIP);
        }

        /**
        @brief 接続要求待ち受け
        @return 成否
        @param backlog ... 要求キューのサイズ
        */
        inline s32 listen(s32 backlog)
        {
            return SocketBase::listen(backlog);
        }

        /**
        @brief 接続要求をひとつ取り出し、接続確立しソケット作成
        @return 成否
        @param socket ... 出力。接続確立したソケット
        @param addr ... 出力。接続先情報
        @param addrlen ... 接続先情報サイズ
        */
        inline bool accept(SocketBase& socket, SOCKADDR* addr, s32* addrlen)
        {
            return SocketBase::accept(socket, addr, addrlen);
        }

        /// スワップ
        inline void swap(Socket& rhs)
        {
            SocketBase::swap(rhs);
        }
    };
}

#endif //INC_LCORE_SOCKET_H__
