# CustomQuotaOss

A XRootD OSS plug-in to overwrite XRootD's base implementation for space usage statistics with calls to the lustre quota API on a singular lustre mount.

## Warning

The sources include XRootD and lustre header files (especially lustre/lustreapi.h,lustre/lustre_user.h)
and needs to be linked against the lustre api library. (-llustreapi)

## Compiling

To create the plug-in shared library, run make in the "src" directory.
For this to work, you need to set 2 environmental variables:

* Set *XRD_PATH* to the top level of your current xrootd installation.
* Set *LUSTRE_PATH* to the top level of your current lustre installation(with header files). 

This will create the shared library "LibXrdLustreOss.so"

## Rocky8 RPM build container

Use the Containerfile with podman to compile the library and package it into a rpm using:

```
podman build -t customquota .
podman run -v rpm:/rpm -it customquota
```

The el8 rpm will then be stored in the rpm directory.

## Configuration

This plug-in is loaded by the XRootD server. In order to accomplish this, you need to indicate the server where the plugin lies in the server's configuration file. Configure your lustre mountpoint and the number of seconds until the cache is invalidated.
```shell
ofs.osslib  /path/to/LibXrdCustomQuotaOss.so
CustomQuota.sourcefile /path/to/sourcefile

```
You also need to define the mount point of your Lustre FS with LustreOss.lustremount


## Usage

When using this plug-in, all high level XRootD usage statistics calls (xrdfs spaceinfo for example) will be fed lustre quota statistics of the data server's executing user's group.

## License

The XrdLustreOssWrapper plug-in is distributed under the terms of the GNU Lesser Public Licence version 3 (LGPLv3)


