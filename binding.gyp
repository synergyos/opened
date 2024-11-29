{
  "targets": [
    {
      "target_name": "binding",
      "sources": ["binding.cc"],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include_dir\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
    },
    {
      "target_name": "copy",
      "type": "none",
      "dependencies": ["binding"],
      "copies": [
        {
          "destination": "<(module_root_dir)",
          "files": ["<(module_root_dir)/build/Release/binding.node"]
        }
      ]
    }
  ]
}
