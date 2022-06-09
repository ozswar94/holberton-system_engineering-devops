#fix error nginx server

exec { 'fix error':
    path    => '/usr/local/bin/:/bin/',
    command => 'sed -i "s/worker_processes 4;/worker_processes 8;/g" /etc/nginx/nginx.conf'
}

exec { 'restart ngnix server':
    path    => '/usr/local/bin/:/bin/',
    command => 'sudo service nginx restart'
}
