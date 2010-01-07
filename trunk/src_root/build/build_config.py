Import('*')
from config_utils import *
import pickle

# Det default Parameters
SConscript('set_defaults.py', exports='baseEnv')

print 'Processing build_config.py ...'

# Adding prereqs dictionary to enviroment
from SCons.Script.SConscript import SConsEnvironment
SConsEnvironment.prereqs = {}

# Try to pickle already downloaded and installed files database
try:
	f=open(str(baseEnv['picklePath_']), 'r')
	baseEnv.prereqs = pickle.load(f)
	f.close()
except IOError:
	pass


parsePrereqsBuilder = getParsePrereqsBuilder();
baseEnv.Append(BUILDERS = {'parsePrereqs' : parsePrereqsBuilder})
#Alias('install-prereqs', Alias('install-prereqs-common', baseEnv.parsePrereqs('dummy', 'common.wget')))
Alias('install-prereqs', Alias('install-prereqs-docbook', Alias('install-prereqs-docbook-html', baseEnv.parsePrereqs('dbh-dummy', 'docbook.wget'))))
Alias('install-prereqs', Alias('install-prereqs-docbook', Alias('install-prereqs-docbook-pdf', baseEnv.parsePrereqs('dbp-dummy', 'fop.wget'))))
Alias('install-prereqs-docbook-pdf','install-prereqs-docbook-html')

print 'Done build_config.py'

# Call platform specific configuration script
SConscript(str(getPlatformConfig(baseEnv)), exports='baseEnv')
