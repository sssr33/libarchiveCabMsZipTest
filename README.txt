!PrecompiledLib folder contains another README.txt with build environment description and prebuilt .dll's.

For now CabMsZipTest::CreateCABFile just returns hardcoded path to .cab archive.
To create file use - makecab "file" "J:\TEMP\archive.cab"
Recommended archive.cab size is 100mbytes or more. Compression is MsZip which is default for makecab.