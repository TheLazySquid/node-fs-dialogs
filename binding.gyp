{
    'targets': [
        {
            'target_name': 'node-fs-dialogs',
            'sources': [
                'src/dialogs.cc',
                "<!@(node -p \"'nfd-ext/src/nfd_' + (process.platform === 'win32' ? 'win' : 'gtk') + '.cpp'\")"
            ],
            'include_dirs': [
                "<!@(node -p \"require('node-addon-api').include\")",
                "nfd-ext/src/include",
                "<!@(node scripts/getLinuxIncludes.js)",
            ],
            'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
            'cflags!': [ '-fno-exceptions', '-Wno-missing-field-initializers' ],
            'cflags_cc!': [ '-fno-exceptions', '-Wno-missing-field-initializers' ],
            'libraries': [
                "<!@(node scripts/getLinuxLibs.js)"
            ],
            'msvs_settings': {
                'VCCLCompilerTool': { 'ExceptionHandling': 1 },
            }
        }
    ]
}