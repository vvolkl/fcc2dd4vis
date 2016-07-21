Usage
======

Convert a FCCSW data file to DDG4 format:

```{bash}
mkdir build
cd build && cmake ..
make
./fcc2dd4vis <path to ddeve xml config file>
```


Filenames are currently hardcoded, `output.root` for FCCSW input files (as this is the default name for FCCSW output files) and `fccvis_converted.root` for output.
The DDEve viewer is started by the program. An optional argument is the DDEve configuration xml.
Event Data  can then loaded from the gui menu, selecting the output `test.root` from the conversion.
