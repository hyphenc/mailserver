# rspamd /etc/rspamd/local.d/worker-proxy.inc
bind_socket = "localhost:11332";
milter = yes;
timeout = 120s;
upstream "local" {
    default = yes;
    self_scan = yes;
}
