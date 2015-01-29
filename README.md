# Habitat Model Console

Habitat Model Console is a C++ tool created by [North Arrow Research](http://northarrowresearch.com). 

## Usage

```
 Habitat Model Command Line
    usage: habitatmodel <root_project_path> <xml_input_file_path> <xml_output_file_path>

 Arguments:
       root_project_path: Absolute full path to existing project folder.
    xml_config_file_path: Absolute full path to existing xml config file.
    xml_output_file_path: Absolute full path to desired xml output file.

```

## Prerequisites: Qt5 and GDal > 1.10

### Qt5

You will need Qt5 but to do that you will need to add the ubuntu-sdk-team repo to your /etc/apt/sources.list

The quickest way to do all of this on Ubuntu is the following:

```
sudo apt-add-repository ppa:ubuntu-sdk-team/ppa
sudo apt-get update
sudo apt-get install qtdeclarative5-dev qt5-default
```

### GDAl 1.10

Please make sure you have GDaL 1.10 or greater installed on your system.

### RasterMan

In order to build this you need the [RasterMan](https://bitbucket.org/northarrowresearch/rasterman) library alongside this one in a specific way:


```
<PROJECTROOT>/
<PROJECTROOT>/HabitatModel/habitatmodel      <-- This repo's root
<PROJECTROOT>/RasterManager/rastermanager/   <-- The rastermanager repo root
```


## Compiling:

```
qmake -r HabitatModelCPP.pro
sudo make
```

## Debugging in OSX

In OSX you can specify your dependent libraries in the `.pro` file but for debugging and at runtime you will need to specify where all the dependencies live.

Do this by setting the `DYLD_FALLBACK_LIBRARY_PATH` environment variable:

```
DYLD_FALLBACK_LIBRARY_PATH=/Users/matt/Projects/nar/HabitatModel/build-HabitatModelCPP-Desktop_Qt_5_3_clang_64bit-Debug/ModelCore
```
This environment variable needs to specify the paths to the following compiled libraries:

* ModelCore
* RasterManager
* GCDCore

You can do this on the project settings in Qt under the run tab. Look for `Run Environmentproject`

Be sure to use different paths for `debug` and `release` libraries so you can debug things correctly.

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## Credits

### Developers

* [Philip Bailey](https://github.com/philipbaileynar)
* [Matt Reimer](https://github.com/MattReimer)


## License

[GPL V3](LICENSE.txt)