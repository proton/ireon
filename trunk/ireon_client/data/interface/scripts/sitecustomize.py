import locale, sys
#encoding = locale.getdefaultlocale()[1]
encoding = 'utf-8'
print encoding
if encoding:
	sys.setdefaultencoding(encoding)