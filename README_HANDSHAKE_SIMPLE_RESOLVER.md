# Handshake Simple Resolver

|.|.|
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
