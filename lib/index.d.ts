declare module 'node-fs-dialogs' {
    interface FileFilter {
        title: string;
        extensions: string | string[];
    }
    
    interface FilePickerOptions {
        filters?: FileFilter | FileFilter[];
        defaultPath?: string;
    }

    export function pickFile(options?: FilePickerOptions): Promise<string | null>;
    export function pickFiles(options?: FilePickerOptions): Promise<string[] | null>;

    interface DirectoryPickerOptions {
        defaultPath?: string;
    }

    export function pickDirectory(options?: DirectoryPickerOptions): Promise<string | null>;
    export function pickDirectories(options?: DirectoryPickerOptions): Promise<string[] | null>;

    interface SaveFileOptions {
        filters?: FileFilter | FileFilter[];
        defaultPath?: string;
        defaultName?: string;
    }

    export function pickSaveFile(options?: SaveFileOptions): Promise<string | null>;
}