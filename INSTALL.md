### Installation Instructions

1. Copy the contents of the folder `houdini/vex_volumelayers` to your preferred location, here defined as `$TARGET_DIR`
2. vex-volumelayers can then be installed like any toolset that should be added to the Houdini path:
   1. Modify `vex_volumelayers.json` and set the `VEX_VOLUMELAYERS` environment variable to point to `$TARGET_DIR`.
   2. Copy `vex_volumelayers.json` into your user preferences sub-folder `houdinixx.x/packages`, or any location defined in `$HOUDINI_PACKAGES_PATH`.

   Or:

   - Add `$TARGET_DIR` to the `$HOUDINI_PATH` environment variable, either within your OS environment, or by adding it to `houdini.env` in your Houdini preferences dir.  If `$HOUDINI_PATH` hasn't been set yet, the line should look like this:
   ``` HOUDINI_PATH = $TARGET_DIR:& ```

More information on packages and environment variables in Houdini:

https://www.sidefx.com/docs/houdini/ref/plugins.html
https://www.sidefx.com/docs/houdini/basics/config_env.html



