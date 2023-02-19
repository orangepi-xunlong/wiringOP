# Instructions

## Build binary

`bzr builddeb -- -us -uc`

## Cleanup

`rm *.deb *.ddeb *.buildinfo *.changes *.dsc *.tar.gz *.upload`

## Rename all files

```
pwsh
gci -Recurse *wiringpi* | Rename-Item -NewName {$_.Name.Replace("wiringpi","wiringpi-opi")}
```

## Build from Source

```
bzr builddeb -S
dput <ppa> wiringpi-opi_2.27~iwj_source.changes
```

