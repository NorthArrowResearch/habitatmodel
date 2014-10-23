# Habitat Model Console


## Folder structure:

In order to build this you need the [GCD console](https://bitbucket.org/northarrowresearch/gcd-console) alongside this one in a specific way:

```
/ 
/HabitatModel      <-- This repo
/GCD/gcd-console   <-- The GCD console repo

Win32:
======================
/GCD/build-gcp-console-Desktop_Qt_5_3_0_MSVC2010_OpenGL_32bit-Release

OSX:
======================
/GCD/gcd-console/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Release

```


## Debugging in OSX

In OSX you can specify your dependant libraries in the `.pro` file but for debugging and at runtime you will need to specify where all the dependencies live.

Do this by setting the `DYLD_FALLBACK_LIBRARY_PATH` environment variable:

```
DYLD_FALLBACK_LIBRARY_PATH=/Users/matt/Projects/nar/GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/GCDCore:/Users/matt/Projects/nar/GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/RasterManager:/Users/matt/Projects/nar/HabitatModel/build-HabitatModelCPP-Desktop_Qt_5_3_clang_64bit-Debug/ModelCore
```

You can do this on the project settings in Qt under the run tab. Look for `Run Environmentproject`