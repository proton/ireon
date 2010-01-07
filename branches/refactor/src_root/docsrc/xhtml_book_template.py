# ireon.org
# this is template file that is called from client sconscript files
#
# sets xhtml build targets, used for books

if ARGUMENTS.get('buildForSite', 0):
	# target for rendering docs for site
	btrg = \
	[docBuildEnv.buildSiteXhtmlBook('index.html', thisDocument_+'.xml'), \
	docBuildEnv.Command('styles.css', '#build/docbook/styles.css', Copy('$TARGET', '$SOURCE')), \
	docBuildEnv.Command('images/', docBuildEnv.getImagesPath(docBuildEnv), Copy('$TARGET', '$SOURCE')), \
	docBuildEnv.Command('figures/', '${SRCDIR_}/figures/', docBuildEnv.Action(docBuildEnv.copyIncludes,docBuildEnv.copyIncludesStr)) ]
else:
	btrg = \
	[docBuildEnv.buildXhtmlBook('index.html', thisDocument_+'.xml'), \
	docBuildEnv.Command('styles.css', '#build/docbook/styles.css', Copy('$TARGET', '$SOURCE')), \
	docBuildEnv.Command('images/', docBuildEnv.getImagesPath(docBuildEnv), Copy('$TARGET', '$SOURCE')), \
	docBuildEnv.Command('figures/', '${SRCDIR_}/figures/', docBuildEnv.Action(docBuildEnv.copyIncludes,docBuildEnv.copyIncludesStr)) ]

docBuildEnv.Alias('docs', \
docBuildEnv.Alias('docs-'+documentCategory_, \
docBuildEnv.Alias('docs-'+documentCategory_+'-'+thisDocument_, btrg )))
docBuildEnv.Alias('docs-html', \
docBuildEnv.Alias('docs-'+documentCategory_+'-html', \
docBuildEnv.Alias('docs-'+documentCategory_+'-'+thisDocument_+'-html', btrg )))


try:
	import glob
	filesToInstall = glob.glob(docBuildEnv.Dir('').get_abspath()+'/*.html')
	itrg = \
	[docBuildEnv.Install(docBuildEnv['installPath_']+'/doc/'+lang_+'/'+documentCategory_+'/'+thisDocument_+'/', filesToInstall),\
	docBuildEnv.Command(docBuildEnv['installPath_']+'/doc/'+lang_+'/'+documentCategory_+'/'+thisDocument_+'/images/', docBuildEnv.getImagesPath(docBuildEnv), Copy('$TARGET', '$SOURCE')), \
	docBuildEnv.Command(docBuildEnv['installPath_']+'/doc/'+lang_+'/'+documentCategory_+'/'+thisDocument_+'/figures/', '${SRCDIR_}/figures/', docBuildEnv.Action(docBuildEnv.copyIncludes,docBuildEnv.copyIncludesStr)), \
	docBuildEnv.Install(docBuildEnv['installPath_']+'/doc/'+lang_+'/'+documentCategory_+'/'+thisDocument_+'/', 'styles.css') ]

	docBuildEnv.Alias('install-docs', \
	docBuildEnv.Alias('install-docs-'+documentCategory_, \
	docBuildEnv.Alias('install-docs-'+documentCategory_+'-'+thisDocument_, itrg )))

	docBuildEnv.Alias('install-docs-html', \
	docBuildEnv.Alias('install-docs-'+documentCategory_+'-html', \
	docBuildEnv.Alias('install-docs-'+documentCategory_+'-'+thisDocument_+'-html', itrg )))
except TypeError:
	pass