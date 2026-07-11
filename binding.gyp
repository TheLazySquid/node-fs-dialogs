{
    'targets': [
        {
            'target_name': 'node-fs-dialogs',
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
            'sources': [
                'src/dialogs.cc',
                "<!@(node -p \"'nfd-ext/src/nfd_' + (process.platform === 'win32' ? 'win' : 'gtk') + '.cpp'\")"
            ],
            'include_dirs': [
                "<!@(node -p \"require('node-addon-api').include\")",
                "nfd-ext/src/include",
                "<!@(node scripts/getIncludes.js)",
            ],
            'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
            'cflags!': [ '-fno-exceptions', '-Wno-missing-field-initializers' ],
            'cflags_cc!': [ '-fno-exceptions', '-Wno-missing-field-initializers' ],
            'libraries': [
                "<!@(node scripts/getLibs.js)"
            ],
            'msvs_settings': {
                'VCCLCompilerTool': { 'ExceptionHandling': 1 },
            }
        }
    ]
}