# Configuration file for ireon.org build system
#
# Do not modify this file, use userconfig.py in
#	src_root directory to overwrite options

# Sets win32 specific options, overrides default options

Import('*')

print 'Processing win32_config.py ...'

# Path to wget
baseEnv['wgetPath_'] = baseEnv.File('#build/win32/tools/wget')

# Path to tar
baseEnv['tarPath_'] = baseEnv.File('#build/win32/tools/tar')

# Path to gzip
baseEnv['gzipPath_'] = baseEnv.File('#build/win32/tools/gzip')

print 'Done win32_config.py'
