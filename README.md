# JsonDatabaseToolchain

[中文](http://www.github.com/Coder33485/JsonDatabaseToolchain/blob/master/README-zh.md) | English

## Menu

### File

- `New database` Create a new database.
- `Open database` Open an existing database, you can also drag a database file to window to open the database.
- `Close database` Close the database which is opened. <u>(Secondary confirmation is not provided)</u>
- `Save database` Save the database.
- `Save as` Save the database to another location.
- `Exit` Exit the program. <u>(Secondary confirmation is not provided)</u>

### Edit

- `Rename` 
    - `Rename table` Rename any table.
    - `Rename this table` Renames the currently selected table without input `Old_Table_Name`.
    - `Rename field` Renames the field Renames the field in the currently selected table.
- `Add`
    - `Add table` Create a new table.
    - `Add field` Create a new field in the currently selected table.
    - `Add a row` Add a row of data.
    - `Add multiple rows` Add multiple rows of data.
- `Delete`
    - `Delete table` Delete an existing table.
    - `Delete field` Delete an existing field in the currently selected table.
    - `Delete a row` Delete a row of data by line number.
    - `Delete multiple rows` Delete multiple rows of data by starting line number and number of rows.
- `Description`
    - `Database description` Read or modify database description information.
    - `Table description` Read or modify table description information.

### Help

- `About` About Database Editor.

## UI

### Console

- Provides a combined list for selecting data tables.
- Some function buttons are available.

### Table data

- Displays the field names and data of the currently selected table.

## Shortcut key

- `Ctrl` + `S` Save database.

## At last

### Used third-party projects

- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++.