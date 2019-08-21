from setuptools import setup

setup(
	name = 'PySSL',
	version = '0.0.1',
	description = 'Basic libraries for SSL',
	packages = ['cmdIf'],
	package_data = {
		'cmdIf' : ['cmdIf.pyd', 'boost_python37-vc140-mt-x64-1_67.dll']
	}
)