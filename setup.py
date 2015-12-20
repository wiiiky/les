#!/usr/bin/env python3
# encoding=utf8


from distutils.core import setup, Extension
import subprocess


def pkgconfig(*packages):
    flags = {'include_dirs': [], 'library_dirs': [], 'extra_link_args': []}

    def popen(cmds):
        p = subprocess.Popen(cmds,
                             stdout=subprocess.PIPE,
                             stderr=open('/dev/null', 'w'))
        t = p.stdout.read().strip()
        return str(t.decode('utf8'))

    for pkg in packages:
        t = popen(['pkg-config', '--cflags', '--libs', pkg])
        for f in t.split():
            if not f:
                continue
            if f.startswith('-I'):
                flags['include_dirs'].append(f[2:])
            elif f.startswith('-L'):
                flags['library_dirs'].append(f[2:])
            elif f.startswith('-l'):
                flags['extra_link_args'].append(f)
    return flags


lessph = Extension('lessph', sources=['les/les-sph.c'], **pkgconfig('sph'))
les = Extension('les', sources=['les/les.c', 'les/les-sph.c'],
                **pkgconfig('sph'))

setup(name='sph',
      version='0.1',
      description='python binding for sph',
      author='Wiky L',
      author_email='wiiiky@outlook.com',
      url='',
      ext_modules=[lessph, les],
      classifiers=[
          'Development Status :: 3 - Alpha',
          'Intended Audience :: Developers',
          'License :: OSI Approved :: GNU Lesser General Public License v3 or later (LGPLv3+)',
          'Operating System :: POSIX :: Linux',
          'Programming Language :: Python',
          ],
      )
