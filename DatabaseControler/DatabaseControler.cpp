#include "DatabaseControler.h"

JDBKey::JDBKey(size_t Id)
{
	SetKey(Id);
}

void JDBKey::SetKey(size_t Id)
{
	switch (Id)
	{
	case 1:
		Description = u8"Description";
		Name = u8"Name";
		Author = u8"Author";
		Message = u8"Block";
		Toolchain = u8"Toolchain";
		Version = u8"Version";
		Identification = u8"VersionCode";
		TableNameList = u8"Tables";
		FieldNameList = u8"Fields";
		break;
	case 2:
		Description = u8"Description";
		Name = u8"Name";
		Author = u8"Author";
		Message = u8"Message";
		Toolchain = u8"Toolchain";
		Version = u8"Version";
		Identification = u8"Identification";
		TableNameList = u8"TableNameList";
		FieldNameList = u8"FieldNameList";
		break;
	default:
		break;
	}
}

size_t AnalysisDatabaseVersion_Inside(DatabaseHandle& Handle)
{
	JDBKey Key(1);
	if (Handle.Data[Key.Description][Key.Identification].is_number() &&
		Handle.Data[Key.Description][Key.Identification] == 1)
		return 1;
	Key.SetKey(2);
	if (Handle.Data[Key.Description][Key.Identification].is_number() &&
		Handle.Data[Key.Description][Key.Identification] == 2)
		return 2;
	return 0;
}

void GetDatabaseDescription_Inside(DatabaseHandle& Handle)
{
	size_t Id = AnalysisDatabaseVersion_Inside(Handle);
	if (Id == 0)
		return;
	JDBKey Key(Id);
	switch (Id)
	{
	case 1:
	case 2:
		Handle.DB_Description.Name = Handle.Data[Key.Description][Key.Name];
		Handle.DB_Description.Author = Handle.Data[Key.Description][Key.Author];
		Handle.DB_Description.Message = Handle.Data[Key.Description][Key.Message];
		Handle.DB_Description.Toolchain = Handle.Data[Key.Description][Key.Toolchain];
		Handle.DB_Description.Version = Handle.Data[Key.Description][Key.Version];
		Handle.DB_Description.Identification = Handle.Data[Key.Description][Key.Identification];
		break;
	default:
		break;
	}
}

void GetTableDescription_Inside(DatabaseHandle& Handle, std::string TableName)
{
	JDBKey Key(Handle.DB_Description.Identification);
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		Handle.TBL_Description.Message = Handle.Data[TableName][Key.Description][Key.Message];
		break;
	default:
		break;
	}
}

void InitDatabaseHandle(DatabaseHandle& Handle)
{
	Handle.Loaded = false;
	Handle.Selected = false;
	Handle.Path.clear();
	Handle.Table.clear();
	Handle.Data.clear();
	memset(&Handle.DB_Description, 0, sizeof(DatabaseDescription));
	memset(&Handle.TBL_Description, 0, sizeof(TableDescription));
}

bool CreateDatabase(DatabaseHandle& Handle, const std::string Path, const std::string DatabaseName, const std::string Author)
{
	std::fstream file(Path, std::ios::out | std::ios::binary | std::ios::trunc);
	if (file.is_open())
	{
		file.close();
		_unlink(Path.c_str());
	}
	else
	{
		return false;
	}
	JDBKey Key(CREATE_VERSION_CODE);
	Handle.Loaded = true;
	Handle.Selected = false;
	Handle.Path = Path;
	Handle.Data[Key.Description][Key.Name] = DatabaseName;
	Handle.Data[Key.Description][Key.Author] = Author;
	Handle.Data[Key.Description][Key.Version] = CREATE_VERSION_NAME;
	Handle.Data[Key.Description][Key.Identification] = CREATE_VERSION_CODE;
	Handle.Data[Key.Description][Key.Toolchain] = TOOLCHAIN_NAME;
	Handle.Data[Key.Description][Key.Message] = std::string();
	Handle.Data[Key.TableNameList] = nlohmann::json::array();
	GetDatabaseDescription_Inside(Handle);
	return true;
}

bool LoadDatabase(DatabaseHandle& Handle, const std::string Path)
{
	if (Handle.Loaded)
		return false;
	unsigned int Identification = 0;
	std::fstream file(Path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	if (file.eof() || file.get() != '{')
	{
		file.close();
		return false;
	}
	file.seekg(file.beg);
	file >> Handle.Data;
	file.close();
	size_t Id = AnalysisDatabaseVersion_Inside(Handle);
	if (Id == 0)
		return false;
	JDBKey Key(Id);
	switch (Id)
	{
	case 1:
	case 2:
		if (Handle.Data.is_object() && Handle.Data[Key.Description][Key.Identification].is_number())
		{
			Identification = Handle.Data[Key.Description][Key.Identification];
			if (std::find(std::begin(LoadSupportedIdentification), std::end(LoadSupportedIdentification), Identification) != std::end(LoadSupportedIdentification))
			{
				Handle.Loaded = true;
				Handle.Path = Path;
				GetDatabaseDescription_Inside(Handle);
				return true;
			}
		}
		InitDatabaseHandle(Handle);
		break;
	default:
		break;
	}
	return false;
}

bool SaveDatabaseDescription(DatabaseHandle& Handle)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		Handle.Data[Key.Description][Key.Name] = Handle.DB_Description.Name;
		Handle.Data[Key.Description][Key.Author] = Handle.DB_Description.Author;
		Handle.Data[Key.Description][Key.Message] = Handle.DB_Description.Message;
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool SaveDatabase(DatabaseHandle& Handle, const bool Format)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	std::fstream file;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		file.open(Handle.Path, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!file.is_open())
			return false;
		if (Format)
			file << std::setw(4) << Handle.Data;
		else
			file << Handle.Data;
		file.close();
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool SaveDatabaseTo(DatabaseHandle& Handle, const std::string Path, const bool Format)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	std::fstream file;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		file.open(Path, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!file.is_open())
			return false;
		if (Format)
			file << std::setw(4) << Handle.Data;
		else
			file << Handle.Data;
		file.close();
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool GetTableList(DatabaseHandle& Handle, stringlist& TableNameList)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	size_t TableNum = 0;
	std::string TableNameTemp;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		TableNameList.clear();
		TableNum = Handle.Data[Key.TableNameList].size();
		if (TableNum > 0)
		{
			for (size_t i = 0; i < TableNum; i++)
			{
				TableNameTemp = Handle.Data[Key.TableNameList][i];
				TableNameList.push_back(TableNameTemp);
			}
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool CreateTable(DatabaseHandle& Handle, const std::string TableName)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	size_t listsize = 0;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (TableName == Key.TableNameList || TableName == Key.Description)
			return false;
		if (Handle.Data[TableName].is_object())
			return false;
		//stringlist TableNameList;
		listsize = Handle.Data[Key.TableNameList].size();
		if (listsize > 0)
		{
			Handle.Data[Key.TableNameList][listsize] = TableName;
			Handle.Data[TableName][Key.Description][Key.Message] = std::string();
			Handle.Data[TableName][Key.TableNameList] = nlohmann::json::array();
		}
		else
		{
			Handle.Data[Key.TableNameList][0] = TableName;
			Handle.Data[TableName][Key.Description][Key.Message] = std::string();
			Handle.Data[TableName][Key.TableNameList] = nlohmann::json::array();
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool DeleteTable(DatabaseHandle& Handle, const std::string TableName)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	size_t TableId = 0, listsize = 0;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (TableName == Key.TableNameList || TableName == Key.Description)
			return false;
		if (!Handle.Data[TableName].is_object())
			return false;
		//stringlist TableNameList;
		listsize = Handle.Data[Key.TableNameList].size();
		for (size_t i = 0; i < listsize; i++)
			if (TableName == Handle.Data[Key.TableNameList][i])
			{
				TableId = i;
				break;
			}
		Handle.Data[Key.TableNameList].erase(TableId);
		Handle.Data.erase(TableName);
		if (Handle.Table == TableName)
		{
			Handle.Selected = false;
			Handle.Table.clear();
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool SelectTable(DatabaseHandle& Handle, const std::string TableName)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (TableName == Key.TableNameList || TableName == Key.Description)
			return false;
		if (!Handle.Data[TableName].is_object())
			return false;
		//stringlist TableNameList;
		Handle.Selected = true;
		Handle.Table = TableName;
		GetTableDescription_Inside(Handle, TableName);
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool RenameTable(DatabaseHandle& Handle, const std::string OldTableName, const std::string NewTableName)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	nlohmann::json JsonTemp;
	size_t listsize = 0;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (OldTableName == Key.TableNameList || OldTableName == Key.Description || NewTableName == Key.TableNameList || NewTableName == Key.Description)
			return false;
		if (!Handle.Data[OldTableName].is_object())
			return false;
		if (Handle.Data[NewTableName].is_object())
			return false;
		//stringlist TableNameList;
		
		listsize = Handle.Data[Key.TableNameList].size();
		for (size_t i = 0; i < listsize; i++)
			if (OldTableName == Handle.Data[Key.TableNameList][i])
			{
				Handle.Data[Key.TableNameList][i] = NewTableName;
				JsonTemp = Handle.Data[OldTableName];
				Handle.Data.erase(OldTableName);
				Handle.Data[NewTableName] = JsonTemp;
				if (Handle.Table == OldTableName)
					Handle.Table = NewTableName;
				return true;
			}
		break;
	default:
		break;
	}
	return false;
}

bool SaveTableDescription(DatabaseHandle& Handle)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		Handle.Data[Handle.Table][Key.Description][Key.Message] = Handle.TBL_Description.Message;
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool GetFieldList(DatabaseHandle& Handle, stringlist& FieldNameList)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	size_t FieldNum = 0;
	std::string FieldNameTemp;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		FieldNameList.clear();
		FieldNum = Handle.Data[Handle.Table][Key.FieldNameList].size();
		if (FieldNum > 0)
		{
			for (size_t i = 0; i < FieldNum; i++)
			{
				FieldNameTemp = Handle.Data[Handle.Table][Key.FieldNameList][i];
				FieldNameList.push_back(FieldNameTemp);
			}
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool CreateField(DatabaseHandle& Handle, const std::string FieldName)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	size_t listsize = 0, FieldDataLength = 0;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		if (FieldName == Key.FieldNameList || FieldName == Key.Description)
			return false;
		if (Handle.Data[Handle.Table][FieldName].is_array())
			return false;
		//stringlist FieldNameList;
		listsize = Handle.Data[Handle.Table][Key.FieldNameList].size();
		if (listsize > 0)
		{
			Handle.Data[Handle.Table][Key.FieldNameList][listsize] = FieldName;
			Handle.Data[Handle.Table][FieldName] = nlohmann::json::array();
			FieldDataLength = Handle.Data[Handle.Table][std::string(Handle.Data[Handle.Table][Key.FieldNameList][0])].size();
			if (FieldDataLength > 0)
			{
				for (size_t i = 0; i < FieldDataLength; i++)
					Handle.Data[Handle.Table][FieldName][i] = std::string();
			}
		}
		else
		{
			Handle.Data[Handle.Table][Key.FieldNameList][0] = FieldName;
			Handle.Data[Handle.Table][FieldName] = nlohmann::json::array();
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool DeleteField(DatabaseHandle& Handle, const std::string FieldName)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	size_t FieldId = 0, listsize = 0;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		if (FieldName == Key.FieldNameList || FieldName == Key.Description)
			return false;
		if (!Handle.Data[Handle.Table][FieldName].is_array())
			return false;
		//stringlist FieldNameList;
		listsize = Handle.Data[Handle.Table][Key.FieldNameList].size();
		for (size_t i = 0; i < listsize; i++)
			if (FieldName == Handle.Data[Handle.Table][Key.FieldNameList][i])
			{
				FieldId = i;
				break;
			}
		Handle.Data[Handle.Table][Key.FieldNameList].erase(FieldId);
		Handle.Data[Handle.Table].erase(FieldName);
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool RenameField(DatabaseHandle& Handle, const std::string OldFieldName, const std::string NewFieldName)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	nlohmann::json JsonTemp;
	size_t listsize = 0;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		if (OldFieldName == Key.FieldNameList || OldFieldName == Key.Description || NewFieldName == Key.FieldNameList || NewFieldName == Key.Description)
			return false;
		if (!Handle.Data[Handle.Table][OldFieldName].is_array())
			return false;
		if (Handle.Data[Handle.Table][NewFieldName].is_array())
			return false;
		//stringlist FieldNameList;
		listsize = Handle.Data[Handle.Table][Key.FieldNameList].size();
		for (size_t i = 0; i < listsize; i++)
			if (OldFieldName == Handle.Data[Handle.Table][Key.FieldNameList][i])
			{
				Handle.Data[Handle.Table][Key.FieldNameList][i] = NewFieldName;
				JsonTemp = Handle.Data[Handle.Table][OldFieldName];
				Handle.Data[Handle.Table].erase(OldFieldName);
				Handle.Data[Handle.Table][NewFieldName] = JsonTemp;
				return true;
			}
		break;
	default:
		break;
	}
	return false;
}

bool GetFieldData(DatabaseHandle& Handle, const std::string FieldName, stringlist& FieldData)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	size_t UnitNum = 0;
	std::string FieldUnit;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		if (FieldName == Key.FieldNameList || FieldName == Key.Description)
			return false;
		if (!Handle.Data[Handle.Table][FieldName].is_array())
			return false;
		FieldData.clear();
		UnitNum = Handle.Data[Handle.Table][FieldName].size();
		if (UnitNum > 0)
		{
			for (size_t i = 0; i < UnitNum; i++)
			{
				FieldUnit = Handle.Data[Handle.Table][FieldName][i];
				FieldData.push_back(FieldUnit);
			}
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool AppendLine(DatabaseHandle& Handle, const stringlist Units)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	std::string FieldName;
	size_t LineId = 0;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		FieldName = Handle.Data[Handle.Table][Key.FieldNameList][0];
		LineId = Handle.Data[Handle.Table][FieldName].size();
		for (size_t i = 0; i < Handle.Data[Handle.Table][Key.FieldNameList].size(); i++)
		{
			FieldName = Handle.Data[Handle.Table][Key.FieldNameList][i];
			if (i < Units.size())
				Handle.Data[Handle.Table][FieldName][LineId] = Units[i];
			else
				Handle.Data[Handle.Table][FieldName][LineId] = std::string();
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool DeleteLine(DatabaseHandle& Handle, const size_t LineId)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	std::string FieldName;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		for (size_t i = 0; i < Handle.Data[Handle.Table][Key.FieldNameList].size(); i++)
		{
			FieldName = Handle.Data[Handle.Table][Key.FieldNameList][i];
			Handle.Data[Handle.Table][FieldName].erase(LineId);
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool DeleteLine(DatabaseHandle& Handle, const size_t LineId, const size_t Number)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	std::string FieldName;
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		for (size_t i = 0; i < Handle.Data[Handle.Table][Key.FieldNameList].size(); i++)
		{
			FieldName = Handle.Data[Handle.Table][Key.FieldNameList][i];
			for (size_t j = 0; j < Number; j++)
				Handle.Data[Handle.Table][FieldName].erase(LineId);
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool ChangeUnit(DatabaseHandle& Handle, const std::string FieldName, const size_t LineId, const std::string DataString)
{
	if (!Handle.Loaded)
		return false;
	JDBKey Key(Handle.DB_Description.Identification);
	switch (Handle.DB_Description.Identification)
	{
	case 1:
	case 2:
		if (!Handle.Selected)
			return false;
		if (FieldName == Key.FieldNameList || FieldName == Key.Description)
			return false;
		Handle.Data[Handle.Table][FieldName][LineId] = DataString;
		return true;
		break;
	default:
		break;
	}
	return false;
}
