# muteki-shims

Unofficial, free as in freedom syscall shims and headers for Besta® RTOS.

## Integrating muteki-shims into a C compiler

> [!TIP]
> When bootstrapping muteki toolchains (e.g. `arm-none-bestaeabi`), it's advised to build and install the shims using a bootstrap C compiler before building the libc and the final compiler, since libc depends on it.

```sh
meson setup --cross-file meson-cross/<your desired cross file here> build
meson compile -C build
sudo meson install -C build
```

## Integrating muteki-shims into Ghidra

Run `python scripts/gen_ghidra_prf.py <path-to-your-ghidra-user-dir>/parserprofiles/muteki-shims.prf`, and use `File -> Parse C Source...` to import the header files.

> [!NOTE]
> The prf file needs to be regenerated when the file names or layout under `include/` changes, or the project root directory is moved or renamed. Failure to do so may cause unexpected behaviors during import.
