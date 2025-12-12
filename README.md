# Handshake Ansible

This repo contains the ansible you need to install various Handshake service
on either a Debian or Ubuntu host. Currently it supports installing
- [Handshake Full Node](README_HANDSHAKE_FULL_NODE.md) (`hsd`)
- [Handshake Simple Resolver](README_HANDSHAKE_SIMPLE_RESOLVER.md)
- [Handshake Mail Server](README_HANDSHAKE_MAILSERVER.md), as in [ShakeTheMail.net](https://ShakeTheMail.net)

I tested it on Debian v13 and Ubuntu v24.04, but its likely the ansible will work
on other versions as ansible is reasonably version independent.

In each server type's case, it will install `docker`, then pull the appropriate
container(s) to run, so all the functional code is actually in the container. The
base operating system is only used as a docker host.

## Other Docker Platforms

Because all the hard work is done in the container, it should be relatively easy to 
get this to work with other platforms, e.g. RedHat / CentOS etc.

The file `roles/docker/tasks/main.yaml` splits the Docker install by platform.

If you run `./do_debug` you can find out the `distribution` name of a new platform,
then edit `roles/docker/tasks/main.yaml` to add support for it.


## Scripts

In the directory `ansible` you will find a number of scripts to run Ansible
Playbooks. The one called `do_playbook` is generally only there to be called
by other scripts. 

All script take a single parameter, which is the name of
the host(s) or host group(s) you want to run the playbook on. These are named
in your `inventory.yaml` file (see below).

`do_install` will run everything necessary to install docker, the container and
run the container.

`do_upgrade` will update both the base operating system and the container,
then restart the container or reboot the server, depending on what got upgraded.

`do_update_container_only` will only update the container and restart it if it
changed.

`do_debug` is for seeing all ansible "facts" on a host.


## Requirements

The basic paradigm of [ansible](https://docs.ansible.com/projects/ansible/latest/getting_started/introduction.html)
is that you run it from a machine, often your desktop, targetting a server, or group of servers, you wish to set up.
Writing a series of instructions to automate the configuration of a server, is often called "IaC", Infrastructure as Code.

The requirements of your server is that it runs an SSH service, you have a login and your login has
permission to `sudo` (i.e. is in the group `sudo`). You will need to know
the password of this login for `sudo` to work. You can do the initial login
either with passwords or ssh-keys, obv keys is better.

You can either put the `sudo` password into a file (default is `~/.ssh/pass`)
or you can get ansible to ask you for the password every time by editing the
script `do_playbook`.

Ansible needs Python v2 or v3 installed on the target server. For more information
see [Ansible - Managed node requirements](https://docs.ansible.com/projects/ansible/2.9/installation_guide/intro_installation.html#managed-node-requirements).

You will also need to have Python v2 or v3 installed on the machine you
plan to run the ansible from.


## Inventory.yaml

The first thing you will need to do is copy `example_inventory.yaml`
to `inventory.yaml`, then edit it to list your hosts. You do not have to
state whether they are Debian or Ubuntu, that is auto-detected by ansible.

You will need to change the initial user name (`ansible_user`) from "james"
to whatever you plan to use.


## Installing

So, for example, if (in your inventory) you have a new host called `newyork1`
in the group `handshake_full_nodes`, then running `./do_install  newyork1`
will install & run everything needed to turn it into a Handshake Full Node.

This installs & runs docker, pulls the container `jamesstevens/handshake-full-node`,
sets up a system service called `handshake-full-node` and enables & runs it.

You can now stop, start & restart the service using `systemctl`, e.g. `sudo systemctl
stop handshake-full-node` or check it's startus with `sudo systemctl status handshake-full-node`


## Customising the Install

Each service has a few options for customising the install. These options are
documented in the service specific README.

If you want to change the install option for all servers of a specific
type, then the best thing to do is edit the corresponding file in the `group_vars`
directory, e.g. `group_vars/handshake_full_nodes.yaml`

If you want to change the install options for a single host, an easy way
to do this is to add a `vars:` section to the corresponding host in the inventory.
