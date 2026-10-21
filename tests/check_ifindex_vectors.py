#!/usr/bin/env python3
"""Compile/link the opt-in bindings and verify the four actual jump offsets."""
import argparse
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
PROBE = r"""
@HEADERS@
struct Library *SocketBase;
ULONG probe_name(const char *name) { return if_nametoindex(name); }
char *probe_index(ULONG index, char *name) { return if_indextoname(index, name); }
struct if_nameindex *probe_list(void) { return if_nameindex(); }
void probe_free(struct if_nameindex *list) { if_freenameindex(list); }
int main(void) { return 0; }
"""


def run(command):
    result = subprocess.run(command, text=True, capture_output=True)
    if result.returncode:
        raise SystemExit(" ".join(map(str, command)) + "\n" + result.stdout + result.stderr)
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", type=Path, required=True)
    args = parser.parse_args()
    toolchain = args.toolchain.resolve()
    count = 0
    with tempfile.TemporaryDirectory(prefix="ifindex-vectors-") as directory:
        work = Path(directory)
        for runtime in ("libnix", "newlib", "clib2"):
            for cpu in ("68000", "68020"):
                for lang, standard, compiler in (("c", "gnu89", "gcc"),
                                                  ("c++", "gnu++98", "g++")):
                    for order in (0, 1):
                        for regparm in (False, True):
                            common = [str(toolchain / "bin" / f"m68k-amigaos-{compiler}"),
                                      f"-m{cpu}", "-Os", "-Wall", "-Wextra", "-Werror",
                                      "-I" + str(ROOT / "include"),
                                      "-I" + str(toolchain / "m68k-amigaos/ndk-include")]
                            if runtime != "libnix":
                                common.append("-mcrt=" + runtime)
                            if regparm:
                                common.append("-mregparm=2")
                            headers = ["proto/bsdsocket.h", "proto/bsdsocket_ifindex.h"]
                            if order:
                                headers.reverse()
                            source = work / "probe.c"
                            source.write_text(PROBE.replace("@HEADERS@", "\n".join(
                                f"#include <{header}>" for header in headers)))
                            obj = work / "probe.o"
                            run(common + ["-x", lang, f"-std={standard}", "-c", str(source), "-o", str(obj)])
                            # These instruction bytes are JSR d16(a6), not
                            # unresolved ordinary C calls to a stub provider.
                            disassembly = run([str(toolchain / "bin/m68k-amigaos-objdump"),
                                               "-d", str(obj)])
                            for displacement in ("fc8e", "fc88", "fc82", "fc7c"):
                                if "4eae " + displacement not in disassembly:
                                    raise SystemExit("Missing proposed vector jump: " + displacement)
                            example = work / "IfNames.o"
                            run(common + ["-x", lang, f"-std={standard}", "-c",
                                          str(ROOT / "examples/IfNames.c"), "-o", str(example)])
                            run(common + [str(obj), str(example), "-o", str(work / "probe")])
                            count += 1
        # Ordinary proto inclusion must never silently select the new ABI.
        # Both existing declaration/stub opt-outs must keep working.
        for mode in ("ordinary", "__NO_NET_API", "_NO_INLINE"):
            for lang, compiler in (("c", "gcc"), ("c++", "g++")):
                source = work / "suppressed.c"
                text = "" if mode == "ordinary" else "#define " + mode + "\n"
                text += "#include <proto/bsdsocket.h>\n#include <net/if.h>\n"
                if mode != "ordinary":
                    text += "#include <proto/bsdsocket_ifindex.h>\n"
                for name in ("if_nametoindex", "if_indextoname", "if_nameindex", "if_freenameindex"):
                    text += "#ifdef " + name + "\n#error Unexpected vector binding\n#endif\n"
                source.write_text(text)
                run([str(toolchain / "bin" / f"m68k-amigaos-{compiler}"),
                     "-x", lang, "-m68000", "-Werror", "-I" + str(ROOT / "include"),
                     "-I" + str(toolchain / "m68k-amigaos/ndk-include"),
                     "-c", str(source), "-o", str(work / "suppressed.o")])
        print(f"ifindex vectors: {count} vector compile/link, example compilation and machine-code checks; 6 opt-in/suppression checks passed")


if __name__ == "__main__":
    main()
