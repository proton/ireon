function makeNewFolder folderpath =
(
	astrings = filterString folderpath "\\"
	pathstring = ""
	ret = false;
	for i=1 to astrings.count do
	(
		pathstring = pathstring + astrings [i]
		ret = makeDir (pathstring)
		pathstring = pathstring + "\\"
	)
	return ret
)