#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <string.h>

int	login(struct http_request *);
int	pass(struct http_request *);

char buffer[256];

int
start(int state)
{
	// Gets loaded at the start, all workers will have "Hello, World!" -> buffer
	printf("LOADING...\n");
	sprintf(buffer, "Hello, World!");
	printf("LOADED!\n%s\n", buffer);
	return KORE_RESULT_OK;
}

int
login(struct http_request *req)
{
	char *user, *pass;
	char cbuff[256];
	struct kore_buf *buff;

	http_populate_get(req);
	buff = kore_buf_alloc(128);

	if (http_argument_get_string(req, "pass", &pass) && 
		http_argument_get_string(req, "user", &user)) {
		// Just a temp return to test that the get works, temp token
		// TODO check against DB if valid login
		kore_buf_appendf(buff, "{\"code\":0,\"token\":\"%s%s\"}" , user, pass);
	} else {
		http_response_header(req, "Content-Type", "application/json");
		sprintf(cbuff, "{\"code\":1,\"token\":null}");
		kore_buf_append(buff, cbuff, strlen(cbuff));
		http_response(req, 401, buff->data, buff->offset);
		return (KORE_RESULT_OK);
	}
	
	// Set the response header to json format
	http_response_header(req, "Content-Type", "application/json");
	http_response(req, 200, buff->data, buff->offset);

	kore_buf_free(buff); // good bois dont create mem leaks
	return (KORE_RESULT_OK);
}

// Recieve a POST
int
pass(struct http_request *req)
{
	char *user;
	struct kore_buf *buff;

	http_populate_post(req);
	buff = kore_buf_alloc(256);

	printf("attempted login recieved\n");
	if (req->method == HTTP_METHOD_POST) {
		printf("POST detected\n");
	}

	if (http_argument_get_string(req, "user", &user)) {
		kore_buf_appendf(buff, "{\"user\":%s}" , user);
		printf("login attempt recieved, user=%s\n", user);
	} else {
		printf("ERROR: FNF\n");
		http_response(req, 500, "ERROR\n", 0);
		return (KORE_RESULT_OK);
	}
	
	http_response(req, 200, buff->data, buff->offset);
	kore_buf_free(buff);
	return (KORE_RESULT_OK);
}
