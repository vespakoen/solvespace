# /opt/homebrew/opt/cython/bin/cython --cplus slvs.pyx
python3.10 -m build .
python3.10 -m pip install dist/solvespace-3.1.0-cp310-cp310-macosx_13_0_arm64.whl --force-reinstall
/Applications/Blender.app/Contents/Resources/3.3/python/bin/python3.10 -m pip install dist/solvespace-3.1.0-cp310-cp310-macosx_13_0_arm64.whl --force-reinstall
python3.10 test