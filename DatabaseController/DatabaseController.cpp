#include "DatabaseController.h"

bool CompressString(const std::string Src, std::string& Dst, int level)
{
	if (Src.size() == 0)
		return false;
	Dst.clear();
	const char* in_str = Src.c_str();
	size_t in_len = Src.size();
	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char out[CHUNK];
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, level);
	if (ret != Z_OK)
		return false;
	std::shared_ptr<z_stream> sp_strm(&strm, [](z_stream* strm) {
		(void)deflateEnd(strm);
		});
	const char* end = in_str + in_len;
	size_t pos_index = 0;
	size_t distance = 0;
	do {
		distance = end - in_str;
		strm.avail_in = (distance >= CHUNK) ? CHUNK : distance;
		strm.next_in = (Bytef*)in_str;
		in_str += strm.avail_in;
		flush = (in_str == end) ? Z_FINISH : Z_NO_FLUSH;
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = deflate(&strm, flush);
			if (ret == Z_STREAM_ERROR)
				break;
			have = CHUNK - strm.avail_out;
			Dst.append((const char*)out, have);
		} while (strm.avail_out == 0);
		if (strm.avail_in != 0)
			break;
	} while (flush != Z_FINISH);
	if (ret != Z_STREAM_END)
		return false;
	return true;
}

bool UncompressString(const std::string Src, std::string& Dst)
{
	if (Src.size() == 0)
		return false;
	Dst.clear();
	const char* in_str = Src.c_str();
	size_t in_len = Src.size();
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char out[CHUNK];
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;
	std::shared_ptr<z_stream> sp_strm(&strm, [](z_stream* strm) {
		(void)inflateEnd(strm);
		});
	const char* end = in_str + in_len;
	size_t pos_index = 0;
	size_t distance = 0;
	int flush = 0;
	do {
		distance = end - in_str;
		strm.avail_in = (distance >= CHUNK) ? CHUNK : distance;
		strm.next_in = (Bytef*)in_str;
		in_str += strm.avail_in;
		flush = (in_str == end) ? Z_FINISH : Z_NO_FLUSH;
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			if (ret == Z_STREAM_ERROR)
				break;
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				return ret;
			}
			have = CHUNK - strm.avail_out;
			Dst.append((const char*)out, have);
		} while (strm.avail_out == 0);
	} while (flush != Z_FINISH);
	return ret == Z_STREAM_END ? true : false;
}

void DataToBase64(const std::string Src, std::string& Dst)
{
	CryptoPP::StringSource(Src, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(Dst)));
}

void Base64ToData(const std::string Src, std::string& Dst)
{
	CryptoPP::StringSource(Src, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(Dst)));
}

void EncryptJsonData_Pkgv1(const nlohmann::json& Json, std::string& EncryptedData)
{
	std::string JsonText, CompressedData;
	JsonText.clear();
	JsonText = Json.dump();
	CompressString(JsonText, CompressedData, 6);
	DataToBase64(CompressedData, EncryptedData);
}

void DecryptJsonData_Pkgv1(const std::string& EncryptedData, nlohmann::json& Json)
{
	std::string JsonText, CompressedData;
	Json.clear();
	Base64ToData(EncryptedData, CompressedData);
	UncompressString(CompressedData, JsonText);
	Json = nlohmann::json::parse(JsonText);
}

bool LoadDatabaseDescription(DatabaseHandle& Handle)
{
	if (!Handle.Loaded)
		return false;
	Handle.DatabaseDescription.Name = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_NAME];
	Handle.DatabaseDescription.Author = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_AUTHOR];
	Handle.DatabaseDescription.Message = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_MESSAGE];
	Handle.DatabaseDescription.Version = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_VERSION];
	Handle.DatabaseDescription.Identification = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_IDENTIFICATION];
	Handle.DatabaseDescription.Toolchain = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_TOOLCHAIN];
	return true;
}

bool LoadSelectedTableDescription(DatabaseHandle& Handle)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	Handle.TableDescription.Message = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_TABLE_MESSAGE];
	return true;
}

void InitDatabaseHandle(DatabaseHandle& Handle)
{
	Handle.Loaded = false;
	Handle.Selected = false;
	if (Handle.File.is_open())
		Handle.File.close();
	Handle.Path.clear();
	Handle.SelectedTable.clear();
	Handle.Data.clear();
	memset(&Handle.DatabaseDescription, 0, sizeof(DatabaseDescription));
	memset(&Handle.TableDescription, 0, sizeof(TableDescription));
}

bool CreateDatabase(DatabaseHandle& Handle, const std::string Path, const std::string DatabaseName, const std::string Author)
{
	if (Handle.Loaded)
		return false;
	Handle.File.open(Path, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!Handle.File.is_open())
		return false;
	Handle.File.close();
	Handle.Loaded = true;
	Handle.Path = Path;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_NAME] = DatabaseName;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_AUTHOR] = Author;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_MESSAGE] = std::string();
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_TOOLCHAIN] = TOOLCHAIN_NAME;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_VERSION] = LATEST_IDENTIFICATION;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_IDENTIFICATION] = LATEST_IDENTIFICATION_CODE;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST] = nlohmann::json::array();
	Handle.Data[KEY_DATABASE_DATA] = nlohmann::json::object();
	LoadDatabaseDescription(Handle);
	return true;
}

bool LoadDatabase(DatabaseHandle& Handle, const std::string Path)
{
	if (Handle.Loaded)
		return false;
	if (Handle.File.is_open())
		Handle.File.close();
	Handle.File.open(Path, std::ios::in | std::ios::binary);
	if (!Handle.File.is_open())
		return false;
	int ch0 = Handle.File.get();
	if (!Handle.File.eof() && ch0 == '{')
		Handle.File.seekg(Handle.File.beg);
	else
	{
		InitDatabaseHandle(Handle);
		return false;
	}
	nlohmann::json TempJson;
	Handle.File >> TempJson;
	Handle.File.close();
	if (TempJson.contains(KEY_JDB_PKG_IDENTIFICATION) && TempJson[KEY_JDB_PKG_IDENTIFICATION].is_number() && TempJson[KEY_JDB_PKG_DATA].is_string())
	{
		size_t Identification = TempJson[KEY_JDB_PKG_IDENTIFICATION];
		std::string EncryptedData;
		switch (Identification)
		{
		case 1:
			EncryptedData = TempJson[KEY_JDB_PKG_DATA];
			DecryptJsonData_Pkgv1(EncryptedData, Handle.Data);
			Handle.Loaded = true;
			Handle.Path = Path;
			LoadDatabaseDescription(Handle);
			TempJson.clear();
			EncryptedData.clear();
			return true;
			break;
		}
	}
	TempJson.clear();
	InitDatabaseHandle(Handle);
	return false;
}

bool SaveDatabaseDescription(DatabaseHandle& Handle)
{
	if (!Handle.Loaded)
		return false;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_NAME] = Handle.DatabaseDescription.Name;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_AUTHOR] = Handle.DatabaseDescription.Author;
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_DATABASE_MESSAGE] = Handle.DatabaseDescription.Message;
	return true;
}

bool SaveDatabase(DatabaseHandle& Handle)
{
	return SaveDatabaseTo(Handle, Handle.Path);
}

bool SaveDatabaseTo(DatabaseHandle& Handle, const std::string Path)
{
	if (!Handle.Loaded)
		return false;
	if (Handle.File.is_open())
		Handle.File.close();
	Handle.File.open(Path, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!Handle.File.is_open())
		return false;
	std::string EncryptedData;
	EncryptJsonData_Pkgv1(Handle.Data, EncryptedData);
	nlohmann::json TempJson;
	TempJson[KEY_JDB_PKG_IDENTIFICATION] = VAL_JDB_PKG_IDENTIFICATION;
	TempJson[KEY_JDB_PKG_DATA] = EncryptedData;
	Handle.File << TempJson;
	Handle.File.close();
	TempJson.clear();
	EncryptedData.clear();
	return true;
}

bool GetTableList(DatabaseHandle& Handle, stringlist& TableNameList)
{
	if (!Handle.Loaded)
		return false;
	TableNameList.clear();
	size_t size = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST].size();
	if (size > 0)
		TableNameList = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST];
	return true;
}

bool CreateTable(DatabaseHandle& Handle, const std::string TableName)
{
	if (!Handle.Loaded)
		return false;
	if (Handle.Data[KEY_DATABASE_DATA].contains(TableName))
		return false;
	size_t size = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST].size();
	Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST][size] = TableName;
	Handle.Data[KEY_DATABASE_DATA][TableName][KEY_TABLE_DESCRIPTION][KEY_TABLE_MESSAGE] = std::string();
	Handle.Data[KEY_DATABASE_DATA][TableName][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST] = nlohmann::json::array();
	Handle.Data[KEY_DATABASE_DATA][TableName][KEY_TABLE_DATA] = nlohmann::json::object();
	return true;
}

bool DeleteTable(DatabaseHandle& Handle, const std::string TableName)
{
	if (!Handle.Loaded)
		return false;
	if (!Handle.Data[KEY_DATABASE_DATA].contains(TableName))
		return false;
	Handle.Data[KEY_DATABASE_DATA].erase(TableName);
	size_t size = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST].size();
	for (size_t i = 0; i < size; i++)
		if (Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST][i] == TableName)
		{
			Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST].erase(i);
			break;
		}
	if (Handle.Selected && TableName == Handle.SelectedTable)
	{
		Handle.Selected = false;
		Handle.SelectedTable.clear();
		memset(&Handle.TableDescription, 0, sizeof(TableDescription));
	}
	return true;
}

bool SelectTable(DatabaseHandle& Handle, const std::string TableName)
{
	if (!Handle.Loaded)
		return false;
	if (!Handle.Data[KEY_DATABASE_DATA].contains(TableName))
		return false;
	Handle.Selected = true;
	Handle.SelectedTable = TableName;
	LoadSelectedTableDescription(Handle);
	return true;
}

bool RenameTable(DatabaseHandle& Handle, const std::string OldTableName, const std::string NewTableName)
{
	if (!Handle.Loaded)
		return false;
	if (!Handle.Data[KEY_DATABASE_DATA].contains(OldTableName) || Handle.Data[KEY_DATABASE_DATA].contains(NewTableName))
		return false;
	Handle.Data[KEY_DATABASE_DATA][NewTableName] = Handle.Data[KEY_DATABASE_DATA][OldTableName];
	Handle.Data[KEY_DATABASE_DATA].erase(OldTableName);
	size_t size = Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST].size();
	for (size_t i = 0; i < size; i++)
		if (Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST][i] == OldTableName)
		{
			Handle.Data[KEY_DATABASE_DESCRIPTION][KEY_TABLE_NAME_LIST][i] = NewTableName;
			break;
		}
	if (Handle.Selected && OldTableName == Handle.SelectedTable)
		Handle.SelectedTable = NewTableName;
	LoadSelectedTableDescription(Handle);
	return true;
}

bool SaveTableDescription(DatabaseHandle& Handle)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_TABLE_MESSAGE] = Handle.TableDescription.Message;
	return true;
}

bool GetFieldList(DatabaseHandle& Handle, stringlist& FieldNameList)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	FieldNameList.clear();
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].size();
	if (size > 0)
		FieldNameList = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST];
	return true;
}

bool CreateField(DatabaseHandle& Handle, const std::string FieldName)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	if (Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].contains(FieldName))
		return false;
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].size();
	Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][size] = FieldName;
	Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][FieldName] = nlohmann::json::array();
	if (size > 0)
	{
		size_t number = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][0])].size();
		if (number > 0)
			for (size_t i = 0; i < number; i++)
				Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][FieldName][i] = std::string();
	}
	return true;
}

bool DeleteField(DatabaseHandle& Handle, const std::string FieldName)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	if (!Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].contains(FieldName))
		return false;
	Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].erase(FieldName);
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].size();
	for (size_t i = 0; i < size; i++)
		if (Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][i] == FieldName)
		{
			Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].erase(i);
			break;
		}
	return true;
}

bool RenameField(DatabaseHandle& Handle, const std::string OldFieldName, const std::string NewFieldName)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	if (!Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].contains(OldFieldName) || Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].contains(NewFieldName))
		return false;
	Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][NewFieldName] = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][OldFieldName];
	Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].erase(OldFieldName);
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].size();
	for (size_t i = 0; i < size; i++)
		if (Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][i] == OldFieldName)
		{
			Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][i] = NewFieldName;
			break;
		}
	return true;
}

bool GetFieldData(DatabaseHandle& Handle, const std::string FieldName, stringlist& FieldData)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	if (!Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].contains(FieldName))
		return false;
	FieldData.clear();
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][FieldName].size();
	if (size > 0)
		FieldData = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][FieldName];
	return true;
}

bool AppendLine(DatabaseHandle& Handle, const stringlist Units)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].size();
	size_t datalen = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][0])].size();
	size_t unitsize = Units.size();
	if (size > 0)
		for (size_t i = 0; i < size; i++)
			if (i < unitsize)
				Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][i])][datalen] = Units[i];
			else
				Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][i])][datalen] = std::string();
	else
		return false;
	return true;
}

bool DeleteLine(DatabaseHandle& Handle, const size_t LineId)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].size();
	size_t datalen = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][0])].size();
	if (size > 0 && LineId < datalen)
		for (size_t i = 0; i < size; i++)
			Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][i])].erase(LineId);
	return true;
}

bool DeleteLine(DatabaseHandle& Handle, const size_t LineId, const size_t Number)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	size_t size = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST].size();
	size_t datalen = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][0])].size();
	if (size > 0 && LineId < datalen && (LineId + Number - 1) < datalen)
		for (size_t i = 0; i < Number; i++)
			for (size_t j = 0; j < size; j++)
				Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][std::string(Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DESCRIPTION][KEY_FIELD_NAME_LIST][j])].erase(LineId);
	return false;
}

bool ChangeUnit(DatabaseHandle& Handle, const std::string FieldName, const size_t LineId, const std::string DataString)
{
	if (!Handle.Loaded || !Handle.Selected)
		return false;
	if (!Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA].contains(FieldName))
		return false;
	size_t datalen = Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][FieldName].size();
	if (LineId < datalen)
		Handle.Data[KEY_DATABASE_DATA][Handle.SelectedTable][KEY_TABLE_DATA][FieldName][LineId] = DataString;
	else
		return false;
	return false;
}
