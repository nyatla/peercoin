// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __cplusplus
# error This header can only be compiled as C++.
#endif

#ifndef __INCLUDED_PROTOCOL_H__
#define __INCLUDED_PROTOCOL_H__

#include <string>

#include "serialize.h"
#include "netbase.h"
#include "uint256.h"

#define PPCOIN_PORT  9901
#define RPC_PORT     9902
#define TESTNET_PORT 9903
#define TESTNET_RPC_PORT 9904

extern bool fTestNet;

void GetMessageStart(unsigned char pchMessageStart[], bool fPersistent = false);

static inline unsigned short GetDefaultPort(const bool testnet = fTestNet)
{
    return testnet ? TESTNET_PORT : PPCOIN_PORT;
}


/** Message header.
 * (4) message start.
 * (12) command.
 * (4) size.
 * (4) checksum.
 */
class CMessageHeader
{
    public:
        CMessageHeader();
        CMessageHeader(const char* pszCommand, unsigned int nMessageSizeIn);

        std::string GetCommand() const;
        bool IsValid() const;
	public:
		unsigned int GetSerializeSize(int nType, int nVersion)const
		{
			//fGetSize=true/fWrite=false/fRead=false
			CSerActionGetSerializeSize ser_action;
			unsigned int nSerSize = 0;
			ser_streamplaceholder s;
			s.nType = nType;
			s.nVersion = nVersion;
			READWRITE(FLATDATA(pchMessageStart));
			READWRITE(FLATDATA(pchCommand));
			READWRITE(nMessageSize);
			READWRITE(nChecksum);
			return nSerSize;
		}
		template<typename Stream>
		void Serialize(Stream& s, int nType, int nVersion)const
		{
			//fGetSize=false/fWrite=true/fRead=false
			CSerActionSerialize ser_action;
			unsigned int nSerSize = 0;
			READWRITE(FLATDATA(pchMessageStart));
			READWRITE(FLATDATA(pchCommand));
			READWRITE(nMessageSize);
			READWRITE(nChecksum);
		}
		template<typename Stream>
		void Unserialize(Stream s, int nType, int nVersion)
		{
			//fGetSize=false/fWrite=false/fRead=true
			CSerActionUnserialize ser_action;
			unsigned int nSerSize = 0;
			READWRITE(FLATDATA(pchMessageStart));
			READWRITE(FLATDATA(pchCommand));
			READWRITE(nMessageSize);
			READWRITE(nChecksum);
		}

    // TODO: make private (improves encapsulation)
    public:
        enum { COMMAND_SIZE=12 };
        unsigned char pchMessageStart[4];
        char pchCommand[COMMAND_SIZE];
        unsigned int nMessageSize;
        unsigned int nChecksum;
};

/** nServices flags */
enum
{
    NODE_NETWORK = (1 << 0),
};

/** A CService with information about it as peer */
class CAddress : public CService
{
    public:
        CAddress();
        explicit CAddress(CService ipIn, uint64 nServicesIn=NODE_NETWORK);

        void Init();
	public:
		unsigned int GetSerializeSize(int nType, int nVersion)const
		{
			//fGetSize=true/fWrite=false/fRead=false
			CSerActionGetSerializeSize ser_action;
			unsigned int nSerSize = 0;
			ser_streamplaceholder s;
			s.nType = nType;
			s.nVersion = nVersion;
			CAddress* pthis = const_cast<CAddress*>(this);
			CService* pip = (CService*)pthis;
			if (nType & SER_DISK)
				READWRITE(nVersion);
			if ((nType & SER_DISK) ||
				(nVersion >= CADDR_TIME_VERSION && !(nType & SER_GETHASH)))
				READWRITE(nTime);
			READWRITE(nServices);
			READWRITE(*pip);
			return nSerSize;
		}
		template<typename Stream>
		void Serialize(Stream& s, int nType, int nVersion)const
		{
			//fGetSize=false/fWrite=true/fRead=false
			CSerActionSerialize ser_action;
			unsigned int nSerSize = 0;
			CAddress* pthis = const_cast<CAddress*>(this);
			CService* pip = (CService*)pthis;
			if (nType & SER_DISK)
				READWRITE(nVersion);
			if ((nType & SER_DISK) ||
				(nVersion >= CADDR_TIME_VERSION && !(nType & SER_GETHASH)))
				READWRITE(nTime);
			READWRITE(nServices);
			READWRITE(*pip);
		}
		template<typename Stream>
		void Unserialize(Stream s, int nType, int nVersion)
		{
			//fGetSize=false/fWrite=false/fRead=true
			CSerActionUnserialize ser_action;
			unsigned int nSerSize = 0;
			CAddress* pthis = const_cast<CAddress*>(this);
			CService* pip = (CService*)pthis;
			pthis->Init();
			if (nType & SER_DISK)
				READWRITE(nVersion);
			if ((nType & SER_DISK) ||
				(nVersion >= CADDR_TIME_VERSION && !(nType & SER_GETHASH)))
				READWRITE(nTime);
			READWRITE(nServices);
			READWRITE(*pip);
		}

        void print() const;

    // TODO: make private (improves encapsulation)
    public:
        uint64 nServices;

        // disk and network only
        unsigned int nTime;

        // memory only
        int64 nLastTry;
};

/** inv message data */
class CInv : public CService
{
    public:
        CInv();
        CInv(int typeIn, const uint256& hashIn);
        CInv(const std::string& strType, const uint256& hashIn);
public:
		unsigned int GetSerializeSize(int nType, int nVersion)const
		{
			//fGetSize=true/fWrite=false/fRead=false
			CSerActionGetSerializeSize ser_action;
			unsigned int nSerSize = 0;
			ser_streamplaceholder s;
			s.nType = nType;
			s.nVersion = nVersion;
			READWRITE(type);
			READWRITE(hash);
			return nSerSize;
		}
		template<typename Stream>
		void Serialize(Stream& s, int nType, int nVersion)const
		{
			//fGetSize=false/fWrite=true/fRead=false
			CSerActionSerialize ser_action;
			unsigned int nSerSize = 0;
			READWRITE(type);
			READWRITE(hash);
		}
		template<typename Stream>
		void Unserialize(Stream s, int nType, int nVersion)
		{
			//fGetSize=false/fWrite=false/fRead=true
			CSerActionUnserialize ser_action;
			unsigned int nSerSize = 0;
			READWRITE(type);
			READWRITE(hash);
		}
        friend bool operator<(const CInv& a, const CInv& b);

        bool IsKnownType() const;
        const char* GetCommand() const;
        std::string ToString() const;
        void print() const;

    // TODO: make private (improves encapsulation)
    public:
        int type;
        uint256 hash;
};

#endif // __INCLUDED_PROTOCOL_H__
