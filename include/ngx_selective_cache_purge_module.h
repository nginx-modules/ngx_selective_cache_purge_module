#ifndef _NGX_SELECTIVE_CACHE_PURGE_MODULE_H_
#define _NGX_SELECTIVE_CACHE_PURGE_MODULE_H_

#include <sqlite3.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_rbtree.h>
#include <ngx_http_cache.h>
#include <nginx.h>

typedef struct {
    ngx_flag_t                enabled;
    ngx_str_t                 database_filename;
    ngx_msec_t                database_cleanup_interval;
} ngx_selective_cache_purge_main_conf_t;

typedef struct {
    ngx_http_complex_value_t *purge_query;
} ngx_selective_cache_purge_loc_conf_t;

typedef struct {
    sqlite3                  *db;
    sqlite3_stmt             *insert_stmt;
    sqlite3_stmt             *delete_stmt;
    sqlite3_stmt             *delete_old_entries_stmt;
    sqlite3_stmt             *select_by_cache_key_stmt;
} ngx_selective_cache_purge_worker_data_t;

typedef struct {
    ngx_queue_t               queue;
    ngx_str_t                *zone;
    ngx_str_t                *type;
    ngx_str_t                *cache_key;
    ngx_str_t                *filename;
    int                       expires;
    ngx_str_t                *path;
    ngx_flag_t                removed;
} ngx_selective_cache_purge_cache_item_t;

typedef struct {
    ngx_queue_t              *entries;
    ngx_str_t                 purge_query;
    ngx_flag_t                remove_any_entry;
} ngx_selective_cache_purge_request_ctx_t;

typedef struct {
    ngx_rbtree_node_t         node;
    ngx_str_t                *name;
    ngx_str_t                *type;
    ngx_shm_zone_t           *cache;
} ngx_selective_cache_purge_zone_t;

// shared memory
typedef struct {
    ngx_rbtree_t              zones_tree;
} ngx_selective_cache_purge_shm_data_t;

static ngx_selective_cache_purge_main_conf_t *ngx_selective_cache_purge_module_main_conf;
static ngx_selective_cache_purge_worker_data_t *ngx_selective_cache_purge_worker_data;

static ngx_int_t ngx_selective_cache_purge_header_filter(ngx_http_request_t *r);
static ngx_int_t ngx_selective_cache_purge_handler(ngx_http_request_t *r);

ngx_http_output_header_filter_pt ngx_selective_cache_purge_next_header_filter;

ngx_shm_zone_t *ngx_selective_cache_purge_shm_zone = NULL;

static ngx_str_t ngx_selective_cache_purge_shm_name = ngx_string("selective_cache_purge_module");

ngx_event_t ngx_selective_cache_purge_database_cleanup_event;

static void ngx_selective_cache_purge_database_cleanup_timer_wake_handler(ngx_event_t *ev);

#define NGX_HTTP_FILE_CACHE_KEY_LEN 6

#define NGX_SELECTIVE_CACHE_PURGE_DATABASE_CLEANUP_INTERVAL 3600000 // 1hr

#if NGX_HTTP_FASTCGI
    extern ngx_module_t  ngx_http_fastcgi_module;
    static ngx_str_t NGX_SELECTIVE_CACHE_PURGE_FASTCGI_TYPE = ngx_string("fastcgi");
#endif /* NGX_HTTP_FASTCGI */

#if NGX_HTTP_PROXY
    extern ngx_module_t  ngx_http_proxy_module;
    static ngx_str_t NGX_SELECTIVE_CACHE_PURGE_PROXY_TYPE = ngx_string("proxy");
#endif /* NGX_HTTP_PROXY */

#if NGX_HTTP_SCGI
    extern ngx_module_t  ngx_http_scgi_module;
    static ngx_str_t NGX_SELECTIVE_CACHE_PURGE_SCGI_TYPE = ngx_string("scgi");
#endif /* NGX_HTTP_SCGI */

#if NGX_HTTP_UWSGI
    extern ngx_module_t  ngx_http_uwsgi_module;
    static ngx_str_t NGX_SELECTIVE_CACHE_PURGE_UWSGI_TYPE = ngx_string("uwsgi");
#endif /* NGX_HTTP_UWSGI */


#endif /* _NGX_SELECTIVE_CACHE_PURGE_MODULE_H_ */
