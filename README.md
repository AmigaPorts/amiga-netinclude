== Networking Headers for the Amiga

The headers of the Roadshow SDK for bsdsocket.library and usergroup.library.

`inline/bsdsocket.h` and `inline/usergroup.h`, which `proto/` includes, are
not here: the toolchain's build writes them with sfdc from the SDK's
`bsdsocket_lib.sfd` and `usergroup_lib.sfd`, as it writes the inline headers
of the NDK's libraries.
