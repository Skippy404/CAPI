# CAPI
The web API for http://yggpi.co.za/ written in C, using https://kore.io/

## Compiling & Running

To build the project, simply run,

````
$ kodev buld
````

To run the project, simply run,
````
$ kodev run
````

## Current API calls

### Login

To get a token to allow logging in, the route,

`/login?user=<username>&pass=<password>`

and a json of the form below is returned,
````
{
	code:<int>,
	token:<string>
}
````
