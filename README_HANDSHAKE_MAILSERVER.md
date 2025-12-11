# Handshake Mail Server

The [Handshake Mail Server](https://github.com/james-stevens/handshake-mailserver) application 
is the container behind the free Handshake mail service at [ShakeTheMail.net](https://ShakeTheMail.net).

It's a little more complex to use than the Full Node or Simple Resolver, as it needs a Handshake-aware
DNS resolver service to be able to operate and you will need an SSL Certificate to support the TLS services.

If you do not have a certificate, it will make one using a private certificate authority, so users
will get security warnings when using the service.

## Before You Install

Before you install, take a look at the file [ansible/group_vars/handshake_mailserver.yaml](ansible/group_vars/handshake_mailserver.yaml)
and make changes.

It contains the domain name you plan to use for the service and a few other options you will definitely want to change,
like the title on the website.

Each time you change this file, re-run the `do_install` script on the target server.

`handshake_mailserver_hostname` is the server's hostname. Some mail services will get unhappy
if the hostname your mail server calls itself does not match the forward and/or reverse DNS
look up of it's IP Address.

That is, when you do a DNS look up of the host name it calls itself, that must match the IP Address
it is connecting from, and a reverse lookup on that IP Address must point back to the host name.

So you should try and ensure this hostname resolves to the IP Address you are connecting to the
outside world from.



## Resolving Handshake Domains

By default, installing the Handshake Mail Server will also mean the [Simple Resolver](README_HANDSHAKE_SIMPLE_RESOLVER.md) will also
be installed. This is the quickest and easiest way to get a Handshake aware resolver (and has been tested),
but you might not want this, or you may already have other Handshake-aware reaolvers.

If you already have other Handshake-aware reaolvers, set `common_with_handshake_simple_resolver` to `false`
and define `handshake_mailserver_dns_resolvers` as a list of the IP Addresses of your resolvers.

If you prefer to use a Handshake Full Node as a resolver, set `common_with_handshake_simple_resolver` to `false`
and set `common_with_handshake_full_node` to `true`. NOTE: this hasn't been fully tested yet.

A new full node may take soem time before it can accurately resolve Handshake domain names as it
will need to fully seed the blockchain first.

## `handshake_mailserver_domain`

This is the domain you plan to use for hosting the service. This will be the name of the website,
the domain for user's default email addresses and the domain user's MX codes will live in.

For the service to work correctly, the DNS of the hosting domain must be set up correctly. However, 
that's quite a simple task.

Here's the DNS for the service at ShakeTheMail.net

```
shakethemail.net.	86400	IN	SOA	ns1.namekshake.net. hostmaster.shakethemail.com. 1765469020 10800 3600 604800 3600
shakethemail.net.	86400	IN	NS	ns1.nameshake.net.
shakethemail.net.	86400	IN	NS	ns2.nameshake.net.
shakethemail.net.	86400	IN	NS	ns3.nameshake.net.
shakethemail.net.	600	IN	TXT	"v=spf1 ip4:82.68.70.166 ip4:82.68.70.174 -all"
shakethemail.net.	600	IN	A	82.68.70.162
shakethemail.net.	600	IN	A	82.68.70.163
*.shakethemail.net.	600	IN	A	82.68.70.166
```

The `SOA` and `NS` records are about controlling where the domain is published from, so will be up to you.

The `TXT` record specifies which IP Addresses email for this domain is allowed to originate from. This is to help
prevent people from spoofing email from your domains. Some mail services, like GMail, will not accept email from
a domain that does not have an SPF record.

The two `A` records on the domain itself point to a pair of load-balancers that host the web site.

The `A` record with the wild card (`82.68.70.166`) is the public IP Address of the container running the service.

The container will provide an HTTPS service internally, so using eternal load-balancers is not necessary. Therefore, 
you could just have

```
shakethemail.net.	600	IN	A	82.68.70.166
*.shakethemail.net.	600	IN	A	82.68.70.166
```

