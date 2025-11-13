# Handshake Ansible

This repo contains the ansible you need to install various Handshake service
on either a Debian or Ubuntu host. Currently it only supports installing a
Handshake Node (`hsd`).

I tested on Debian v13 and Ubuntu v24.04, but its likely the ansible will work
on other versions as ansible is reasonably version independent.

In each server type's case, it will install `docker`, then pull the appropriate
container(s) to run, so all the functional code is actually in the container. The
base operating system is only used as a docker host.

## Other Docker Platforms

Becuase all the hard work is done in the container, it should be relatively easy to 
get this to work with other platforms.

The file `roles/docker/tasks/main.yaml` splits the Docker install by platform.

If you run `./do_debug` you can find out the `distribution` name of a new platform,
then edit `roles/docker/tasks/main.yaml` to add suppport for it.

## Scripts

In the directory `ansible` you will find a number of scripts to run Ansible
Playbooks. The one called `do_playbook` is generally only there to be called
by other scripts. All script take a single parameter, which is the name of
the host(s) or host group(s) you want to run the playbook on. These are named
in your `inventory.yaml` file (see below).

`do_install` will run everything necessary to install docker, the container and
run the container.

`do_upgrade` will update both the base operating system and the container,
then restart the container or reboot the server, depending on what got upgraded.

`do_update_container_only` will only update the container and restart it if it
changed.

`do_debug` is for seeing all “facts” on a host.

## Inventory.yaml

The first thing you will need to do is copy `example_inventory.yaml`
to `inventory.yaml`, then edit it to list your hosts. You do not have to
state whether they are Debian or Ubuntu, that is auto-detected by ansible.

The requirements of your server are that you have a login and your login has
permission to `sudo` (i.e. is in the group `sudo`). You will need to know
the password of this login for `sudo` to work. You can do the initial login
either with passwords or ssh-keys, obv keys is better.

You can either put the `sudo` password into a file (default is `~/.ssh/pass`)
or you can get ansible to ask you for the password every time by editing the
script `do_playbook`.

## Installing

So, for example, if (in your inventory) you have a new host called `newyork1`
in the group `handshake_full_nodes`, then running `./do_install  newyork1`
will install & run everything needed to turn it into a Handshake Full Node.

This installs & runs docker, pulls the container `jamesstevens/handshake-full-node`,
sets up a system service called `handshake-full-node` and enables & runs it.

You can now stop, start & restart the service using `systemctl`, e.g. `sudo systemctl
stop handshake-full-node` or check it's startus with `sudo systemctl status handshake-full-node`

## Handshake Full Node

The Handshake Full Node is simply that - it's just `hsd` running in a container. 

If you use my container (`jamesstevens/handshake-full-node`), to help with the
DNS it also has a DNS proxy that can split the incoming DNS queries based on the `RD` bit.
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
| `handshake_full_nodes_hsd_public_node` | `12038` & `44806` | `0.0.0.0:12038` & `0.0.0.0:44806`
| `handshake_full_nodes_dns_auth_public` | `5349` | `0.0.0.0:53`
| `handshake_full_nodes_dns_recurse_public` | `5350` | `0.0.0.0:53`
| `handshake_full_nodes_dns_both_public` | `53` | `0.0.0.0:53`
| `handshake_full_nodes_hsd_private_node` | `12038` & `44806` | `127.0.0.1:12038` & `127.0.0.1:44806`
| `handshake_full_nodes_dns_auth_private` | `5349` | `127.0.0.1:53`
| `handshake_full_nodes_dns_recurse_private` | `5350` | `127.0.0.1:53`
| `handshake_full_nodes_dns_both_private` | `53` | `127.0.0.1:53`

**NOTE**: Only one of the six DNS options, and one of the two `hsd` options,
can be `true` at a time, or the container will fail to start.

If you change any of these options, you can simply re-run the `do_install` script and it will
make the necessary changes and restart the container to reflect the new situaution.
