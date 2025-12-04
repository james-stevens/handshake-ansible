# Handshake Simple Resolver

|<!-- -->|<!-- -->|
|---------|---------|
|Container|jamesstevens/handshake-simple-resolver|
|System Service|handshake-simple-resolver|


This container will provide a resolver service for ICANN, Handshake & ETH names.

It does **NOT** resolve the names itself, but it relies on open public resolvers. However, 
it will cache the answers it gets and hides from the pubic resolver exactly who the query came from.

It reads the list of public resolvers from the host name `recs.hogshake.net`, and will automatically
change its configuration if the list of IP Addresses changes.

If any of the IP Addresses ceases to service, it will automatically stop using it. It will also automatically
detect which public resolvers respond the fasest and prefer using those. This will normally mean the ones
that are physically closest to you.

It will also take advanatgee of the cache at the public resolver to get answers faster.

This is by far the fastest and easiest way to get a local Handshake resolver, even though it does rely on
other people to actually work.


If you wish you can then front this with a DoH proxy to get either a JSON or binary DoH serivce.

## Options

### handshake_simple_resolver_docker_image
string: name of the container to pull and run for this service

		handshake_simple_resolver_docker_image: jamesstevens/handshake-simple-resolver

### handshake_simple_resolver_memory_cap
string, docker memory specification: Cap on the memory this container can use

		handshake_simple_resolver_memory_cap: 1g

### handshake_simple_resolver_require_dns_cookies
boolean: Require clients to use DNS cookies, normal clients are likely to do this, abuse traffic less so

		handshake_simple_resolver_require_dns_cookies: true

### handshake_simple_resolver_server_only_access
boolean: When `true` the container only allows access from 127.0.0.0/8 (localhost)

		handshake_simple_resolver_server_only_access: false

### handshake_simple_resolver_allowed_subnets
IP Subnet List: List of subnet allowed to use this resolver, use `- any` to mean anybody

		 handshake_simple_resolver_allowed_subnets:
			 - 192.168.0.0/16
			 - 10.0.0.0/8
			 - 172.16.0.0/12
			 - 127.0.0.0/8
