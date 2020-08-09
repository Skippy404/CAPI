#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>

int	login(struct http_request *);
int	pass(struct http_request *);

int
login(struct http_request *req)
{
	char *user;
	struct kore_buf *buff;

	http_populate_get(req);
	buff = kore_buf_alloc(128);


	if (http_argument_get_string(req, "id", &user)) {
		kore_buf_appendf(buff, "{\"id\":%s}" , user);
		printf("ID=%s\n", user);
	} else {
		http_response(req, 500, NULL, 0);
	}
	
	// Set the response header
	http_response_header(req, "Content-Type", "application/json");
	http_response(req, 200, buff->data, buff->offset);
	kore_buf_free(buff);
	return (KORE_RESULT_OK);
}

// Recieve a POST
int
pass(struct http_request *req)
{
	char *pass, *user;
	struct kore_buf *buff;

	http_populate_get(req);
	buff = kore_buf_alloc(128);

	printf("attempted login recieved\n");
	if (req->method == HTTP_METHOD_POST) {
		printf("POST detected\n");
	}


	if (http_argument_get_string(req, "pass", &pass) &&
		http_argument_get_string(req, "user", &user)) {
		kore_buf_appendf(buff, "{\"user\":%s,\"pass\":%s}" , user, pass);
		printf("login attempt recieved, user=%s, pass=%s\n", user, pass);
	} else {
		http_response(req, 500, NULL, 0);
	}
	
	http_response(req, 200, buff->data, buff->offset);
	kore_buf_free(buff);
	return (KORE_RESULT_OK);
}
