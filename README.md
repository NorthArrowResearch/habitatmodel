# Habitat Model Console


## Folder structure:

In order to build this you need the [GCD console](https://bitbucket.org/northarrowresearch/gcd-console) alongside this one in a specific way:

```
<PROJECTROOT>/
<PROJECTROOT>/HabitatModel/habitatmodel      <-- This repo's root
<PROJECTROOT>/GCD/gcd-console/               <-- The GCD console repo root
<PROJECTROOT>/RasterManager/rastermanager/   <-- The GCD console repo root
<PROJECTROOT>/Release/Debug(32/64)           <-- Debug executable for all projects
<PROJECTROOT>/Deploy/Release(32/64)          <-- Release executables for all projects
```


## Debugging in OSX

In OSX you can specify your dependant libraries in the `.pro` file but for debugging and at runtime you will need to specify where all the dependencies live.

Do this by setting the `DYLD_FALLBACK_LIBRARY_PATH` environment variable:

```
DYLD_FALLBACK_LIBRARY_PATH=/Users/matt/Projects/nar/GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/GCDCore:/Users/matt/Projects/nar/GCD/build-gcd-console-Desktop_Qt_5_3_clang_64bit-Debug/RasterManager:/Users/matt/Projects/nar/HabitatModel/build-HabitatModelCPP-Desktop_Qt_5_3_clang_64bit-Debug/ModelCore
```
This environment variable needs to specify the paths to the following compiled libraries:

* ModelCore
* RasterManager
* GCDCore

You can do this on the project settings in Qt under the run tab. Look for `Run Environmentproject`

Be sure to use different paths for `debug` and `release` libraries so you can debug things correctly.