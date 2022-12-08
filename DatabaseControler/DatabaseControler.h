#pragma once

#ifndef JSON_DATABASE_CONTROLER_H
#define JSON_DATABASE_CONTROLER_H

#include "json.hpp"

#include <fstream>
#include <string>
#include <vector>

#define CREATE_VERSION_CODE		2
#define CREATE_VERSION_NAME		u8"JDB_v2"
#define TOOLCHAIN_NAME			u8"JDB-Controler_v2.0@Coder33485"

const unsigned int LoadSupportedIdentification[] = { 1, 2 };

typedef class JDBKey
{
public:
	JDBKey(size_t Id = 1);
	void SetKey(size_t Id);
	std::string Description;
	std::string Name;
	std::string Author;
	std::string Message;
	std::string Toolchain;
	std::string Version;
	std::string Identification;
	std::string TableNameList;
	std::string FieldNameList;
} JDBKey;

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
	std::string Path;
	std::string Table;
	TableDescription TBL_Description;
	DatabaseDescription DB_Description;
	nlohmann::json Data;
} DatabaseHandle;

typedef std::vector<std::string> stringlist;

_declspec(dllexport) void InitDatabaseHandle(DatabaseHandle& Handle);

_declspec(dllexport) bool CreateDatabase(DatabaseHandle& Handle, const std::string Path, const std::string DatabaseName, const std::string Author);

_declspec(dllexport) bool LoadDatabase(DatabaseHandle& Handle, const std::string Path);

_declspec(dllexport) bool SaveDatabaseDescription(DatabaseHandle& Handle);

_declspec(dllexport) bool SaveDatabase(DatabaseHandle& Handle, const bool Format = false);

_declspec(dllexport) bool SaveDatabaseTo(DatabaseHandle& Handle, const std::string Path, const bool Format = false);

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
