# ireon.org
# this is template file that is called from client sconscript files
#
# sets pdf build targets

btrg = \
[docBuildEnv.buildFODocument('pdf/'+thisDocument_+'.fo',thisDocument_+'.xml'), \
docBuildEnv.Command('pdf/images/', docBuildEnv.getImagesPath(docBuildEnv), Copy('$TARGET', '$SOURCE')), \
docBuildEnv.Command('pdf/figures/', '${SRCDIR_}/figures/', docBuildEnv.Action(docBuildEnv.copyIncludes,docBuildEnv.copyIncludesStr)), \
docBuildEnv.buildPDFDocument('pdf/'+thisDocument_+'.pdf', 'pdf/'+thisDocument_+'.fo') ]

docBuildEnv.Alias('docs', \
docBuildEnv.Alias('docs-'+documentCategory_, \
docBuildEnv.Alias('docs-'+documentCategory_+'-'+thisDocument_, btrg )))

docBuildEnv.Alias('docs-pdf', \
docBuildEnv.Alias('docs-'+documentCategory_+'-pdf', \
docBuildEnv.Alias('docs-'+documentCategory_+'-'+thisDocument_+'-pdf', btrg )))


itrg = \
[docBuildEnv.Install(docBuildEnv['installPath_']+'/doc/'+lang_+'/'+documentCategory_+'/'+thisDocument_+'/pdf/', 'pdf/'+thisDocument_+'.pdf') ]

docBuildEnv.Alias('install-docs', \
docBuildEnv.Alias('install-docs-'+documentCategory_, \
docBuildEnv.Alias('install-docs-'+documentCategory_+'-'+thisDocument_, itrg )))

docBuildEnv.Alias('install-docs-pdf', \
docBuildEnv.Alias('install-docs-'+documentCategory_+'-pdf', \
docBuildEnv.Alias('install-docs-'+documentCategory_+'-'+thisDocument_+'-pdf', itrg )))
