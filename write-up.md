# write-up

* replace placeholder vars with your own values
* don't forget to replace `example.com` with `YOURDOMAIN.TLD`

to find all occurences you can use this, if you use the fish shell:
```
for file in (find ./*/ -type f)
  grep -Hn example.com $file
end
```

as root:
```
apt update && sudo apt -y upgrade

apt -y install dovecot-core dovecot-imapd dovecot-lmtpd postfix rspamd redis certbot ufw tmux unbound neovim bash-completion postfix-policyd-spf-perl && sudo apt update && apt -y autoremove

systemctl stop dovecot; systemctl stop postfix; systemctl stop rspamd
```

*setup a non-root user with a home dir*

example:
`useradd -m -G sudo -s /bin/bash USERNAME && echo "export EDITOR=nvim" >> /home/USERNAME/.bashrc`

`su USERNAME`

### certbot
> get a tls cert for your domain

```
certbot certonly --standalone -d mail.example.com
sudo crontab -e
@monthly certbot renew --renew-hook --force-renewal "systemctl reload dovecot; systemctl reload postfix; systemctl reload rspamd" -q

# to use 4096 bit RSA keys, add
rsa_key_size = 4096
# in /etc/letsencrypt/renewal/mail.example.com.conf
```

### unbound
> local dns resolver

```
sudo unbound-anchor -a /var/lib/unbound/root.key
sudo sytemctl reload unbound
sudo systemctl restart unbound
sudo echo "nameserver 127.0.0.1" > /etc/resolv.conf
```

### ufw
> simple firewall

`sudo ufw default deny`

⚠️⚠️⚠️`sudo ufw allow YOUR_SSH_PORT` ⚠️⚠️⚠️

```
sudo ufw allow "Dovecot IMAP"
sudo ufw allow "Dovecot Secure IMAP"
sudo ufw allow "Postfix"
sudo ufw allow "Postfix SMTPS"
sudo ufw allow "Postfix Submission"
sudo ufw allow "80,443/tcp" # letsencrypt
sudo ufw status
sudo ufw enable
```

### postfix, dovecot, rspamd
> just copying the files _should_ be fine. no guarantees or promises though!!

#### postfix aliases
> also known as mail aliases

```
# edit the alias file
# map them
sudo postmap /etc/postfix/virtual
# restart postfix
sudo postfix reload
```

#### postfix dh params
> regenerate with bigger sizes for increased security

```
mkdir /etc/postfix/dhparam/
# make sure you have enough entropy
openssl dhparam -out /etc/postfix/dhparam/postfix-dh-4096.pem -2 4096
openssl dhparam -out /etc/postfix/dhparam/postfix-dh-512.pem -2 4096
```

#### dkim key
* we use rspamd's dkim-signing module to sign our mail, for that we need a key

```
# store it somewhere
mkdir -p /etc/mail/dkim
# generate a 1024 bit key (it's only 1024 bits, cuz else the pubkey armor data would be too long for dns records)
openssl genrsa -out /etc/mail/dkim/example.com.key 1024
# rspamd needs to be able to access the file (for dkim signing) so set the permissions right
```

* you need to put the pubkey armor data into your dkim dns record (see `dns-setup.md`)

### services
```
sudo touch /etc/postfix/without_ptr
sudo touch /etc/postfix/postscreen_access
sudo postfix reload
sudo systemctl reload postfix
sudo postalias /etc/postfix/aliases
sudo postfix check

# in bash:
services=("postfix" "dovecot" "redis-server" "rspamd" "unbound" "ufw")
for srv in "${services[@]}"; do sudo systemctl enable "$srv" ; sudo systemctl start "$srv"; done
```

### misc

#### add a new mail user
> for sending mail as user@domain.tld . mail aliases can only receive mail.

```
sudo useradd -m -s /bin/bash USERNAME
sudo passwd USERNAME
# restart all the services
```

#### uptime monitoring

https://uptimerobot.com/
