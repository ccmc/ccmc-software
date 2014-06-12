SUBDIRS = src
#EXTRA_DIST = doc
dist-hook:
	rm -rf `find $(distdir) -type d -name .svn`
	rm -rf `find $(distdir) -type f -name .DS_Store`
	
ACLOCAL_AMFLAGS = -I m4

clean-local:
	-rm -rf kameleon_plus-*
