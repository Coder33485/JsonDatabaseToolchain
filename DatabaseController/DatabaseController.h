#pragma once

#ifndef JSON_DATABASE_CONTROLER_H
#define JSON_DATABASE_CONTROLER_H

#include "json.hpp"
#include "zlib/zlib.h"
#include "CryptoPP/base64.h"

#include <fstream>
#include <string>
#include <vector>

#define CHUNK	16384

#define LATEST_IDENTIFICATION_CODE		3
#define LATEST_IDENTIFICATION			u8"JDB_v3"
#define TOOLCHAIN_NAME					u8"JDB-Controler_v3.3@Coder33485"

#define KEY_DATABASE_DESCRIPTION		u8"DatabaseDescription"
#define KEY_DATABASE_NAME				u8"DatabaseName"
#define KEY_DATABASE_AUTHOR				u8"DatabaseAuthor"
#define KEY_DATABASE_MESSAGE			u8"DatabaseMessage"
#define KEY_DATABASE_TOOLCHAIN			u8"DatabaseToolchain"
#define KEY_DATABASE_VERSION			u8"DatabaseVersion"
#define KEY_DATABASE_IDENTIFICATION		u8"DatabaseIdentification"
#define KEY_TABLE_NAME_LIST				u8"TableNameList"
#define KEY_DATABASE_DATA				u8"DatabaseData"
#define KEY_TABLE_DESCRIPTION			u8"TableDescription"
#define KEY_TABLE_MESSAGE				u8"TableMessage"
#define KEY_FIELD_NAME_LIST				u8"FieldNameList"
#define KEY_TABLE_DATA					u8"TableData"

#define KEY_JDB_PKG_IDENTIFICATION		u8"JsonDatabase@Coder33485@EncryptPackage"
#define VAL_JDB_PKG_IDENTIFICATION		1
#define KEY_JDB_PKG_DATA				u8"EncryptedData"

typedef struct DatabaseDescription
{
	std::string Name;
	std::string Author;
	std::string Message;
	std::string Toolchain;
	std::string Version;
	unsigned int Identification;
} DatabaseDescription;

typedef struct TableDescription
{
	std::string Message;
} TableDescription;

typedef struct DatabaseHandle
{
	bool Loaded;
	bool Selected;
	std::fstream File;
	std::string Path;
	std::string SelectedTable;
	TableDescription TableDescription;
	DatabaseDescription DatabaseDescription;
	nlohmann::json Data;
} DatabaseHandle;

typedef std::vector<std::string> stringlist;

_declspec(dllexport) void InitDatabaseHandle(DatabaseHandle& Handle);

_declspec(dllexport) bool CreateDatabase(DatabaseHandle& Handle, const std::string Path, const std::string DatabaseName, const std::string Author);

_declspec(dllexport) bool LoadDatabase(DatabaseHandle& Handle, const std::string Path);

_declspec(dllexport) bool SaveDatabaseDescription(DatabaseHandle& Handle);

_declspec(dllexport) bool SaveDatabase(DatabaseHandle& Handle);

_declspec(dllexport) bool SaveDatabaseTo(DatabaseHandle& Handle, const std::string Path);

_declspec(dllexport) bool GetTableList(DatabaseHandle& Handle, stringlist& TableNameList);

_declspec(dllexport) bool CreateTable(DatabaseHandle& Handle, const std::string TableName);

_declspec(dllexport) bool DeleteTable(DatabaseHandle& Handle, const std::string TableName);

_declspec(dllexport) bool SelectTable(DatabaseHandle& Handle, const std::string TableName);

_declspec(dllexport) bool RenameTable(DatabaseHandle& Handle, const std::string OldTableName, const std::string NewTableName);

_declspec(dllexport) bool SaveTableDescription(DatabaseHandle& Handle);

_declspec(dllexport) bool GetFieldList(DatabaseHandle& Handle, stringlist& FieldNameList);

_declspec(dllexport) bool CreateField(DatabaseHandle& Handle, const std::string FieldName);

_declspec(dllexport) bool DeleteField(DatabaseHandle& Handle, const std::string FieldName);

_declspec(dllexport) bool RenameField(DatabaseHandle& Handle, const std::string OldFieldName, const  std::string NewFieldName);

_declspec(dllexport) bool GetFieldData(DatabaseHandle& Handle, const std::string FieldName, stringlist& FieldData);

_declspec(dllexport) bool AppendLine(DatabaseHandle& Handle, const stringlist Units);

_declspec(dllexport) bool DeleteLine(DatabaseHandle& Handle, const size_t LineId);

_declspec(dllexport) bool DeleteLine(DatabaseHandle& Handle, const size_t LineId, const size_t Number);

_declspec(dllexport) bool ChangeUnit(DatabaseHandle& Handle, const std::string FieldName, const size_t LineId, const std::string DataString);

#endif // !JSON_DATABASE_CONTROLER_H
