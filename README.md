# JsonDatabaseToolchain

[中文](http://www.github.com/Coder33485/JsonDatabaseToolchain/blob/master/README-zh.md) | English

> It should be noted that all the operations mentioned below do not have a secondary confirmation link, please operate carefully.

## Menu

### File

- `New database` Create a new database.
- `Open database` Open an existing database, you can also drag a database file to window to open the database.
- `Close database` Close the database which is opened.
- `Save database` Save the database.
- `Save as` Save the database to another location.
- `Exit` Exit the program.

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

- `Ctrl` + `N` Create a new database.
- `Ctrl` + `O` Open an existing database, you can also drag a database file to window to open the database.
- `Ctrl` + `S` Save database.
- `Ctrl` + `D` Close the database which is opened.
- `Ctrl` + `K` -> `Ctrl` + `S` Save the database to another location.

## About canceling operations

> `Add a row` and `Add multiple rows` can cancel the operation when entering the first value of each line, enter '/exit' '$exit' 'exit' or not. <br/>
> All other operations that require data entry can be canceled before the final entry is completed, and a null value can be submitted in either input box. <br/>
> Other operations that do not require data entry cannot be canceled.

## About modules

Support mounting external modules for operating databases, and allow external modules to use the input box of the editor to obtain data, but the number of data bars is fixed. <br/>
The external module must have three functions: `ModuleInit` `ModuleMain` and `ModuleFree` for `Initialize Module` `Main Entry Function` and `Free Module`. <br/>
External modules can be unloaded. <br/>
Entries left over from external modules in the menu cannot be cleared for the time being. <br/>
Put the module DLL into the program directory to load automatically when the program starts.

## At last

### Used third-party projects

- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++.