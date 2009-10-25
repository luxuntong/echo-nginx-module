#define DDEBUG 0

#include "ddebug.h"
#include "util.h"
#include "location.h"
#include "handler.h"

static ngx_int_t ngx_http_echo_adjust_subrequest(ngx_http_request_t *sr);

static ngx_int_t ngx_http_echo_post_subrequest(ngx_http_request_t *r,
        void *data, ngx_int_t rc);

ngx_int_t
ngx_http_echo_exec_echo_location_async(ngx_http_request_t *r,
        ngx_http_echo_ctx_t *ctx, ngx_array_t *computed_args) {
    ngx_int_t                   rc;
    ngx_http_request_t          *sr; /* subrequest object */
    ngx_str_t                   *computed_arg_elts;
    ngx_str_t                   location;
    ngx_str_t                   *url_args;

    computed_arg_elts = computed_args->elts;

    location = computed_arg_elts[0];

    if (computed_args->nelts > 1) {
        url_args = &computed_arg_elts[1];
    } else {
        url_args = NULL;
    }

    DD("location: %s", location.data);
    DD("location args: %s", (char*) (url_args ? url_args->data : (u_char*)"NULL"));

    rc = ngx_http_echo_send_header_if_needed(r, ctx);
    if (r->header_only || rc >= NGX_HTTP_SPECIAL_RESPONSE) {
        return rc;
    }

    rc = ngx_http_subrequest(r, &location, url_args, &sr, NULL, 0);
    if (rc != NGX_OK) {
        return NGX_ERROR;
    }

    rc = ngx_http_echo_adjust_subrequest(sr);
    if (rc != NGX_OK) {
        return rc;
    }

    return NGX_OK;
}

ngx_int_t
ngx_http_echo_exec_echo_location(ngx_http_request_t *r,
        ngx_http_echo_ctx_t *ctx, ngx_array_t *computed_args) {
    ngx_int_t                           rc;
    ngx_http_request_t                  *sr; /* subrequest object */
    ngx_str_t                           *computed_arg_elts;
    ngx_str_t                           location;
    ngx_str_t                           *url_args;
    ngx_http_post_subrequest_t          *psr;

    computed_arg_elts = computed_args->elts;

    location = computed_arg_elts[0];

    if (computed_args->nelts > 1) {
        url_args = &computed_arg_elts[1];
    } else {
        url_args = NULL;
    }

    rc = ngx_http_echo_send_header_if_needed(r, ctx);
    if (r->header_only || rc >= NGX_HTTP_SPECIAL_RESPONSE) {
        return rc;
    }

    psr = ngx_palloc(r->pool, sizeof(ngx_http_post_subrequest_t));
    if (psr == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    psr->handler = ngx_http_echo_post_subrequest;
    psr->data = ctx;

    rc = ngx_http_subrequest(r, &location, url_args, &sr, psr, 0);
    if (rc != NGX_OK) {
        return NGX_ERROR;
    }

    rc = ngx_http_echo_adjust_subrequest(sr);
    if (rc != NGX_OK) {
        return rc;
    }

    return NGX_OK;
}

static ngx_int_t
ngx_http_echo_post_subrequest(ngx_http_request_t *r,
        void *data, ngx_int_t rc) {
    ngx_http_echo_ctx_t         *ctx;

    ctx = data;
    ctx->next_handler_cmd++;

    return ngx_http_echo_handler(r->parent);
}

static ngx_int_t
ngx_http_echo_adjust_subrequest(ngx_http_request_t *sr) {
    ngx_http_core_main_conf_t  *cmcf;

    /* we do not inherit the parent request's variables */
    cmcf = ngx_http_get_module_main_conf(sr, ngx_http_core_module);
    sr->variables = ngx_pcalloc(sr->pool, cmcf->variables.nelts
                                        * sizeof(ngx_http_variable_value_t));

    if (sr->variables == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    return NGX_OK;
}

