{
  "targets": [
    {
      "target_name": "bsdiff",
      "sources": [
        "src/Main.cpp",
        "src/DiffWorkerCallback.cpp",
        "src/PatchWorkerCallback.cpp",
        "src/c/bsdiff/bsdiff.c",
        "src/c/bspatch/bspatch.c",
        "src/c/bzip2/bzlib.c",
        "src/c/bzip2/compress.c",
        "src/c/bzip2/crctable.c",
        "src/c/bzip2/randtable.c",
        "src/c/bzip2/blocksort.c",
        "src/c/bzip2/huffman.c",
        "src/c/bzip2/decompress.c"
      ],
      "conditions": [
        ['OS=="win"', {
          "libraries": [
            "-DelayLoad:node.exe"
          ]
        }]
      ],
      "include_dirs": [
        "include", "./src/c/bzip2",
        "<!(node -p \"require('node-addon-api').include_dir\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": [
          "UNICODE",
          "_UNICODE",
          "NAPI_VERSION=<(napi_build_version)"
      ],
      "msvs_settings": {
          "VCCLCompilerTool": {
              "ExceptionHandling": 1
          }
      },
      "msbuild_settings": {
        "ClCompile": {
          "AdditionalOptions": ["-std:c++17"]
        }
      }
    }
  ]
}
