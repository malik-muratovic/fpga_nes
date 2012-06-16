/***************************************************************************************************
** % File:        dbgpacket.h
*  % Description: DbgPacket class header.
***************************************************************************************************/
#ifndef DBGPACKET_H
#define DBGPACKET_H

#include <windows.h>

enum DbgPacketOpCode
{
    DbgPacketOpCodeEcho              = 0x00, // echo packet body back to debugger
    DbgPacketOpCodeCpuMemRd          = 0x01, // read CPU memory
    DbgPacketOpCodeCpuMemWr          = 0x02, // write CPU memory
    DbgPacketOpCodeDbgBrk            = 0x03, // debugger break (stop execution)
    DbgPacketOpCodeDbgRun            = 0x04, // debugger run (resume execution)
    DbgPacketOpCodeCpuRegRd          = 0x05, // read CPU register
    DbgPacketOpCodeCpuRegWr          = 0x06, // read CPU register
    DbgPacketOpCodeQueryDbgBrk       = 0x07, // query if we are currently in the dbg brk state
    DbgPacketOpCodeQueryErrCode      = 0x08, // query NES error code
    DbgPacketOpCodePpuMemRd          = 0x09, // read PPU memory
    DbgPacketOpCodePpuMemWr          = 0x0A, // write PPU memory
    DbgPacketOpCodePpuDisable        = 0x0B, // disable PPU
    DbgPacketOpCodeCartSetCfg        = 0x0C, // set cartridge config from iNES header
};

enum CpuReg
{
    CpuRegPcl = 0x00, // PCL: Program Counter Low
    CpuRegPch = 0x01, // PCH: Program Counter High
    CpuRegAc  = 0x02, // AC:  Accumulator reg
    CpuRegX   = 0x03, // X:   X index reg
    CpuRegY   = 0x04, // Y:   Y index reg
    CpuRegP   = 0x05, // P:   Processor Status reg
    CpuRegS   = 0x06, // S:   Stack Pointer reg
};

/***************************************************************************************************
** % Class:       DbgPacket
*  % Description: Represents messages sent to and received from the NES FPGA.
***************************************************************************************************/
class DbgPacket
{
public:
    static DbgPacket* CreateObjFromString(TCHAR* pString);
    VOID Destroy() { delete this; }

    virtual ~DbgPacket();

    virtual const BYTE* PacketData() const { return m_pData; }
    virtual UINT SizeInBytes() const = 0;
    virtual UINT ReturnBytesExpected() const = 0;

protected:
    DbgPacket();

    BYTE* m_pData;

private:
    DbgPacket& operator=(const DbgPacket&);
    DbgPacket(const DbgPacket&);
};

/***************************************************************************************************
** % Class:       EchoPacket
*  % Description: Echo debug packet.
***************************************************************************************************/
class EchoPacket : public DbgPacket
{
public:
    EchoPacket(const BYTE* pEchoData, USHORT numBytes);
    virtual ~EchoPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    EchoPacket();
    EchoPacket& operator=(const EchoPacket&);
    EchoPacket(const EchoPacket&);
};

/***************************************************************************************************
** % Class:       CpuMemRdPacket
*  % Description: CPU memory read debug packet.
***************************************************************************************************/
class CpuMemRdPacket : public DbgPacket
{
public:
    CpuMemRdPacket(USHORT addr, USHORT numBytes);
    virtual ~CpuMemRdPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    CpuMemRdPacket();
    CpuMemRdPacket& operator=(const CpuMemRdPacket&);
    CpuMemRdPacket(const CpuMemRdPacket&);
};

/***************************************************************************************************
** % Class:       CpuMemWrPacket
*  % Description: CPU memory write debug packet.
***************************************************************************************************/
class CpuMemWrPacket : public DbgPacket
{
public:
    CpuMemWrPacket(USHORT addr, USHORT numBytes, const BYTE* pData);
    virtual ~CpuMemWrPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    CpuMemWrPacket();
    CpuMemWrPacket& operator=(const CpuMemRdPacket&);
    CpuMemWrPacket(const CpuMemRdPacket&);
};

/***************************************************************************************************
** % Class:       DbgBrkPacket
*  % Description: Debug break debug packet.
***************************************************************************************************/
class DbgBrkPacket : public DbgPacket
{
public:
    DbgBrkPacket();
    virtual ~DbgBrkPacket() {};

    virtual UINT SizeInBytes() const { return 1; }
    virtual UINT ReturnBytesExpected() const { return 0; }

private:
    DbgBrkPacket& operator=(const DbgBrkPacket&);
    DbgBrkPacket(const DbgBrkPacket&);
};

/***************************************************************************************************
** % Class:       DbgRunPacket
*  % Description: Debug run debug packet.
***************************************************************************************************/
class DbgRunPacket : public DbgPacket
{
public:
    DbgRunPacket();
    virtual ~DbgRunPacket() {};

    virtual UINT SizeInBytes() const { return 1; }
    virtual UINT ReturnBytesExpected() const { return 0; }

private:
    DbgRunPacket& operator=(const DbgRunPacket&);
    DbgRunPacket(const DbgRunPacket&);
};

/***************************************************************************************************
** % Class:       CpuRegRdPacket
*  % Description: CPU register read debug packet.
***************************************************************************************************/
class CpuRegRdPacket : public DbgPacket
{
public:
    CpuRegRdPacket(CpuReg reg);
    virtual ~CpuRegRdPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    CpuRegRdPacket();
    CpuRegRdPacket& operator=(const CpuRegRdPacket&);
    CpuRegRdPacket(const CpuRegRdPacket&);
};

/***************************************************************************************************
** % Class:       CpuRegWrPacket
*  % Description: CPU register write debug packet.
***************************************************************************************************/
class CpuRegWrPacket : public DbgPacket
{
public:
    CpuRegWrPacket(CpuReg reg, BYTE val);
    virtual ~CpuRegWrPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    CpuRegWrPacket();
    CpuRegWrPacket& operator=(const CpuRegRdPacket&);
    CpuRegWrPacket(const CpuRegRdPacket&);
};

/***************************************************************************************************
** % Class:       QueryDbgBrkPacket
*  % Description: Debug packet to query the current FPGA state (running or in debug break).
***************************************************************************************************/
class QueryDbgBrkPacket : public DbgPacket
{
public:
    QueryDbgBrkPacket();
    virtual ~QueryDbgBrkPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    QueryDbgBrkPacket& operator=(const QueryDbgBrkPacket&);
    QueryDbgBrkPacket(const QueryDbgBrkPacket&);
};

/***************************************************************************************************
** % Class:       PpuMemRdPacket
*  % Description: PPU memory read debug packet.
***************************************************************************************************/
class PpuMemRdPacket : public DbgPacket
{
public:
    PpuMemRdPacket(USHORT addr, USHORT numBytes);
    virtual ~PpuMemRdPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    PpuMemRdPacket();
    PpuMemRdPacket& operator=(const PpuMemRdPacket&);
    PpuMemRdPacket(const PpuMemRdPacket&);
};

/***************************************************************************************************
** % Class:       PpuMemWrPacket
*  % Description: PPU memory write debug packet.
***************************************************************************************************/
class PpuMemWrPacket : public DbgPacket
{
public:
    PpuMemWrPacket(USHORT addr, USHORT numBytes, const BYTE* pData);
    virtual ~PpuMemWrPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const;

private:
    PpuMemWrPacket();
    PpuMemWrPacket& operator=(const PpuMemRdPacket&);
    PpuMemWrPacket(const PpuMemRdPacket&);
};

/***************************************************************************************************
** % Class:       PpuDisablePacket
*  % Description: PPU disable debug packet.
***************************************************************************************************/
class PpuDisablePacket : public DbgPacket
{
public:
    PpuDisablePacket();
    virtual ~PpuDisablePacket() {};

    virtual UINT SizeInBytes() const { return 1; }
    virtual UINT ReturnBytesExpected() const { return 0; }

private:
    PpuDisablePacket& operator=(const PpuDisablePacket&);
    PpuDisablePacket(const PpuDisablePacket&);
};

/***************************************************************************************************
** % Class:       CartSetCfgPacket
*  % Description: Set cartridge configuration based on iNES header.
***************************************************************************************************/
class CartSetCfgPacket : public DbgPacket
{
public:
    CartSetCfgPacket(const BYTE* pINesHeader);
    virtual ~CartSetCfgPacket() {};

    virtual UINT SizeInBytes() const;
    virtual UINT ReturnBytesExpected() const { return 0; }

private:
    CartSetCfgPacket();
    CartSetCfgPacket& operator=(const CartSetCfgPacket&);
    CartSetCfgPacket(const CartSetCfgPacket&);
};

#endif // DBGPACKET_H
