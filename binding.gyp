{
  "targets": [
    {
      "target_name": "binding",
      "sources": [ "binding.cc" ],
      "include_dirs": [ "<!(node -p \"require('path').join(require.resolve('nan'), '../../..')\")" ]
    },
    "cflags": ["-std=c++17"],
    "cflags_cc": ["-std=c++17"]
    {
      "target_name": "copy",
      "type": "none",
      "dependencies": [ "binding" ],
      "copies": [
        {
          'destination': '<(module_root_dir)',
          'files': ['<(module_root_dir)/build/Release/binding.node']
        }
      ]
    }
  ]
}
