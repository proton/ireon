from config_utils import *
import os
Import('*')

print 'Processing set_defaults.py ...'

# Set parameter for current platform. We do not use SCons value
# because we may want finer graining than Scons in the future
baseEnv['platform_'] = baseEnv['PLATFORM']

# Set root directory for all builders. This is *not* install directory
baseEnv['buildRoot_'] = baseEnv.Dir('#build/output/' + baseEnv['platform_'])

# Build components pickle path
baseEnv['picklePath_'] = baseEnv.File('#build/dnl.pickle')

# Root folder for installed files, relative to src_root
baseEnv['installPath_'] = os.path.normpath(baseEnv.Dir('#').get_abspath()+'/../')

# Turn on debug mode in compiled programs
baseEnv['_debugMode'] = 1

# -------------------------- utilites settings ------------------------------------

# Cheat: we use # in path to delete path and to let system search for utilites itself

# Path to wget
baseEnv['wgetPath_'] = baseEnv.File('#wget')

# Additional params to use with wget, e.g. proxy settings
baseEnv['wgetParams_'] = ''

# Remove downloaded component archives after component installation?
baseEnv['removeDownloadedComponentArchives_'] = 1

# Path to tar
baseEnv['tarPath_'] = baseEnv.File('#tar')

# Path to gzip
baseEnv['gzipPath_'] = baseEnv.File('#gzip')

print 'Done set_defaults.py'
