# This program generates empty Visual Studio test project file.
#
# Intended usage:
# 1) Generate new project file from template with this program.
# 2) Include newly generated project into tests.sln solution.
# 3) Write unit tests in test project.


# Copyright (C) 2005 ireon.org developers council
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.


import sys
import os.path

projectsTemplateFile = 'template_project/template.vcproj'

# Get project name and check it
projName = raw_input('Enter new project name (Enter to quit): ')

if (not projName):
	sys.exit(0)

if (os.path.exists(projName+'.vcproj')):
	print 'Project file with name '+projName+'.vcproj already exists!'
	raw_input('Press Enter to exit')
	sys.exit(1)


# Read template
templateLines = ''
try:
	f=open(projectsTemplateFile, 'r')
	templateLines = f.readlines()
	f.close()
except (IOError, OSError):
	print 'Could not open project template!'
	raw_input('Press Enter to exit')
	sys.exit(1)


# Process template, substitute variables
outputLines = ''
for line in templateLines:
	line = line.replace('%%PROJNAME%%', projName)
	outputLines += line


# Write new project file
try:
	f=open(projName+'.vcproj', 'w')
	f.writelines(outputLines)
	f.close()
except (IOError, OSError):
	print 'Could not write project file!'
	raw_input('Press Enter to exit')
	sys.exit(1)

print 'Done.'




