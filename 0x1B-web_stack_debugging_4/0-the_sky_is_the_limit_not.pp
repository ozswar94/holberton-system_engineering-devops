#fix error nginx server

exec { 'fix error':
    path    => '/usr/local/bin/:/bin/',
    command => 'sed -i "s/15/4096/" /etc/default/nginx'
}

exec { 'restart ngnix server':
    path    => '/usr/local/bin/:/bin/:/usr/bin/',
    command => 'sudo service nginx restart'
}
