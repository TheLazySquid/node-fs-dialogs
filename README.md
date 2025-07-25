# Node FS Dialogs

This library provides bindings for [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended), which allows node.js to open native file dialogs for selecting files and directories.

## Installation

```bash
npm install node-fs-dialogs
```

On linux you will need to have `libgtk-3-dev` and potentially `pkg-config` installed.

## Usage

```js
import { pickFile } from "node-fs-dialogs";

const file = await pickFile({
    filters: [
        { name: "Plaintext", extensions: ["txt", "md"] },
        { name: "Javascript", extensions: "js,cjs,mjs" }
    ],
    defaultPath: process.cwd()
});

console.log(file); // "/path/to/file", or null if canceled
```

## API

`node-fs-dialogs` exports the following functions: `pickFile`, `pickFiles`, `pickDirectory`, `pickDirectories`, and `pickSaveFile`. Each function returns a promise that resolves to the selected item(s), or `null` if the user canceled the dialog.

#### `pickFile(options?: PickFileOptions): Promise<string | null>`
#### `pickFiles(options?: PickFileOptions): Promise<string[] | null>`
|Parameter|Type|Description|
|---|---|---|
|options.filter?|FileFilter \| FileFilter[]|Filters to apply to the file in the dialog. The user is still able to select files that do not match these filters, so you will still need to validate the file type after selection.|
|options.filter[i].name|string|The name of the filter, shown in the dialog.|
|options.filter[i].extensions|string \| string[]|The file extensions to filter by. Can be a comma-seperated string or an array of strings.|
|options.defaultPath?|string|The default path to open the dialog in. On windows the user's last used path will override this.|

#### `pickDirectory(options?: PickDirectoryOptions): Promise<string | null>`
#### `pickDirectories(options?: PickDirectoryOptions): Promise<string[] | null>`
|Parameter|Type|Description|
|---|---|---|
|options.defaultPath?|string|The default path to open the dialog in. On windows the user's last used path will override this.|

#### `pickSaveFile(options?: PickSaveFileOptions): Promise<string | null>`
|Parameter|Type|Description|
|---|---|---|
|options.filter?|FileFilter \| FileFilter[]|Filters to apply to the file the user is creating or selecting. The user is still able to create or select files that do not match these filters, so you will still need to validate the file type after selection.|
|options.filter[i].name|string|The name of the filter, shown in the dialog.|
|options.filter[i].extensions|string \| string[]|The file extensions to filter by. Can be a comma-seperated string or an array of strings.|
|options.defaultPath?|string|The default path to open the dialog in. On windows the user's last used path will override this.|
|options.defaultName?|string|The default name of the file.|