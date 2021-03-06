# debian mailserver

notes on setting up a mailserver on debian using postfix, dovecot and rspamd (+ ufw, unbound)

## you should (have)
* a clean, updated debian install
* a non-root user
* dns records setup as described in dns-setup.md
* replace `example.com` in the config files with your `domain.tld`
* read write-up.md so you actually _kind of_ know what you're doing.

## further reading
* https://doc.dovecot.org/
* http://www.postfix.org/documentation.html
* https://rspamd.com/doc/configuration/
* https://wiki.archlinux.org/index.php/Mail_server
* https://wiki.archlinux.org/index.php/Postfix
* https://poolp.org/posts/2019-09-14/setting-up-a-mail-server-with-opensmtpd-dovecot-and-rspamd/
* https://mecsa.jrc.ec.europa.eu/en/postfix

### useful tools
* https://www.dmarcanalyzer.com/dkim/dkim-check/
* https://mxtoolbox.com/
* https://ssl-tools.net/mailservers
* https://ssl-tools.net/mails
* https://www.checktls.com/index.html
* https://www.whatsmydns.net/
* https://www.nslookup.io/
* https://mecsa.jrc.ec.europa.eu/
* https://www.emailsecuritygrader.com/
* https://en.internet.nl/mail/
* https://tls.imirhil.fr/tls/

## other

**no guarantees that any of this will work**

* i mainly wrote this for myself
* no explanation for why an option is configured that way is provided, ever

### todo
* MTA-STS
* DNSSEC
* DANE

### backing up your Maildir
the folder needs to be accessible to `$SSH_USER` for this to work. since mail users are just regular users on the system (and authenticated with pam), you may need to `rsync` each user's `~/Maildir` to a directory which is accessible by `$SSH_USER`.

`rsync -e 'ssh -p $PORT' -avzud --delete $SSH_USER@$IP:/path/to/accessible/Maildir/ ~/Documents/backups/mailbackup/`
