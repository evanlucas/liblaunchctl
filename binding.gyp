{
  "targets": [
    {
      "target_name": "launchctl",
			"product_prefix": "lib",
      "sources": ["liblaunchctl/liblaunchctl.c"],
      'type': 'static_library',
      "conditions": [
        ['OS=="mac"', {
          "defines": [ '__MACOSX_CORE__' ],
          'ldflags': [ '-liblaunch', '-framework CoreFoundation', '-framework System' ],
        }]
      ]
    }
  ]
}
