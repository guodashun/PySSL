from setuptools import setup, find_packages

PACKAGES = ['pyssl'] + ["%s.%s" % ('pyssl', i) for i in find_packages('pyssl')]
setup(
	name = 'PySSL',
	version = '0.0.7',
	description = 'Basic libraries for SSL',
	packages = PACKAGES,
	package_data = {
		'pyssl.cmdIf' : ['cmdIf.pyd', 
		                 'boost_python37-vc140-mt-x64-1_67.dll',
		                 'Qt5Core.dll',
		                 'Qt5Network.dll'],
		'pyssl.visionIf' : ['visionIf.pyd', 
		                    'boost_python37-vc140-mt-x64-1_67.dll',
		                    'Qt5Core.dll',
		                    'Qt5Network.dll']
	}
)