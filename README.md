# CC-Fractal-Suite
A Suite of Fractal Cop2 Operators for Houdini!

### Current for Houdini 18.0.391

Created originally by the *Doji Dojo*

Code by [Cole Clark](http://www.coleclark.com/)

Icons by [Mihoshi Clark](https://www.mihoshiclark.com/)

## Windows Installation Guide
- Learn and download Git and CMake if you haven't already.
- Install Visual Studio 2017 with VS15 (64 Bit)
- Clone this repository
- Make a release directory on your file system for where you want the executables to live (can be anywhere)
- Run: `cmake -G "Visual Studio 15 2017 Win64" 
     {RELEASE_DIR_PATH}`
- Open the visual studio solution CMake created.
- Build! Be sure your solution configuration is set to 'Release'
- If all went well, when you dive into your release directory, you should see a couple `.dll` files in it under: `../release/{HOUDINI_VERSION}/dso`
- For Houdini to actually see the plugin, you must add the `../release/{$HOUDINI_VERSION}` directory to the `$HOUDINI_PATH` environment variable. The easiest way to do this is to add it to your Houdini version's `houdini.env` file in your user folder. An example of what you'd add is:
`HOUDINI_PATH = D:\HDK\staging\CC-Fractal-Suite\build\release\houdini18.0;&`

## Linux Installation Guide
Just do it
