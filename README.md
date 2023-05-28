# Bulky

This simple reusable utility to make bulk http request to any server.

**THIS UTILITY SHOULD BE USED ONLY FOR TESTING**

Most of the python script that "put load" on your server are just a joke, they are running 100% cpu and sending ~300 request per second.
By using C and [libuv](http://docs.libuv.org/en/v1.x/index.html), you should be able to send much more request.
Also there are two header files, that are can be used to create and parse http requests.

This is mainly example project to show what is possible.

## Features

* [x] HTTP request generation API
* [x] HTTP response parsing API
* [x] Simle example
* [ ] OpenSSL based tls support to send https requests
* [ ] Chained request example
* [ ] Interactive controls
  * [ ] Spawn more threads
  * [ ] Increase # request per second

