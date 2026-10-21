#!/usr/bin/env python3
"""Compile, link and execute the portable SDK ABI test with volamos."""
import argparse
from contextlib import ExitStack
import json
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--volamos", type=Path, required=True)
    parser.add_argument("--toolchain", type=Path, help="GCC toolchain, default libnix runtime")
    parser.add_argument("--sasc", type=Path, help="SAS/C installation containing c/, lib/, libs/, include/")
    parser.add_argument("--output-dir", type=Path, help="new directory for binaries and logs")
    args = parser.parse_args()
    if not (args.toolchain or args.sasc):
        parser.error("specify --toolchain and/or --sasc")
    volamos = str(args.volamos.resolve())
    with ExitStack() as stack:
        if args.output_dir:
            work = args.output_dir.resolve()
            work.mkdir(parents=True)
        else:
            work = Path(stack.enter_context(tempfile.TemporaryDirectory(prefix="socket-abi-")))
        results = []

        def run(label, command):
            result = subprocess.run(list(map(str, command)), text=True, capture_output=True, timeout=90)
            (work / (label + ".log")).write_text(result.stdout + result.stderr)
            results.append({"step": label, "returncode": result.returncode})
            (work / "results.json").write_text(json.dumps(results, indent=2) + "\n")
            if result.returncode:
                raise SystemExit(label + ":\n" + result.stdout + result.stderr)

        if args.toolchain:
            toolchain = args.toolchain.resolve()
            run("gcc-assemble", [toolchain / "bin/vasmm68k_mot", "-quiet", "-Fhunk", "-m68000",
                "-o", work / "gcc-vectors.o", ROOT / "tests/ifindex_vectors_68k.s"])
            for cpu in ("68000", "68020"):
                for order in (0, 1):
                    name = f"gcc-{cpu}-{order}"
                    command = [toolchain / "bin/m68k-amigaos-gcc", "-m" + cpu, "-Os",
                        "-Wall", "-Wextra", "-Werror", "-DTEST_REVERSE_INCLUDE=" + str(order),
                        "-I" + str(ROOT / "include")]
                    ndk = toolchain / "m68k-amigaos/ndk-include"
                    if ndk.is_dir():
                        command.append("-I" + str(ndk))
                    run(name + "-link", command + [ROOT / "tests/socket_runtime.c",
                        ROOT / "examples/IfNames.c", ROOT / "support/in6_addr.c",
                        work / "gcc-vectors.o", "-o", work / name])
                    run(name + "-execute", [volamos, "--cpu", cpu, work / name])
            print("GCC: 4 compile/link and volamos executions passed")

        if args.sasc:
            sasc = args.sasc.resolve()
            common = [volamos, "--cpu", "68020", "--stack", "256K", "--ram", "32M",
                "--libs-dir", sasc / "libs", "--cwd", "WORK:"]
            for volume, path in [("SC", sasc), ("LIB", sasc / "lib"),
                    ("INCLUDE", sasc / "include"), ("CXXINCLUDE", sasc / "cxxinclude"),
                    ("WORK", work), ("SDK", ROOT / "include"), ("TEST", ROOT / "tests"),
                    ("EXAMPLE", ROOT / "examples"), ("SUPPORT", ROOT / "support")]:
                common += ["-V", volume + ":" + str(path)]
            run("sasc-assemble", common + [sasc / "c/asm", "-oWORK:sasc-vectors.o",
                "TEST:ifindex_vectors_68k.s"])
            for cpu in ("68000", "68020"):
                for order in (0, 1):
                    name = f"sasc-{cpu}-{order}"
                    for obj, source in [("runtime", "TEST:socket_runtime.c"),
                            ("example", "EXAMPLE:IfNames.c"), ("constants", "SUPPORT:in6_addr.c")]:
                        run(name + "-" + obj, common + [sasc / "c/sc", "includedir=SDK:",
                            "includedir=TEST:", "cpu=" + cpu, "define=TEST_REVERSE_INCLUDE=" + str(order),
                            "objectname=WORK:" + obj + ".o", source])
                    run(name + "-link", common + [sasc / "c/slink", "FROM",
                        "LIB:c.o+WORK:runtime.o+WORK:example.o+WORK:constants.o+WORK:sasc-vectors.o",
                        "TO", "WORK:" + name, "LIB", "LIB:sc.lib+LIB:amiga.lib"])
                    run(name + "-execute", [volamos, "--cpu", cpu, work / name])
            print("SAS/C: 4 compile/link and volamos executions passed")


if __name__ == "__main__":
    main()
