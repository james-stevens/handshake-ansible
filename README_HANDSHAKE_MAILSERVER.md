# Handshake Mail Server

The [Handshake Mail Server](https://github.com/james-stevens/handshake-mailserver) application 
is the container behind the free Handshake mail service at [ShakeTheMail.net](https://ShakeTheMail.net).

It's a little more complex to use than the Full Node or Simple Resolver, as it needs a Handshake-aware
DNS resolver service to be able to operate and you will need an SSL Certificate to support the TLS services.



## Before You Install

Before you install, take a look at the file [ansible/group_vars/handshake_mailserver/main.yaml](ansible/group_vars/handshake_mailserver/main.yaml)
and make changes. More info is in the file.

It contains the domain name you plan to use for the service and a few other options you will definitely want to change,
like the title on the website.

Each time you change this file, re-run the `do_install` script on the target server. If you start using the service,
especially if you start changing the WebMail's internal configurtion, 
some things will get baked into the system, so try and get it all sorted from day-1.

If you want to completely reset the system, you can just remove everything in the data directory, then re-run `do_install`.
Obviously, this discards all accounts and any email they might have.

`handshake_mailserver_hostname` is the server's hostname. Some mail services will get unhappy
if the hostname your mail server calls itself does not match the forward and/or reverse DNS
look up of it's IP Address.

That is, when you do a DNS look up of the host name it calls itself, that must match the IP Address
it is connecting from, and a reverse lookup on that IP Address must point back to the host name.

So you should try and ensure this hostname resolves to the IP Address you are connecting to the
outside world from.



## Your TLS Certificate and Key

The Handshake Mail Server supports TLS on all services it runs, that is HTTP, POP3, IMAP and SMTP.

You will need a Private Key and Certificate that matches your `handshake_mailserver_domain`. This domain is
used for both the website and email services. The Certificate will need to match both the domain and the 
wild card name (`*`) in the domain.

If you are using LetsEncrypt CertBot, you get this kind of certificate by specfying `-d` twice when creating the
initial request. For exmaple `-d '*.shakethemail.net' -d shakethemail.net`.

If you do not have a certificate & key, it will make one using a private certificate authority.
It will last 10 yrs, but users will get security warnings when using the service.

If you do have a publicly verifiable certificate, use the script `mailserver_update_server_pem` to encrypt
both the certificate and key into your ansible facts, by running

		./mailserver_update_server_pem <cert-file>.pem <key-file>.pem

for example

		./mailserver_update_server_pem /etc/letsencrypt/live/shakethemail.net/fullchain.pem /etc/letsencrypt/live/shakethemail.net/privkey.pem

This will also create a file in your `${HOME}` directory called `.handshake_vault_password`. This will store the key
it used to encrypt the ansible vault where the cert & key will be stored, `group_vars/handshake_mailserver/server_pem.yaml`.

It is considered safe to store encrypted ansible vaults in public resposibories, like git.

Now when you run an `install` on a mail server host, the host will also be given the key & certificate. If
there is no `server_pem.yaml` file, that step is simple skipped.

### Updating the Certificate & Key

As the certificate expires, you will need to update it. To do this, re-run your `mailserver_update_server_pem` command
to recreate the ansible vault with the new cert/key, then run

		$ PLAYBOOK=certificate_update.yaml ./do_playbook <hostname>

Alternatively, you can update the certificate & key manually on the target server by updating the file `{{ handshake_mailserver_data_directory }}/pems/server.pem`,
by default `/opt/data/handshake-mailserver/pems/server.pem`.

You will need to `cat` the certificate and key into a single file, e.g.

		cat fullchain.pem privkey.pem > /opt/data/handshake-mailserver/pems/server.pem

Every hour, the service will check if the `server.pem` has been updated and install the new cert/key if it has.


## Resolving Handshake Domains

By default, installing the Handshake Mail Server will also mean the [Simple Resolver](README_HANDSHAKE_SIMPLE_RESOLVER.md) will also
be installed. This is the quickest and easiest way to get a Handshake aware resolver (and has been tested),
but you might not want this, or you may already have other Handshake-aware resolvers.

If you already have (or want to use) existing Handshake-aware resolvers, including using existing public Handshake Resolvers,
set `common_with_handshake_simple_resolver` to `false` and define `handshake_mailserver_dns_resolvers`
as a list of the IP Addresses of the resolvers (also tested).

If you prefer to use a Handshake Full Node as a resolver, set `common_with_handshake_simple_resolver` to `false`
and set `common_with_handshake_full_node` to `true`. NOTE: this hasn't been fully tested yet.

A new full node may take some time before it can accurately resolve Handshake domain names as it
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
prevent people from spoofing email from your domain. Some mail services, like GMail, will not accept email from
a domain that does not have an SPF record.

The two `A` records on the domain itself point to a pair of load-balancers that run the TLS for the web site.

The `A` record with the wild card (`82.68.70.166`) is the public IP Address of the container running the service.

The container does provide an HTTPS service internally, so using external load-balancers is not necessary. Therefore, 
you could just have this, instead of the last three lines.

```
shakethemail.net.	600	IN	A	82.68.70.166
*.shakethemail.net.	600	IN	A	82.68.70.166
```

