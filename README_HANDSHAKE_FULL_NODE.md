# Handshake Full Node

|Container|jamesstevens/handshake-full-node
|System Service|handshake-full-node

The Handshake Full Node is simply that - it's just a pre-built copy of `hsd` running [in a container](https://github.com/james-stevens/handshake-full-node). 

The container has been hardened and designed to run read-only.

If you use my container (`jamesstevens/handshake-full-node`), to help with the
DNS it also has a very lightweight DNS proxy that can split the incoming DNS queries based on the `RD` flag.

So queries with `RD=0` will get sent to `hsd`'s Auth port (`5349`) and `RD=1` queries
go to the recursive port (`5350`). The proxy listens on port `53`.

The container start-script can map the container's ports `5349`, `5350` or `53` to the
host's port `53`, either on address `0.0.0.0` or `127.0.0.1`, depending if you enable
the service as public or private. The default is to make the `hsd` ports public
and the dns-proxy's port (`53`) private (`127.0.0.1`).

The file `group_vars/handshake_full_nodes.yaml` has a few more variables you can
tweak. This includes running a different container, if you wish.

**NOTE**: Some of the options in the `group_vars` file may not work, if you use a different
container.

**NOTE**: When you first run it, it will need to seed the blockchain, before it can
do anything useful, this can take some time.

## Port Mapping Options
| Option Name | Container Port | Mapped to Host|
|-------------|----------------|---------------|
| `handshake_full_node_hsd_public_node` | `12038` & `44806` | `0.0.0.0:12038` & `0.0.0.0:44806`
| `handshake_full_node_dns_auth_public` | `5349` | `0.0.0.0:53`
| `handshake_full_node_dns_recurse_public` | `5350` | `0.0.0.0:53`
| `handshake_full_node_dns_both_public` | `53` | `0.0.0.0:53`
| `handshake_full_node_hsd_private_node` | `12038` & `44806` | `127.0.0.1:12038` & `127.0.0.1:44806`
| `handshake_full_node_dns_auth_private` | `5349` | `127.0.0.1:53`
| `handshake_full_node_dns_recurse_private` | `5350` | `127.0.0.1:53`
| `handshake_full_node_dns_both_private` | `53` | `127.0.0.1:53`

**NOTE**: Only one of the six DNS options, and one of the two `hsd` options,
can be `true` at a time, or the container will fail to start.

If you change any of these options, you can simply re-run the `do_install` script and it will
make the necessary changes and restart the container to reflect the new situaution.
