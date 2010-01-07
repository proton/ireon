import SCons.Builder
import os
import re
import urlparse
import pickle
import shutil

def getPlatformConfig(env):
	'''Returns File node of platform specific config file'''
	return env.File(env['platform_']+os.sep+env['platform_']+'_config.py')

def downloadFile(env, source, dir):
	print 'dnl: Downloading file %s ...' % source
	wgetPath = env['wgetPath_']
	cmdString = str(wgetPath) + ' --directory-prefix=' + dir + ' -nc ' + env['wgetParams_'] + ' '+ str(source)
	print 'dnl: '+ cmdString
	pipe = os.popen(cmdString)
	text = pipe.read()
	status = pipe.close()
	if status:
		print 'dnl: Error. Download failed.'
	else:
		print 'dnl: Done.'
	return status


def untarFile(env, file, targetDir):
	print 'untar: Uncompressing file %s ...' % file
	tarPath = env['tarPath_']
	gzipPath = env['gzipPath_']
	extractPath = env.Dir(targetDir)
	cmdString = str(gzipPath) + ' -d < ' + file + ' | ' + str(tarPath) + ' -xv -C '+ str(extractPath) + ' -f -'
	print 'untar: ' + cmdString
	pipe = os.popen(cmdString)
	text = pipe.read()
	status = pipe.close()
	# FIXME unsafe behaviour becouse of 'broken pipe' error
	# assume everything went ok
	status = None
	if status:
		print 'untar: Error. Uncompressing failed.'
	else:
		print 'untar: Done.'
	return status


# Target structure is: component_name target_path revision download_url sub_dir
# sub dir - directory in tar.gz file where files are packed
regexp = re.compile('^(\\S+)\\s+(\\S+)\\s+(\\d+)\\s+(\\S+)\\s+(\\S+)$')


def parsePrereqs_(target, source, env):
	'''Function downloads download definiution file, parses it and downloads
		 missing or updated components for build system'''
	print 'Parsing %s ...' % source[0]
	try:
		f=open(source[0].get_abspath(), 'r')
		url = f.readline().strip()
		f.close()
	except IOError:
		print 'Wget definition file processing failed!'
		return 1
	print 'Done.'

	downloadPath = env.Dir('./build/').get_abspath()
	if downloadFile(env, url, downloadPath):
		return 1

	try:
		# os.path.basename(urlparse.urlsplit(url)[2]) gets filename from download url
		f=open(downloadPath + os.sep + os.path.basename(urlparse.urlsplit(url)[2]), 'r')
		downloadItems = f.readlines()
		f.close()
		os.remove(downloadPath + os.sep + os.path.basename(urlparse.urlsplit(url)[2]))
	except (IOError, OSError):
		print 'Download definition file processing failed!'
		return 1

	for item in downloadItems:
		item = item.strip()

		# Skipping lines starting with '#' (comments) or empty lines
		if (len(item) == 0) or (item[0] == '#'):
			continue

		# Parse download definition file lines
		m = regexp.match(item)
		componentName = m.group(1)
		targetPath = m.group(2)
		revision = m.group(3)
		downloadUrl = m.group(4)
		# sub dir - directory in tar.gz file where files are packed
		subDir = m.group(5)
		tarPath = os.path.normpath(downloadPath+os.sep+targetPath)
		tarFileName = os.path.normpath(tarPath+os.sep+os.path.basename(urlparse.urlsplit(downloadUrl)[2]))
		fullTargetPath = os.path.normpath(tarPath+os.sep+subDir+os.sep)

		print 'Component %s ...' % componentName
		# If it is new or updated file then download
		if env['forceReinstall_'] or not env.prereqs.has_key(componentName) \
		   or (env.prereqs.has_key(componentName) and (env.prereqs[componentName][1] < revision)):
			# Try to erase target directory to make clean install. Ignore errors
			if env.prereqs.has_key(componentName):
				shutil.rmtree(env.prereqs[componentName][0])
			if downloadFile(env, downloadUrl, tarPath):
				return 1
			if untarFile(env, tarFileName, tarPath):
				return 1
			# Adding new component to register
			env.prereqs[componentName] = [fullTargetPath, revision, downloadUrl]
			if env['removeDownloadedComponentArchives_']:
				try:
					os.remove(tarFileName)
				except OSError:
					# Mmm... ok, pass it
					pass
		else:
			print 'Skipping component %s -- there is already downloaded version' % componentName
		print 'Done.'

	# Save downloaded components database
	f=open(str(env['picklePath_']), 'w')
	pickle.dump(env.prereqs, f)
	f.close()
	return None

def getParsePrereqsBuilder():
	return SCons.Builder.Builder(action = parsePrereqs_)

