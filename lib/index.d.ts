declare module 'node-fs-dialogs' {
    export interface FileFilter {
        name: string;
        extensions: string | string[];
    }
    
    export interface FilePickerOptions {
        filters?: FileFilter | FileFilter[];
        defaultPath?: string;
        title?: string;
        acceptLabel?: string;
        cancelLabel?: string;
    }

    export function pickFile(options?: FilePickerOptions): Promise<string | null>;
    export function pickFiles(options?: FilePickerOptions): Promise<string[] | null>;

    export interface DirectoryPickerOptions {
        defaultPath?: string;
        title?: string;
        acceptLabel?: string;
        cancelLabel?: string;
    }

    export function pickDirectory(options?: DirectoryPickerOptions): Promise<string | null>;
    export function pickDirectories(options?: DirectoryPickerOptions): Promise<string[] | null>;

    export interface SaveFileOptions {
        filters?: FileFilter | FileFilter[];
        defaultPath?: string;
        defaultName?: string;
        title?: string;
        acceptLabel?: string;
        cancelLabel?: string;
    }

    export function pickSaveFile(options?: SaveFileOptions): Promise<string | null>;
}