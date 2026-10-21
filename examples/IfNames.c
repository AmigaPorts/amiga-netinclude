/* Exercise the proposed common interface-index vectors.
 * Requires a library implementing this exact ABI.
 * A table-size or version check alone is not sufficient.
 */
#include <exec/libraries.h>
#include <proto/bsdsocket_ifindex.h>

/* Interface names must terminate within IF_NAMESIZE. Keep the example
 * independent of a C runtime's out-of-line string-function stubs.
 */
static int SameInterfaceName(const char *a, const char *b)
{
    unsigned int i;
    for (i = 0; i < IF_NAMESIZE; i++) {
        if (a[i] != b[i])
            return 0;
        if (a[i] == '\0')
            return 1;
    }
    return 0;
}

/* The parameter shadows the usual external base for these direct calls.
 * Returns zero on success and -1 on failure.
 */
int CheckInterfaceNames(struct Library *SocketBase)
{
    struct if_nameindex *names;
    char name[IF_NAMESIZE];
    ULONG i;
    int result = 0;

    if (SocketBase == NULL || SocketBase->lib_NegSize < 0x384)
        return -1;
    names = if_nameindex();
    if (names == NULL)
        return -1;
    for (i = 0; names[i].if_index != 0; i++) {
        if (names[i].if_name == NULL ||
            if_nametoindex(names[i].if_name) != names[i].if_index ||
            if_indextoname(names[i].if_index, name) == NULL ||
            !SameInterfaceName(name, names[i].if_name))
            result = -1;
    }
    if (names[i].if_name != NULL ||
        if_nametoindex("interface-name-longer-than-IF_NAMESIZE") != 0 ||
        if_indextoname(0, name) != NULL)
        result = -1;
    /* RFC 3493 section 4.4: free the array returned by if_nameindex(),
     * including a valid list containing only its terminator.
     */
    if_freenameindex(names);
    return result;
}
