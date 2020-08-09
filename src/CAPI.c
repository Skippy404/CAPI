#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>

int	login(struct http_request *);
int	pass(struct http_request *);

int
login(struct http_request *req)
{
	char *user, *id;
	struct kore_buf *buff;

	http_populate_get(req);
	buff = kore_buf_alloc(128);

	if (http_argument_get_string(req, "id", &id) && 
		http_argument_get_string(req, "name", &user)) {
		kore_buf_appendf(buff, "{\"name\":\"%s\",\"id\":%s}" , user, id);
	} else {
		http_response(req, 500, NULL, 0);
		return (KORE_RESULT_OK);
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
