#pragma once
/* 1.14.60 ����
// [ԭ��] public: void __cdecl CommandRegistry::registerCommand(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64,char const * __ptr64,enum CommandPermissionLevel,struct CommandFlag,struct CommandFlag) __ptr64
// [����] ?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z
constexpr RVA MSSYM_MD5_8574de98358ff66b5a913417f44dd706 = 0x003D1CE0;

// [ԭ��] public: static void __cdecl ChangeSettingCommand::setup(class CommandRegistry & __ptr64)
// [����] ?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z
constexpr RVA MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z = 0x0009CBA0;

// [ԭ��] private: bool __cdecl SPSCQueue<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >,512>::inner_enqueue<0,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64>(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64) __ptr64
// [����] ??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z
constexpr RVA MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e = 0x000A5400;

// [ԭ��] public: virtual void __cdecl ServerNetworkHandler::handle(class NetworkIdentifier const & __ptr64,class CommandRequestPacket const & __ptr64) __ptr64
// [����] ?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z
constexpr RVA MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z = 0x003495A0;

// [ԭ��] private: class ServerPlayer * __ptr64 __cdecl ServerNetworkHandler::_getServerPlayer(class NetworkIdentifier const & __ptr64,unsigned char) __ptr64
// [����] ?_getServerPlayer@ServerNetworkHandler@@AEAAPEAVServerPlayer@@AEBVNetworkIdentifier@@E@Z
constexpr RVA MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z = 0x00356960;

// [ԭ��] public: class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64 __cdecl Level::getPlayerXUID(class mce::UUID const & __ptr64)const __ptr64
// [����] ?getPlayerXUID@Level@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVUUID@mce@@@Z
constexpr RVA MSSYM_MD5_337bfad553c289ba4656ac43dcb60748 = 0x009E6B70;

// [ԭ��] public: virtual class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64 __cdecl Actor::getNameTag(void)const __ptr64
// [����] ?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
constexpr RVA MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd = 0x004FBF00;

// [ԭ��] public: virtual class AutomaticID<class Dimension,int> __cdecl Actor::getDimensionId(void)const __ptr64
// [����] ?getDimensionId@Actor@@UEBA?AV?$AutomaticID@VDimension@@H@@XZ
constexpr RVA MSSYM_B1QE14getDimensionIdB1AA5ActorB2AAA4UEBAB1QA2AVB2QDE11AutomaticIDB1AE10VDimensionB2AAA1HB2AAA2XZ = 0x00501040;

// [ԭ��] public: virtual class Vec3 const & __ptr64 __cdecl Actor::getPos(void)const __ptr64
// [����] ?getPos@Actor@@UEBAAEBVVec3@@XZ
constexpr RVA MSSYM_B1QA6getPosB1AA5ActorB2AAE12UEBAAEBVVec3B2AAA2XZ = 0x004EE910;

// [ԭ��] class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl getEntityName(class Actor const & __ptr64)
// [����] ?getEntityName@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVActor@@@Z
constexpr RVA MSSYM_MD5_01064f7d893d9f9ef50acf1f931d1d79 = 0x003C47B0;

// [ԭ��] public: class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl mce::UUID::asString(void)const __ptr64
// [����] ?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
constexpr RVA MSSYM_MD5_40e8abf6eb08f7ee446159cdd0a7f283 = 0x0017A240;

// [ԭ��] public: __cdecl Player::Player(class Level & __ptr64,class PacketSender & __ptr64,enum GameType,class NetworkIdentifier const & __ptr64,unsigned char,class mce::UUID,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64,class std::unique_ptr<class Certificate,struct std::default_delete<class Certificate> >,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64) __ptr64
// [����] ??0Player@@QEAA@AEAVLevel@@AEAVPacketSender@@W4GameType@@AEBVNetworkIdentifier@@EVUUID@mce@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$unique_ptr@VCertificate@@U?$default_delete@VCertificate@@@std@@@8@55@Z
constexpr RVA MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a = 0x006DC0F0;

// [ԭ��] public: enum DedicatedServer::StartResult __cdecl DedicatedServer::start(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64) __ptr64
// [����] ?start@DedicatedServer@@QEAA?AW4StartResult@1@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z
constexpr RVA MSSYM_MD5_47b0d441c8637d711dea1d116dd4c5b2 = 0x000A6F60;

// [ԭ��] class std::basic_ostream<char,struct std::char_traits<char> > & __ptr64 __cdecl std::_Insert_string<char,struct std::char_traits<char>,unsigned __int64>(class std::basic_ostream<char,struct std::char_traits<char> > & __ptr64,char const * __ptr64 const,unsigned __int64)
// [����] ??$_Insert_string@DU?$char_traits@D@std@@_K@std@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@0@AEAV10@QEBD_K@Z
constexpr RVA MSSYM_MD5_b5f2f0a753fc527db19ac8199ae8f740 = 0x000A2FA0;
*/

// 1.16.10����
// [ԭ��] public: void __cdecl CommandRegistry::registerCommand(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64,char const * __ptr64,enum CommandPermissionLevel,struct CommandFlag,struct CommandFlag) __ptr64
// [����] ?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z
constexpr RVA MSSYM_MD5_8574de98358ff66b5a913417f44dd706 = 0x00429820;

// [ԭ��] public: static void __cdecl ChangeSettingCommand::setup(class CommandRegistry & __ptr64)
// [����] ?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z
constexpr RVA MSSYM_B1QA5setupB1AE20ChangeSettingCommandB2AAE22SAXAEAVCommandRegistryB3AAAA1Z = 0x000A2D50;

// [ԭ��] private: bool __cdecl SPSCQueue<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >,512>::inner_enqueue<0,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64>(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64) __ptr64
// [����] ??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z
constexpr RVA MSSYM_MD5_b5c9e566146b3136e6fb37f0c080d91e = 0x000AB690;

// [ԭ��] public: virtual void __cdecl ServerNetworkHandler::handle(class NetworkIdentifier const & __ptr64,class CommandRequestPacket const & __ptr64) __ptr64
// [����] ?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z
constexpr RVA MSSYM_B1QA6handleB1AE20ServerNetworkHandlerB2AAE26UEAAXAEBVNetworkIdentifierB2AAE24AEBVCommandRequestPacketB3AAAA1Z = 0x0039A140;

// [ԭ��] private: class ServerPlayer * __ptr64 __cdecl ServerNetworkHandler::_getServerPlayer(class NetworkIdentifier const & __ptr64,unsigned char) __ptr64
// [����] ?_getServerPlayer@ServerNetworkHandler@@AEAAPEAVServerPlayer@@AEBVNetworkIdentifier@@E@Z
constexpr RVA MSSYM_B2QUE15getServerPlayerB1AE20ServerNetworkHandlerB2AAE20AEAAPEAVServerPlayerB2AAE21AEBVNetworkIdentifierB2AAA1EB1AA1Z = 0x003A8330;

// [ԭ��] public: class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64 __cdecl Level::getPlayerXUID(class mce::UUID const & __ptr64)const __ptr64
// [����] ?getPlayerXUID@Level@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVUUID@mce@@@Z
constexpr RVA MSSYM_MD5_337bfad553c289ba4656ac43dcb60748 = 0x00A78D10;

// [ԭ��] public: virtual class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64 __cdecl Actor::getNameTag(void)const __ptr64
// [����] ?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
constexpr RVA MSSYM_MD5_7044ab83168b0fd345329e6566fd47fd = 0x0055F1C0;

// [ԭ��] public: __cdecl Player::Player(class Level & __ptr64,class PacketSender & __ptr64,enum GameType,class NetworkIdentifier const & __ptr64,unsigned char,class mce::UUID,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64,class std::unique_ptr<class Certificate,struct std::default_delete<class Certificate> >,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const & __ptr64) __ptr64
// [����] ??0Player@@QEAA@AEAVLevel@@AEAVPacketSender@@W4GameType@@AEBVNetworkIdentifier@@EVUUID@mce@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$unique_ptr@VCertificate@@U?$default_delete@VCertificate@@@std@@@8@55@Z
constexpr RVA MSSYM_MD5_c4b0cddb50ed88e87acce18b5bd3fb8a = 0x0070DF00;

// [ԭ��] public: class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > __cdecl mce::UUID::asString(void)const __ptr64
// [����] ?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
constexpr RVA MSSYM_MD5_40e8abf6eb08f7ee446159cdd0a7f283 = 0x001868E0;
