# JsonDatabaseToolchain

中文 | [English](http://www.github.com/Coder33485/JsonDatabaseToolchain/blob/master/README.md)

> 需要注意：以下提到的所有操作均没有二次确认环节，请慎重操作。

## 菜单

### 文件

- `新建数据库` 创建新数据库。
- `打开数据库` 打开现有数据库，也可以将数据库文件拖到窗口以打开数据库。
- `关闭数据库` 关闭打开的数据库。
- `保存数据库` 保存数据库。
- `另存为` 将数据库保存到其他位置。
- `退出` 退出程序。

### 编辑

- `重命名`
	- `重命名表` 重命名任何表。
	- `重命名当前表 `重命名当前选定的表，不输入`Old_table_Name`。
	- `重命名字段` 重命名字段重命名当前选定表中的字段。
- `添加`
	- `添加表` 创建新表。
	- `添加字段` 在当前选定的表中创建新字段。
	- `添加一行` 添加一行数据。
	- `添加多行` 添加多个数据行。
- `删除`
	- `删除表` 删除现有表。
	- `删除字段` 删除当前选定表中的现有字段。
	- `删除一行` 按行号删除一行数据。
	- `删除多行` 通过起始行号和行数删除多行数据。
- `描述`
	- `数据库描述信息` 读取或修改数据库描述信息。
	- `数据表描述信息` 读取或修改表描述信息。

### 帮助

- `关于` 关于数据库编辑器。

## 用户界面

### 操作面板

- 提供用于选择数据表的组合列表。
- 提供一些功能按钮。

### 表数据

- 显示当前选定表的字段名和数据。

## 快捷键

- `Ctrl` + `N` 新建数据库。
- `Ctrl` + `O` 加载数据库。
- `Ctrl` + `S` 保存数据库。
- `Ctrl` + `D` 关闭数据库。
- `Ctrl` + `K` -> `Ctrl` + `S` 另存为

## 关于取消操作

> `添加一行` 与 `添加多行` 在输入每行第一个值时可以取消操作，输入 `/exit` `$exit` `\exit` 均可。<br/>
> 其他所有需要输入数据的操作在最终输入完毕之前都可以取消操作，在任一输入框中提交空值即可。<br/>
> 对于其它无需输入数据的操作均不可取消。

## 关于模块

支持挂载外部模块用于操作数据库，并允许外部模块使用编辑器的输入框获取数据，但数据条数是固定的。<br/>
外部模块必须拥有三个函数：  `ModuleInit` `ModuleMain` 以及 `ModuleFree` 分别用于 `初始化模块` `主入口函数` `释放模块`。<br/>
外部模块可以卸载。<br/>
外部模块在菜单中遗留的条目暂时无法清除。<br/>
将模块dll放入程序目录以在程序启动时自动加载。

## 最后

### 用到的第三方项目

- 由DatabaseControler引用 -> [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++.
- 由DatabaseControler引用 -> [zlib-1.2.13](http://zlib.net/) - A Massively Spiffy Yet Delicately Unobtrusive Compression Library.
- 由DatabaseControler引用 -> [Crypto++](https://cryptopp.com/) - Free C++ Class Library of Cryptographic Schemes.
